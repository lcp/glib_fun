#include <string.h>
#include <glib.h>

/* The parser to extract keys and values from the JSON style session */
GHashTable*
session_parse (const char *session)
{
	GHashTable* hashTable = g_hash_table_new(g_str_hash, g_str_equal);
	char *input = g_strdup (session);
	char **split_input, **session_vals, *key, *value;
	int last, i;

        if (input[0] == '{')
		input[0] = ' ';
	last = strlen(input)-1;
	if (input[last] == '}')
		input[last] = ' ';
	g_strstrip (input);

	/* Split the string by ," */
	split_input = g_strsplit (input, ",\"", 0);
	for (i = 0; split_input[i] != NULL; i++){
		/* Seperate key and value from "key:value" */
		session_vals = g_strsplit (split_input[i], ":", 2);
		if (session_vals[0] != NULL && session_vals[1] != NULL){
			/* Remove " from the head and the tail */
			g_strdelimit (session_vals[0], "\"", ' ');
			g_strstrip (session_vals[0]);
			g_strdelimit (session_vals[1], "\"", ' ');
			g_strstrip (session_vals[1]);
			key = g_strdup (session_vals[0]);
			value = g_strdup (session_vals[1]);

			/* Insert into the Hashtable */
			g_hash_table_insert (hashTable, key, value);

			/* free the strings */
			g_strfreev (session_vals);
		}
	}
	/* free the strings */
	g_strfreev (split_input);

	return hashTable;
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
	g_print ("session_key = %s\n", svalue);

	svalue = g_hash_table_lookup (output_hash, "secret");
	g_print ("secret      = %s\n", svalue);

	svalue = g_hash_table_lookup (output_hash, "uid");
	g_print ("uid         = %s\n", svalue);


	return 0;
}
