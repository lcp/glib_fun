/*
 * html 4.0 special entities:
 * 	http://htmlhelp.com/reference/html40/entities/special.html
 * */

#include <glib.h>

#define ENTITY_NUM 4
#define ENTITY_TYPES 3
char special_entities[ENTITY_NUM][ENTITY_TYPES+1][7] = {
{"\"", "&quot;", "&#34;", "&#x22;"},
{"&",  "&amp;",  "&#38;", "&#x26;"},
{"<",  "&lt;",   "&#60;", "&#x3C;"},
{">",  "&gt;",   "&#62;", "&#x3E;"}
};

char *
translate_entities(char *in_text)
{
	int i, j;
	char **split_text;
	char *input, *output;

	input = g_strdup (in_text);

	for (i = 0; i < ENTITY_NUM; i++){
		for (j = 1; j < ENTITY_TYPES+1; j++){
			split_text = g_strsplit (input,
						 special_entities[i][j],
						 -1);
			output = g_strjoinv (special_entities[i][0],
					     split_text);
			g_strfreev (split_text);
			g_free (input);
			input = output;
		}
	}
}

int
main()
{
	char *test = ">, &gt;, &#62;, &#x3E;, "
                     "\", &quot;, &#34;, &#x22;, "
                     "&, &amp;, &#38;, &#x26;, "
                     "<, &lt;, &#60;, &#x3C;";

	g_print ("%s\n", translate_entities (test));

	return 0;
}
