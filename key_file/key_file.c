#include <glib.h>

int
main ()
{
	GKeyFile *key_file;
	gsize length;
	char **key_list;
	int i;

	key_file = g_key_file_new ();

	if (!g_key_file_load_from_file (key_file, "key.conf", G_KEY_FILE_NONE, NULL)) {
		g_key_file_free (key_file);
		return 1;
	}

	key_list = g_key_file_get_keys (key_file, "devices", &length, NULL);

	for (i = 0; i < length; i++) {
		g_printf ("%s\n", key_list[i]);
	}

	g_key_file_free (key_file);
	g_strfreev (key_list);

	return 0;
}
