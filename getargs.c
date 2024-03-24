
#include	"distchart.h"

static void Usage ()
{
	printf ( "USAGE: distchart -r 5-50       -in file [-h] [-d x] [-c # [#]]\n" );
	printf ( "USAGE: distchart -cfg file.cfg -in file\n" );
	printf ( "  -r       - ranges\n" );
	printf ( "  -in file - data file\n" );
	printf ( "  -h       - column headers in first line\n" );
	printf ( "  -d x     - delimeter character\n" );
	printf ( "  -c # #   - columns to analyze\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	GotCfg = 0;
	Bands = 0;
	StartCol = 1;	// human value
	EndCol = 1;		// human value
	sprintf ( Delimeter, "\n" );

	for ( int xa = 1; xa < argc; xa++ )
	{
		if ( xa + 1 < argc && strcmp ( argv[xa], "-cfg" ) == 0 )
		{
			xa++;
			getcfg ( argv[xa] );
			GotCfg = 1;
		}
		else if ( xa + 1 < argc && strcmp ( argv[xa], "-in" ) == 0 )
		{
			xa++;
			File =  argv[xa];
		}
		else if ( GotCfg == 0 && xa + 1 < argc && strcmp ( argv[xa], "-r" ) == 0 )
		{
			xa++;
			Bands = atoi ( argv[xa] );
			if (( Data = calloc ( Bands, sizeof(RECORD) )) == NULL )
			{
				fprintf ( stderr, "calloc failed\n" );
				exit ( 1 );
			}
		}
		else if ( GotCfg == 0 && strcmp ( argv[xa], "-h" ) == 0 )
		{
			Headers = 1;
		}
		else if ( GotCfg == 0 && xa + 1 < argc && strcmp ( argv[xa], "-d" ) == 0 )
		{
			xa++;
			sprintf ( Delimeter, "%c", argv[xa][0] );
		}
		else if ( GotCfg == 0 && xa + 1 < argc && strcmp ( argv[xa], "-c" ) == 0 )
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

