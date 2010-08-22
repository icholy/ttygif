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

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ttyrec.h"

#define SWAP_ENDIAN(val) ((unsigned int) ( \
    (((unsigned int) (val) & (unsigned int) 0x000000ffU) << 24) | \
    (((unsigned int) (val) & (unsigned int) 0x0000ff00U) <<  8) | \
    (((unsigned int) (val) & (unsigned int) 0x00ff0000U) >>  8) | \
    (((unsigned int) (val) & (unsigned int) 0xff000000U) >> 24)))

static int 
is_little_endian ()
{
    static int retval = -1;

    if (retval == -1) {
	int n = 1;
	char *p = (char *)&n;
	char x[] = {1, 0, 0, 0};

	assert(sizeof(int) == 4);

	if (memcmp(p, x, 4) == 0) {
	    retval = 1;
	} else {
	    retval = 0;
	}
    }

    return retval;
}

static int
convert_to_little_endian (int x)
{
    if (is_little_endian()) {
	return x;
    } else {
	return SWAP_ENDIAN(x);
    }
}

int
read_header (FILE *fp, Header *h)
{
    int buf[3];

    if (fread(buf, sizeof(int), 3, fp) == 0) {
	return 0;
    }

    h->tv.tv_sec  = convert_to_little_endian(buf[0]);
    h->tv.tv_usec = convert_to_little_endian(buf[1]);
    h->len        = convert_to_little_endian(buf[2]);

    return 1;
}

int
write_header (FILE *fp, Header *h)
{
    int buf[3];

    buf[0] = convert_to_little_endian(h->tv.tv_sec);
    buf[1] = convert_to_little_endian(h->tv.tv_usec);
    buf[2] = convert_to_little_endian(h->len);

    if (fwrite(buf, sizeof(int), 3, fp) == 0) {
	return 0;
    }

    return 1;
}

static char *progname = "";
void
set_progname (const char *name)
{
    progname = strdup(name);
}

FILE *
efopen (const char *path, const char *mode)
{
    FILE *fp = fopen(path, mode);
    if (fp == NULL) {
	fprintf(stderr, "%s: %s: %s\n", progname, path, strerror(errno));
	exit(EXIT_FAILURE);
    }
    return fp;
}

int
edup (int oldfd)
{
    int fd = dup(oldfd);
    if (fd == -1) {
	fprintf(stderr, "%s: dup failed: %s\n", progname, strerror(errno));
	exit(EXIT_FAILURE);
    }
    return fd;
}

int
edup2 (int oldfd, int newfd)
{
    int fd = dup2(oldfd, newfd);
    if (fd == -1) {
	fprintf(stderr, "%s: dup2 failed: %s\n", progname, strerror(errno));
	exit(EXIT_FAILURE);
    }
    return fd;
}

FILE *
efdopen (int fd, const char *mode)
{
    FILE *fp = fdopen(fd, mode);
    if (fp == NULL) {
	fprintf(stderr, "%s: fdopen failed: %s\n", progname, strerror(errno));
	exit(EXIT_FAILURE);
    }
}
