/*   This is part of um-ViewOS
 *   The user-mode implementation of OSVIEW -- A Process with a View
 *
 *   umfuse parameters management
 *   
 *   Copyright 2005 Renzo Davoli University of Bologna - Italy
 *   Modified 2005 Paolo Angelelli, Andrea Seraghiti
 *   
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License, version 2, as
 *   published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *   $Id: umfuseargs.c 913 2011-01-06 17:46:23Z rd235 $
 *
 */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fuse/fuse.h>
#include "vufusestd.h"

#define MAXARGS 256

//#define FUSEEXCEPT 9  //"except" /*NEGATIVE MOUNT to be discussed*/
//#define FUSEARGHUMAN 10 //"human"	/*HUMAN MODE to be discussed*/
//#define FUSEARGHARDREMOVE 13 //"fsdebug" /*MNT_DETACH at the moment*/

#define FUSEARGMERGE 11 //"merge"

//#define FUSEFLAGHASSTRING 1

static struct fuseargitem {
	char *arg;
	char tag;
	char flags;
} fuseargtab[] = {
	//{"except=",FUSEEXCEPT, FUSEFLAGHASSTRING},
	//{"hard_remove",FUSEARGHARDREMOVE, 0},
	//{"human", FUSEARGHUMAN, 0},
	{"merge", FUSEARGMERGE, 0},
};
#define FUSEARGTABSIZE sizeof(fuseargtab)/sizeof(struct fuseargitem)

static char dasho[]="-o";
static int fuseaddargs(char *fmt, char *source, char *mountpoint, char *opts, char ***pargv, int nargc)
{
	char *s=fmt;
	int newfield=1;
	char quote=0;
	int oldnargc=nargc;
	
	/* from space-separated fields to argv format*/
	while (s != NULL && *s != 0) {
			switch (*s) {
				case ' ':
					if (quote==0) {
						*s=0;
						if (newfield==0) newfield=1;
					} 
					break;
				case '%':
					switch (*(s+1)) {
						case 'O':if (opts != NULL && *opts != 0) {
								 pargv[0][nargc++]=dasho;
								 pargv[0][nargc++]=opts;
							 }
							 s++; 
							 break;
						case 'S':if (source != NULL) 
								 pargv[0][nargc++]=source;
							 s++;
							 break;
						case 'D':
						case 'M':
							 if (mountpoint != NULL) 
								 pargv[0][nargc++]=mountpoint;
							 s++;
							 break;
					}
					break;
				case '\'':
				case '\"':
					quote=(*s == quote)?0:*s;
				/* roll down */
				default:
				if (newfield) {
					newfield=0;
					if (nargc<MAXARGS)
						pargv[0][nargc++]=s;
				}
				break;
			}
		s++;
	}
	int i;
	/* unwrap one quotation layer */
	for (i=oldnargc;i<nargc;i++) {
		char *s,*t;
		quote=0;
		for (s=t=pargv[0][i];*s != 0;s++) {
			if (quote == 0 && *s=='\\' && *(s+1) != 0) {
				*t++=*++s;
			} else if (*s == '\'' || *s == '\"') {
				if (quote==0) {
					quote=*s;
				} else if (quote==*s) {
					quote=0;
				} else
					*t++=*s;
			} else
				*t++=*s;
		}
		*t=0;
	}

	return nargc;
}

int fuseargs(char* filesystemtype,char *source, char *mountpoint, char *opts, char ***pargv,struct fuse_context *fc,unsigned long *pflags,char ***pexceptions)
{
	char *sepopts[MAXARGS];
	//char *exceptions[MAXARGS];
	int nsepopts=0;
	char newopts[PATH_MAX];
	//int nexceptions=0;
	int i;
	newopts[0]=0;
	char *s=opts;
	char quote=0,olds;

	/* PHASE 1: tokenize options */
	for (quote=0,s=opts,olds=*s;olds != 0 && nsepopts < MAXARGS;s++) {
		sepopts[nsepopts++]=s;
		while (*s != 0 && (*s != ',' || quote != 0))
		{
			if (*s=='\\' && *(s+1)!=0)
				s+=2;
			if (*s=='\'' || *s=='\"') {
				if (*s == quote)
					quote=0;
				else
					if (quote==0)
						quote=*s;
			}
			s++;
		}
		olds=*s;*s=0;
	}

	/* PHASE 2 recognize UMFUSE options */
	for (i=0;i<nsepopts;i++) {
		int j;
		for (j=0; j<FUSEARGTABSIZE && 
				strncmp(sepopts[i],fuseargtab[j].arg,strlen(fuseargtab[j].arg)) != 0; j++)
				;
		switch ((j<FUSEARGTABSIZE)?fuseargtab[j].tag:0) {
			/*case FUSEEXCEPT:
				exceptions[nexceptions]=sepopts[i]+strlen(fuseargtab[j].arg);
				nexceptions++;
				break;*/
			/*case FUSEARGHUMAN:
				*pflags |= FUSE_HUMAN;*/
				break;
			case FUSEARGMERGE:
				*pflags |= FUSE_MERGE;
				break;
			/*case FUSEARGHARDREMOVE:
				*pflags |= FUSE_HARDREMOVE;
				break;*/
			default:
				{
					int len=PATH_MAX-strlen(newopts);
					if (len < 0) len=0;
					if (*newopts != 0)
						strncat(newopts,",",len--);
					if (len < 0) len=0;
					strncat(newopts,sepopts[i],len);
				}
				break;
		}
	}
	/* PHASE 2B set up exceptions (if there are) */
	/*if (nexceptions > 0) {
		char **newexceptions=*pexceptions=malloc((nexceptions+1)*sizeof(char *));
		if (newexceptions) {
			int i;
			for (i=0;i<nexceptions;i++)
				newexceptions[i]=strdup(exceptions[i]);
			newexceptions[i]=0;
		}
	}*/

	/* PHASE 3 build newargc newargv */
	char *nargv[MAXARGS];
	int nargc=0;
	char **pnargv=nargv;
	nargv[nargc++]=filesystemtype; // argv[0] is filesystemtype


	if (*newopts != 0) {
		nargv[nargc++]="-o";
		nargv[nargc++]=newopts;
	}
	if (source != NULL)
		nargv[nargc++]=source;
	if (mountpoint != NULL)
		nargv[nargc++]=mountpoint;
	

	*pargv = (char **) malloc (nargc * sizeof(char*));
	for (i=0; i<nargc;i++) {
		pargv[0][i]=strdup(nargv[i]);
	}
	return nargc;
}

void fusefreearg(int argc,char *argv[])
{
	int i;
	for (i=0;i<argc;i++)
		free(argv[i]);
	free(argv);
}


void freeexceptions(char **exceptions)
{
	if (__builtin_expect((exceptions == NULL),1))
		return;
	else {
		char **excscan=exceptions;
		while (*excscan != 0) {
			free(*excscan);
			excscan++;
		}
		free(exceptions);
	}
}
