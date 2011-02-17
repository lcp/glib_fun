/*
 * html 4.0 special entities:
 * 	http://htmlhelp.com/reference/html40/entities/special.html
 * List of XML and HTML character entity references
 *      http://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references
 * */

#include <glib.h>
#include <string.h>
#include <stdio.h>

char *
translate_entities(const char *in_text)
{
	/* Entity types: ASCII ENTITY DECIMAL HEX */
	#define ENTITY_TYPES 4
	int i, j;
	char **split_text;
	char *input, *output;
	char *special_entities[][ENTITY_TYPES] = {
		{"\"", "&quot;", "&#34;", "&#x22;"},
		{"\'", "&apos;", "&#39;", "&#x27;"},
		{"<",  "&lt;",   "&#60;", "&#x3C;"},
		{">",  "&gt;",   "&#62;", "&#x3E;"},
		{"&",  "&amp;",  "&#38;", "&#x26;"},
	};
	int entity_num = sizeof (special_entities) / (sizeof (char *) * ENTITY_TYPES);

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

/**
 * sw_unescape_entities
 *
 * Replace the xml entities in the given string in place.
 *
 * Returns: the string with the entities replaced
 */
gchar *
sw_unescape_entities (gchar *string)
{
  gchar *p = string;
  gchar bucket[10];
  size_t length;

  length = strlen (string);

  for (; p[0]; p++)
    {
      if (p[0] == '&')
        {
          gint length_diff;
          gchar *q;;
          gint bucket_i = 0;
          gunichar replacement = 0;
          gint replacement_length;

          /* p stays the same until the end of this block */

          q = p + 1; /* Move onto next character */

          /* Fill the bucket with the characters in the entity reference */
          while (q[0] != ';' && q[0] && bucket_i < 9)
            {
              bucket[bucket_i] = q[0];
              q++;
              bucket_i++;
            }
          bucket[bucket_i]='\0';

          /* http://bit.ly/EJujl */
          if (g_str_equal (bucket, "quot"))
            replacement = 0x0022;
          else if (g_str_equal (bucket, "amp"))
            replacement = 0x0026;
          else if (g_str_equal (bucket, "apos"))
            replacement = 0x0027;
          else if (g_str_equal (bucket, "lt"))
            replacement = 0x003c;
          else if (g_str_equal (bucket, "gt"))
            replacement = 0x003e;
          else if (bucket[0] == '#' && bucket[1] == 'x')
            {
              /* Convert the bucket hex -> gunichar */
              sscanf (&bucket[2], "%x", &replacement);
            }
          else if (bucket[0] == '#')
            {
              /* Convert the bucket decimal -> gunichar */
              sscanf (&bucket[1], "%u", &replacement);
            }
          else
            {
              continue;
            }

          replacement_length = g_unichar_to_utf8 (replacement, p);

          /*
           * The utf8 representation is always fewer bytes than the entity
           * string itself
           */
          length_diff = bucket_i + 2 - replacement_length;
          if (length_diff > 0)
            {
              size_t len; /* # bytes until the end of the remaining string */

              /* This number *excludes* the \0 */
              len = length - (p - string + bucket_i + 2);
              g_memmove (p + replacement_length, p + bucket_i + 2, len + 1);
            }

            p = p + replacement_length - 1;
        }
    }

  if (!g_utf8_validate (string, -1, NULL))
      g_critical ("Invalid utf-8");

  return string;
}

int
main()
{
	char *test = ">, &gt;, &#62;, &#x3E;, "
                     "\", &quot;, &#34;, &#x22;, "
                     "\', &apos;, &#39;, &#x27;, "
                     "&, &amp;, &#38;, &#x26;, "
                     "<, &lt;, &#60;, &#x3C;, "
		     "&amp;lt;";
	char *test2 = "&gt;&quot;&apos;&amp;&lt;";

	g_print ("%s\n", translate_entities (test));
	g_print ("%s\n", translate_entities (test2));
	g_print ("%s\n", sw_unescape_entities (g_strdup (test)));
	g_print ("%s\n", sw_unescape_entities (g_strdup (test2)));

	return 0;
}
