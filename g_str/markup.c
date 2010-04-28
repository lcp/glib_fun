/* gcc `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` markup.c -o markup */

#include <stdio.h>
#include <glib.h>

char *
replace_substring (char *source, char *target, char *replace)
{
	char **str_set, *output;

	str_set = g_strsplit (source, target, 0);
	output = g_strjoinv (replace, str_set);
	g_strfreev (str_set);

	return output;
}

char *
markup_text (char *source)
{
	char *str1, *str2;

	str1 = source;
	str2 = replace_substring (str1, "&amp;", "&");
	str1 = replace_substring (str2, "&lt;", "<");
	g_free (str2);
	str2 = replace_substring (str1, "&gt;", ">");
	g_free (str1);
	str1 = replace_substring (str2, "&quot;", "\"");
	g_free (str2);
	str2 = replace_substring (str1, "&apos;", "\'");
	g_free (str1);

	return str2;
}

int
main()
{
	char *input = "&amp;howcan&lt;I&gt;Know&quot;This&apos;istrue";
	char *output;

//	output = g_markup_escape_text (input, -1);
	output = markup_text (input);
	printf("%s\n", output);

	return 0;
}
