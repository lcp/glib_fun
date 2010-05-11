/*
 * html 4.0 special entities:
 * 	http://htmlhelp.com/reference/html40/entities/special.html
 * List of XML and HTML character entity references
 *      http://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references
 * */

#include <glib.h>

/* Entity types: ASCII ENTITY DECIMAL HEX */
#define ENTITY_TYPES 4
char *special_entities[][ENTITY_TYPES] = {
  {"\"", "&quot;", "&#34;", "&#x22;"},
  {"&",  "&amp;",  "&#38;", "&#x26;"},
  {"\'", "&apos;", "&#39;", "&#x27;"},
  {"<",  "&lt;",   "&#60;", "&#x3C;"},
  {">",  "&gt;",   "&#62;", "&#x3E;"}
};

int entity_num = sizeof (special_entities) / (sizeof (char *) * ENTITY_TYPES);

char *
translate_entities(char *in_text)
{
	int i, j;
	char **split_text;
	char *input, *output;

	input = g_strdup (in_text);

	for (i = 0; i < entity_num; i++){
		for (j = 1; j < ENTITY_TYPES; j++){
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

	return output;
}

int
main()
{
	char *test = ">, &gt;, &#62;, &#x3E;, "
                     "\", &quot;, &#34;, &#x22;, "
                     "\', &apos;, &#39;, &#x27;, "
                     "&, &amp;, &#38;, &#x26;, "
                     "<, &lt;, &#60;, &#x3C;";

	g_print ("%s\n", translate_entities (test));
	g_print ("%d\n", entity_num);

	return 0;
}
