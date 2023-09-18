/*---------------------------------------------------------------------------
	Copyright © 2023 Tom Stevelt and Silver Hammer Software LLC
	Tom Stevelt <tstevelt@silverhammersoftware.com>
	This software is free software; you can redistribute it and/or modify
	it under the terms of the MIT license. See LICENSE for details.
---------------------------------------------------------------------------*/

#include	"shslib.h"

int GetTokensD ( char xbuf[], char delim[], char *toks[], int maxtoks )
{
	int		rv = 0;
	int		xl, xi, xf;

	if ( maxtoks < 0 )
		return ( rv );

	xl = strlen ( xbuf );
	xf = 1;
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( strchr ( delim, xbuf[xi] ) != (char *) 0 )
		{
			xbuf[xi] = '\0';
			if ( xf == 0 )
				xf = 1;
		}
		else
		{
			if ( xf )
			{
				xf = 0;
				toks[rv] = &xbuf[xi];
				rv++;

				if ( rv >= maxtoks )
					return ( rv );
			}
		}
	}

	return ( rv );
}
