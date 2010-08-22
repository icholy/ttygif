/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* 1999-02-22 Arkadiusz Mi¶kiewicz <misiek@misiek.eu.org>
 * - added Native Language Support
 */

/* 2000-12-27 Satoru Takabayashi <satoru@namazu.org>
 * - modify `script' to create `ttyrec'.
 */

/*
 * script
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/signal.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#if defined(SVR4)
#include <fcntl.h>
#include <stropts.h>
#endif /* SVR4 */

#include <sys/time.h>
#include "ttyrec.h"
#include "io.h"

#define HAVE_inet_aton
#define HAVE_scsi_h
#define HAVE_kd_h

#define _(FOO) FOO

#ifdef HAVE_openpty
#include <libutil.h>
#endif

#if defined(SVR4) && !defined(CDEL)
#if defined(_POSIX_VDISABLE)
#define CDEL _POSIX_VDISABLE
#elif defined(CDISABLE)
#define CDEL CDISABLE
#else /* not _POSIX_VISIBLE && not CDISABLE */
#define CDEL 255
#endif /* not _POSIX_VISIBLE && not CDISABLE */
#endif /* SVR4 && ! CDEL */

void done(void);
void fail(void);
void fixtty(void);
void getmaster(void);
void getslave(void);
void doinput(void);
void dooutput(void);
void doshell(const char*);

char	*shell;
FILE	*fscript;
int	master;
int	slave;
int	child;
int	subchild;
char	*fname;

struct	termios tt;
struct	winsize win;
int	lb;
int	l;
#if !defined(SVR4)
#ifndef HAVE_openpty
char	line[] = "/dev/ptyXX";
#endif
#endif /* !SVR4 */
int	aflg;
int	uflg;

int
main(argc, argv)
	int argc;
	char *argv[];
{
	extern int optind;
	int ch;
	void finish();
	char *getenv();
	char *command = NULL;

	while ((ch = getopt(argc, argv, "aue:h?")) != EOF)
		switch((char)ch) {
		case 'a':
			aflg++;
			break;
		case 'u':
		        uflg++;
			break;
		case 'e':
			command = strdup(optarg);
			break;
		case 'h':
		case '?':
		default:
			fprintf(stderr, _("usage: ttyrec [-u] [-e command] [-a] [file]\n"));
			exit(1);
		}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		fname = argv[0];
	else
		fname = "ttyrecord";
	if ((fscript = fopen(fname, aflg ? "a" : "w")) == NULL) {
		perror(fname);
		fail();
	}
	setbuf(fscript, NULL);

	shell = getenv("SHELL");
	if (shell == NULL)
		shell = "/bin/sh";

	getmaster();
	fixtty();

	(void) signal(SIGCHLD, finish);
	child = fork();
	if (child < 0) {
		perror("fork");
		fail();
	}
	if (child == 0) {
		subchild = child = fork();
		if (child < 0) {
			perror("fork");
			fail();
		}
		if (child)
			dooutput();
		else
			doshell(command);
	}
	doinput();

	return 0;
}

void
doinput()
{
	register int cc;
	char ibuf[BUFSIZ];

	(void) fclose(fscript);
#ifdef HAVE_openpty
	(void) close(slave);
#endif
	while ((cc = read(0, ibuf, BUFSIZ)) > 0)
		(void) write(master, ibuf, cc);
	done();
}

#include <sys/wait.h>

void
finish()
{
#if defined(SVR4)
	int status;
#else /* !SVR4 */
	union wait status;
#endif /* !SVR4 */
	register int pid;
	register int die = 0;

	while ((pid = wait3((int *)&status, WNOHANG, 0)) > 0)
		if (pid == child)
			die = 1;

	if (die)
		done();
}

struct linebuf {
    char str[BUFSIZ + 1]; /* + 1 for an additional NULL character.*/
    int len;
};


void
check_line (const char *line)
{
    static int uuencode_mode = 0;
    static FILE *uudecode;

    if (uuencode_mode == 1) {
	fprintf(uudecode, "%s", line);
	if (strcmp(line, "end\n") == 0) {
	    pclose(uudecode);
	    uuencode_mode = 0;
	}
    } else {
	int dummy; char dummy2[BUFSIZ];
	if (sscanf(line, "begin %o %s", &dummy, dummy2) == 2) {
	    /* 
	     * uuencode line found! 
	     */
	    uudecode = popen("uudecode", "w");
	    fprintf(uudecode, "%s", line);
	    uuencode_mode = 1;
	}
    }
}

void
check_output(const char *str, int len)
{
    static struct linebuf lbuf = {"", 0};
    int i;

    for (i = 0; i < len; i++) {
	if (lbuf.len < BUFSIZ) {
	    lbuf.str[lbuf.len] = str[i];
	    if (lbuf.str[lbuf.len] == '\r') {
		lbuf.str[lbuf.len] = '\n';
	    }
	    lbuf.len++;
	    if (lbuf.str[lbuf.len - 1] == '\n') {
		if (lbuf.len > 1) { /* skip a blank line. */
		    lbuf.str[lbuf.len] = '\0';
		    check_line(lbuf.str);
		}
		lbuf.len = 0;
	    }
	} else {/* buffer overflow */
	    lbuf.len = 0;
	}
    }
}

void
dooutput()
{
	int cc;
	char obuf[BUFSIZ];

	setbuf(stdout, NULL);
	(void) close(0);
#ifdef HAVE_openpty
	(void) close(slave);
#endif
	for (;;) {
		Header h;

		cc = read(master, obuf, BUFSIZ);
		if (cc <= 0)
			break;
		if (uflg)
		    check_output(obuf, cc);
		h.len = cc;
		gettimeofday(&h.tv, NULL);
		(void) write(1, obuf, cc);
		(void) write_header(fscript, &h);
		(void) fwrite(obuf, 1, cc, fscript);
	}
	done();
}

void
doshell(const char* command)
{
	/***
	int t;

	t = open(_PATH_TTY, O_RDWR);
	if (t >= 0) {
		(void) ioctl(t, TIOCNOTTY, (char *)0);
		(void) close(t);
	}
	***/
	getslave();
	(void) close(master);
	(void) fclose(fscript);
	(void) dup2(slave, 0);
	(void) dup2(slave, 1);
	(void) dup2(slave, 2);
	(void) close(slave);

	if (!command) {
		execl(shell, strrchr(shell, '/') + 1, "-i", 0);
	} else {
		execl(shell, strrchr(shell, '/') + 1, "-c", command, 0);	
	}
	perror(shell);
	fail();
}

void
fixtty()
{
	struct termios rtt;

	rtt = tt;
#if defined(SVR4)
	rtt.c_iflag = 0;
	rtt.c_lflag &= ~(ISIG|ICANON|XCASE|ECHO|ECHOE|ECHOK|ECHONL);
	rtt.c_oflag = OPOST;
	rtt.c_cc[VINTR] = CDEL;
	rtt.c_cc[VQUIT] = CDEL;
	rtt.c_cc[VERASE] = CDEL;
	rtt.c_cc[VKILL] = CDEL;
	rtt.c_cc[VEOF] = 1;
	rtt.c_cc[VEOL] = 0;
#else /* !SVR4 */
	cfmakeraw(&rtt);
	rtt.c_lflag &= ~ECHO;
#endif /* !SVR4 */
	(void) tcsetattr(0, TCSAFLUSH, &rtt);
}

void
fail()
{

	(void) kill(0, SIGTERM);
	done();
}

void
done()
{
	if (subchild) {
		(void) fclose(fscript);
		(void) close(master);
	} else {
		(void) tcsetattr(0, TCSAFLUSH, &tt);
	}
	exit(0);
}

void
getmaster()
{
#if defined(SVR4)
	(void) tcgetattr(0, &tt);
	(void) ioctl(0, TIOCGWINSZ, (char *)&win);
	if ((master = open("/dev/ptmx", O_RDWR)) < 0) {
		perror("open(\"/dev/ptmx\", O_RDWR)");
		fail();
	}
#else /* !SVR4 */
#ifdef HAVE_openpty
	(void) tcgetattr(0, &tt);
	(void) ioctl(0, TIOCGWINSZ, (char *)&win);
	if (openpty(&master, &slave, NULL, &tt, &win) < 0) {
		fprintf(stderr, _("openpty failed\n"));
		fail();
	}
#else
#ifdef HAVE_getpt
	if ((master = getpt()) < 0) {
		perror("getpt()");
		fail();
	}
#else
	char *pty, *bank, *cp;
	struct stat stb;

	pty = &line[strlen("/dev/ptyp")];
	for (bank = "pqrs"; *bank; bank++) {
		line[strlen("/dev/pty")] = *bank;
		*pty = '0';
		if (stat(line, &stb) < 0)
			break;
		for (cp = "0123456789abcdef"; *cp; cp++) {
			*pty = *cp;
			master = open(line, O_RDWR);
			if (master >= 0) {
				char *tp = &line[strlen("/dev/")];
				int ok;

				/* verify slave side is usable */
				*tp = 't';
				ok = access(line, R_OK|W_OK) == 0;
				*tp = 'p';
				if (ok) {
					(void) tcgetattr(0, &tt);
				    	(void) ioctl(0, TIOCGWINSZ, 
						(char *)&win);
					return;
				}
				(void) close(master);
			}
		}
	}
	fprintf(stderr, _("Out of pty's\n"));
	fail();
#endif /* not HAVE_getpt */
#endif /* not HAVE_openpty */
#endif /* !SVR4 */
}

void
getslave()
{
#if defined(SVR4)
	(void) setsid();
	grantpt( master);
	unlockpt(master);
	if ((slave = open((const char *)ptsname(master), O_RDWR)) < 0) {
		perror("open(fd, O_RDWR)");
		fail();
	}
	if (isastream(slave)) {
		if (ioctl(slave, I_PUSH, "ptem") < 0) {
			perror("ioctl(fd, I_PUSH, ptem)");
			fail();
		}
		if (ioctl(slave, I_PUSH, "ldterm") < 0) {
			perror("ioctl(fd, I_PUSH, ldterm)");
			fail();
		}
#ifndef _HPUX_SOURCE
		if (ioctl(slave, I_PUSH, "ttcompat") < 0) {
			perror("ioctl(fd, I_PUSH, ttcompat)");
			fail();
		}
#endif
		(void) ioctl(0, TIOCGWINSZ, (char *)&win);
	}
#else /* !SVR4 */
#ifndef HAVE_openpty
	line[strlen("/dev/")] = 't';
	slave = open(line, O_RDWR);
	if (slave < 0) {
		perror(line);
		fail();
	}
	(void) tcsetattr(slave, TCSAFLUSH, &tt);
	(void) ioctl(slave, TIOCSWINSZ, (char *)&win);
#endif
	(void) setsid();
	(void) ioctl(slave, TIOCSCTTY, 0);
#endif /* SVR4 */
}
