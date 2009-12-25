#include <string.h>
#include <glib.h>

GHashTable*
session_parse (const char *session)
{
	GHashTable* hashtable = g_hash_table_new(g_str_hash, g_str_equal);
	char *input = g_strdup (session);
	char **split_1, **split_2;
	char **session_vals, *key, *value;
	int last, i, j;

        if (input[0] == '{')
		input[0] = ' ';
	last = strlen(input)-1;
	if (input[last] == '}')
		input[last] = ' ';
	g_strstrip (input);

	split_1 = g_strsplit (input, ",\"", 0);
	for (i = 0; split_1[i] != NULL; i++){
		split_2 = g_strsplit (split_1[i], "\",\"", 0);
		for (j = 0; split_2[j] != NULL; j++){
			session_vals = g_strsplit (split_2[j], ":", 2);
			if (session_vals[0] != NULL && session_vals[1] != NULL){
				g_strdelimit (session_vals[0], "\"", ' ');
				g_strstrip (session_vals[0]);
				key = g_strdup (session_vals[0]);

				g_strdelimit (session_vals[1], "\"", ' ');
				g_strstrip (session_vals[1]);
				value = g_strdup (session_vals[1]);

				/* Insert into the Hashtable */
				g_hash_table_insert (hashtable, key, value);

				/* free the split string */
				g_strfreev (session_vals);
			}
		}
	}
	g_strfreev (split_1);
	g_strfreev (split_2);

	return hashtable;
}

int
main()
{
	char *session = "{\"session_key\":\"123456789-+=!@#$%^&*{}[]|?,.~`\",\"uid\":1234567,\"expires\":0,\"secret\":\"1234abcdefghijk\",\"sig\":\"123456789jkdlfadnskfdsf\"}";
	GHashTable* output_hash;

	output_hash = session_parse (session);

	GHashTableIter iter;
	gpointer key, value;
	char *svalue;

	g_hash_table_iter_init (&iter, output_hash);
	while(g_hash_table_iter_next(&iter, &key, &value)) {
		g_print("key\t: \"%s\"\nvalue\t: \"%s\"\n\n", (char *)key , (char *)value);
	}
	svalue = g_hash_table_lookup (output_hash, "session_key");
	g_print ("session_key\t= %s\n", svalue);

	svalue = g_hash_table_lookup (output_hash, "secret");
	g_print ("secret\t\t= %s\n", svalue);

	svalue = g_hash_table_lookup (output_hash, "uid");
	g_print ("uid\t\t= %s\n", svalue);


	return 0;
}
