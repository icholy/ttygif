/*
 * Copyright (c) 2000 Satoru Takabayashi <satoru@namazu.org>
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <string.h>

#include "ttyrec.h"
#include "util.h"
#include "io.h"

typedef int    (*ReadFunc)    (FILE *fp, Header *h, char **buf);
typedef void   (*WriteFunc)   (char *buf, int len);
typedef void   (*ProcessFunc) (FILE *fp, char *cmd_tmpl, ReadFunc read_func);

struct timeval
timeval_diff (struct timeval tv1, struct timeval tv2)
{
    struct timeval diff;

    diff.tv_sec = tv2.tv_sec - tv1.tv_sec;
    diff.tv_usec = tv2.tv_usec - tv1.tv_usec;

    if (diff.tv_usec < 0) {
        diff.tv_sec--;
        diff.tv_usec += 1000000;
    }

    return diff;
}

int
ttydelay (struct timeval prev, struct timeval cur)
{
    struct timeval diff = timeval_diff(prev, cur);
    if (diff.tv_sec < 0) {
      diff.tv_sec = diff.tv_usec = 0;
    }
    return diff.tv_sec;
}

int
ttyread (FILE *fp, Header *h, char **buf)
{
    if (read_header(fp, h) == 0) {
        return 0;
    }

    *buf = malloc(h->len);
    if (*buf == NULL) {
        perror("malloc");
    }
	
    if (fread(*buf, 1, h->len, fp) == 0) {
        perror("fread");
    }
    return 1;
}

int
ttypread (FILE *fp, Header *h, char **buf)
{
    /*
     * Read persistently just like tail -f.
     */
    while (ttyread(fp, h, buf) == 0) {
        struct timeval w = {0, 250000};
        select(0, NULL, NULL, NULL, &w);
        clearerr(fp);
    }
    return 1;
}

void
ttywrite (char *buf, int len)
{
    fwrite(buf, 1, len, stdout);
}

void
clear_screen (void) {
    printf("\e[1;1H\e[2J");
}

char *
make_cmd (char *template, int index, double delay)
{
    char *cmd;
    char index_str[10];
    char delay_str[10];

    sprintf(index_str, "%d", index);
    sprintf(delay_str, "%f", delay);

    cmd = str_replace(template, "{{delay}}", delay_str);
    if (!cmd) { return NULL; }
    
    cmd = str_replace(cmd, "{{index}}", index_str);
    if (!cmd) { return NULL; }

    return cmd;
}

void
ttyplay (FILE *fp, char *cmd_tmpl, ReadFunc read_func, WriteFunc write_func)
{
    int index = 0;
    int delay = 0;
    struct timeval prev;

    clear_screen();

    setbuf(stdout, NULL);
    setbuf(fp, NULL);

    while (1) {

        char *buf;
        Header h;

        if (read_func(fp, &h, &buf) == 0) {
            break;
        }

        if (index != 0) {
            delay = ttydelay(prev, h.tv);
        }

        char *cmd = make_cmd(cmd_tmpl, index, delay);
        if (!cmd) {
          perror("malloc");
          break;
        }

        if (system(cmd) != 0) {
            perror("system");
            break;
        }
        index++;

        write_func(buf, h.len);
        prev = h.tv;
        free(buf);
        free(cmd);
    }
}

void ttyplayback (FILE *fp, char *cmd_tmpl, ReadFunc read_func)
{
    ttyplay(fp, cmd_tmpl, ttyread, ttywrite);
}

void
usage (void)
{
    printf("Usage: ttygif [FILE] \"[SHELL COMMAND]\"\n");
    exit(EXIT_FAILURE);
}

/*
 * We do some tricks so that select(2) properly works on
 * STDIN_FILENO in ttywait().
 */
FILE *
input_from_stdin (void)
{
    FILE *fp;
    int fd = edup(STDIN_FILENO);
    edup2(STDOUT_FILENO, STDIN_FILENO);
    return efdopen(fd, "r");
}

int 
main (int argc, char **argv)
{
    ReadFunc read_func  = ttyread;
    ProcessFunc process = ttyplayback;
    FILE *input = NULL;
    struct termios old, new;
    char *cmd_template;

    if (argc < 3) {
      usage();
    }

    set_progname(argv[0]);
    cmd_template = argv[2];

    if (optind < argc) {
        input = efopen(argv[optind], "r");
    } else {
        input = input_from_stdin();
    }
    assert(input != NULL);

    tcgetattr(0, &old); /* Get current terminal state */
    new = old;          /* Make a copy */
    new.c_lflag &= ~(ICANON | ECHO | ECHONL); /* unbuffered, no echo */
    tcsetattr(0, TCSANOW, &new); /* Make it current */

    process(input, cmd_template, read_func);
    tcsetattr(0, TCSANOW, &old);  /* Return terminal state */

    return 0;
}
