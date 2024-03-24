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
	tms		09/29/2023	Added -cfg file for args and added specified ranges

----------------------------------------------------------------------------*/

#define		MAIN
#include	"distchart.h"

static	char	ColumnHeader[24];
static	int		Outliers = 0;

static void InitFromData ( int Column )
{
	double	Min = 0.0;
	double	Max = 0.0;
	double	Value;
	int		lineno = 0;
	char	xbuffer[1024];
	char	*tokens[MAXTOKS];
	int		tokcnt;

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

}

static void EachColumn ( int Column )
{
	double	Value;
	int		lineno = 0;
	char	xbuffer[1024];
	char	*tokens[MAXTOKS];
	int		tokcnt;
	int		TotalCount;

	if ( GotCfg == 0 )
	{
		InitFromData ( Column );
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

		if ( Value < Data[0].Start || Value > Data[Bands-1].End )
		{
			Outliers++;
		}
		else
		{
			for ( int b = 0; b < Bands; b++ )
			{
				if ( Value >= Data[b].Start && Value <= Data[b].End + 0.0001 )
				{
					Data[b].Count++;
					break;
				}
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

	for ( int c = StartCol; c <= EndCol; c++ )
	{
		EachColumn ( c );
	}

	if ( Outliers )
	{
		printf ( "Ignored %d outliers\n", Outliers );
	}


	free ( Data );

	fclose ( ifp );

	return ( 0 );
}
