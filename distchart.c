/*----------------------------------------------------------------------------
	Program : distchart.c
	Author  : Silver Hammer Software LLC
	Author  : Tom Stevelt
	Date    : Sep 2023
	Synopsis: Construct distribution chart for numeric values.
	Return  : 

	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"shslib.h"

typedef struct
{
	double	Start;
	double	End;
	int		Count;
} RECORD;

static	RECORD	*Data;

static	FILE	*ifp;
static	int		Bands = 0;
static	char	*File = NULL;
static	int		Headers = 0;;
static	char	Delimeter[2];
static	int		StartCol = 1;	// human value
static	int		EndCol = 1;		// human value

#define		MAXTOKS		20

static void Usage ()
{
	printf ( "USAGE: distchart -r 5-50 -in file [-h] [-d x] [-c # [#]]\n" );
	printf ( "  -r       - ranges\n" );
	printf ( "  -in file - data file\n" );
	printf ( "  -h       - column headers in first line\n" );
	printf ( "  -d x     - delimeter character\n" );
	printf ( "  -c # #   - columns to analyze\n" );
	exit ( 1 );
}

static void getargs ( int argc, char *argv[] )
{
	Bands = 0;
	sprintf ( Delimeter, "\n" );
	for ( int xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && strcmp ( argv[xa], "-r" ) == 0 )
		{
			xa++;
			Bands = atoi ( argv[xa] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-in" ) == 0 )
		{
			xa++;
			File =  argv[xa];
		}
		else if ( strcmp ( argv[xa], "-h" ) == 0 )
		{
			Headers = 1;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-d" ) == 0 )
		{
			xa++;
			sprintf ( Delimeter, "%c", argv[xa][0] );
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-c" ) == 0 )
		{
			xa++;
			StartCol = EndCol = atoi ( argv[xa] );
			if ( xa + 1 < argc && argv[xa+1][0] != '-' )
			{
				xa++;
				EndCol = atoi ( argv[xa] );
			}
		}
		else
		{
			Usage ();
		}
	}
	if ( Bands < 5 || Bands > 50 )
	{
		Usage ();
	}
	if ( File == NULL )
	{
		Usage ();
	}
}

static void EachColumn ( int Column )
{
	double	Min = 0.0;
	double	Max = 0.0;
	double	Value;
	int		lineno = 0;
	char	xbuffer[1024];
	char	*tokens[MAXTOKS];
	int		tokcnt;
	int		TotalCount;
	char	ColumnHeader[24];

	rewind ( ifp );

	/*----------------------------------------------------------
		Column is human based ( not zero based ).
	----------------------------------------------------------*/

	lineno = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( xbuffer, Delimeter, tokens, MAXTOKS )) < Column )
		{
			continue;
		}

		if ( lineno == 1 && Headers == 1 )
		{
			snprintf ( ColumnHeader, sizeof(ColumnHeader), "%s", tokens[Column-1] );
			continue;
		}

		Value = atof ( tokens[Column-1] );

		if (( lineno == 1 && Headers == 0 ) ||
			( lineno == 2 && Headers == 1 ))
		{
			Min = Max = Value;
		}
		else
		{
			if ( Min > Value )
			{
				Min = Value;
			}
			else if ( Max < Value )
			{
				Max = Value;
			}
		}
	}

	printf ( "lines %d, min %g max %g\n", lineno, Min, Max );

	double	Increment = (Max - Min) / (double) Bands;
	Data[0].Start = Min;
	Data[0].End = Data[0].Start + Increment;

	for ( int b = 1; b < Bands; b++ )
	{
		Data[b].Start = Data[b-1].End;
		Data[b].End = Data[b].Start + Increment;
	}

	rewind ( ifp );

	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != NULL )
	{
		lineno++;

		if (( tokcnt = GetTokensD ( xbuffer, Delimeter, tokens, MAXTOKS )) < Column )
		{
			continue;
		}

		Value = atof ( tokens[Column-1] );

		for ( int b = 0; b < Bands; b++ )
		{
			if ( Value >= Data[b].Start && Value <= Data[b].End )
			{
				Data[b].Count++;
				break;
			}
		}
	}

	printf ( "File %s\n", File );

	if ( Headers == 0 && EndCol > StartCol )
	{
		printf ( "Column %d\n", Column );
	}
	else if ( Headers == 1 )
	{
		printf ( "Column %s\n", ColumnHeader );
	}

	TotalCount = 0;
	for ( int b = 0; b < Bands; b++ )
	{
		printf ( "%g %g %d\n", Data[b].Start, Data[b].End, Data[b].Count );
		TotalCount += Data[b].Count;
	}

	printf ( "Total count %d\n", TotalCount );
}

int main ( int argc, char *argv[] )
{
	getargs ( argc, argv );
	
	if (( ifp = fopen ( File, "r" )) == NULL )
	{
		fprintf ( stderr, "Cannot open %s for input\n", File );
		exit ( 1 );
	}

	if (( Data = calloc ( Bands, sizeof(RECORD) )) == NULL )
	{
		fprintf ( stderr, "calloc failed\n" );
		exit ( 1 );
	}

	for ( int c = StartCol; c <= EndCol; c++ )
	{
		EachColumn ( c );
	}

	free ( Data );

	fclose ( ifp );

	return ( 0 );
}
