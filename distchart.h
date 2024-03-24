
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"shslib.h"

#ifdef MAIN
#define		TYPE	/* */
#else
#define		TYPE	extern
#endif

typedef struct
{
	double	Start;
	double	End;
	int		Count;
} RECORD;

TYPE	RECORD	*Data;

TYPE	FILE	*ifp;
TYPE	int		Bands;
TYPE	char	*File;
TYPE	int		Headers;
TYPE	char	Delimeter[2];
TYPE	int		StartCol;
TYPE	int		EndCol;
TYPE	int		GotCfg;

#define		MAXTOKS		100


/* distchart.c */
int main ( int argc , char *argv []);

/* getargs.c */
void getargs ( int argc , char *argv []);

/* getcfg.c */
void getcfg ( char *Filename );

/* shslib.c */
int GetTokensD ( char xbuf [], char delim [], char *toks [], int maxtoks );
