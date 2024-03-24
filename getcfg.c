
#include	"distchart.h"

void getcfg ( char *Filename )
{
	FILE	*fp;
	char	xbuffer[256];
	char	*tokens[MAXTOKS];
	int		tokcnt, xt;
	int		xo;

	if (( fp = fopen ( Filename, "r" )) == NULL )
	{
		printf ( "Cannot open cfg file %s\n", Filename );
		exit ( 1 );
	}

	Bands = 0;
	StartCol = 1;	// human value
	EndCol = 1;		// human value
	sprintf ( Delimeter, "\n" );
	Headers = 0;

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != NULL )
	{
		if ( xbuffer[0] == '#' )
		{
			continue;
		}

/*----------------------------------------------------------
#
#	CEO.cfg
#

ranges	10
bounds 0 100 200 300 400 500 600 700 800 900 1000
header n
delimeter	,
columns	1 1
----------------------------------------------------------*/

		if (( tokcnt = GetTokensD ( xbuffer, " \n\r\t", tokens, MAXTOKS )) < 2 )
		{
			continue;
		}

		if ( strcmp ( tokens[0], "ranges" ) == 0 )
		{
			Bands = atoi ( tokens[1] );

			if (( Data = calloc ( Bands, sizeof(RECORD) )) == NULL )
			{
				printf ( "calloc failed\n" );
				exit ( 1 );
			}
		}
		else if ( strcmp ( tokens[0], "bounds" ) == 0 )
		{
			if ( Bands == 0 )
			{
				printf ( "Specify 'ranges' before 'bounds'\n" );
				exit ( 1 );
			}
			if ( tokcnt - 2 != Bands )
			{
				printf ( "Incorrect number of 'bounds'\n" );
				exit ( 1 );
			}
			for ( xt = 1, xo = 0; xt < tokcnt - 1 && xo < Bands; xt++, xo++ )
			{
				Data[xo].Start = atoi ( tokens[xt] );
				Data[xo].End   = atoi ( tokens[xt+1] );
			}
		}
		else if ( strcmp ( tokens[0], "header" ) == 0 )
		{
			switch ( tokens[1][0] )
			{
				case 'n':
				case 'N':
				case '0':
					Headers = 0;
					break;

				case 'y':
				case 'Y':
				case '1':
					Headers = 1;
					break;

				default:
					printf ( "Unknown value for 'headers'\n" );
					exit ( 1 );
			}
		}
		else if ( strcmp ( tokens[0], "delimeter" ) == 0 )
		{
			sprintf ( Delimeter, "%c", tokens[2][0] );
		}
		else if ( tokcnt > 2 && strcmp ( tokens[0], "columns" ) == 0 )
		{
			StartCol = atoi ( tokens[1] );
			EndCol   = atoi ( tokens[2] );
		}
		else
		{
			printf ( "Unknown line in config file\n" );
			exit ( 1  );
		}
	}

	fclose ( fp );
}
