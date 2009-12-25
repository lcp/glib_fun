#include <glib.h>
#include <glib/gprintf.h>

int
main()
{
	gchar *str="what the hell is that?";

	gchar **tokens1, **tokens2;

	tokens1 = g_strsplit ( str, "the", 1 );
	tokens2 = g_strsplit ( str, "the", 2 );

	g_printf( "tokens1: %s, %s\n", tokens1[0], tokens1[1] );
	g_printf( "tokens2: %s, %s\n", tokens2[0], tokens2[1] );

	return 0;
}
