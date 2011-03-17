#include <glib.h>

int
main ()
{
	GKeyFile *key_file;
	gsize length;
	char **key_list;
	int i;
	gboolean enable_input;
	gboolean ret;
	GError *error = NULL;

	key_file = g_key_file_new ();

	if (!g_key_file_load_from_file (key_file, "key.conf", G_KEY_FILE_NONE, NULL)) {
		g_key_file_free (key_file);
		return 1;
	}

	key_list = g_key_file_get_keys (key_file, "input devices", &length, NULL);

	if (key_list == NULL) {
		g_printf ("key list is NULL\n");
		g_strfreev (key_list);
		return 1;
	}

	for (i = 0; key_list[i] != NULL; i++) {
		g_printf ("%s\n", key_list[i]);
	}

	enable_input = g_key_file_get_boolean (key_file, "general", "enable input", NULL);

	if (enable_input)
		g_printf ("enable input\n");
	else
		g_printf ("disable input\n");

	error = NULL;
	ret = g_key_file_get_boolean (key_file, "general", "test1", &error);
	g_printf ("test1 ret = %d\n", ret);
	if (error)
		g_printf ("error: %s\n", error->message);

	error = NULL;
	ret = g_key_file_get_boolean (key_file, "general", "test2", &error);
	g_printf ("test2 ret = %d\n", ret);

	g_key_file_free (key_file);
	g_strfreev (key_list);

	return 0;
}
