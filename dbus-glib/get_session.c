#include <glib.h>
#include <dbus/dbus-glib.h>

static char *
get_current_session (void)
{
	DBusGConnection *connection;
	DBusGProxy *proxy;
	char *session_id;
	GError *error = NULL;
	gboolean ret;

	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
	if (error != NULL) {
		g_warning ("Failed to get bus: %s", error->message);
		g_error_free (error);
		return NULL;
	}

	proxy = dbus_g_proxy_new_for_name (connection,
					   "org.freedesktop.ConsoleKit",
					   "/org/freedesktop/ConsoleKit/Manager",
					   "org.freedesktop.ConsoleKit.Manager");

	ret = dbus_g_proxy_call (proxy, "GetCurrentSession", &error,
				 G_TYPE_INVALID,
				 DBUS_TYPE_G_OBJECT_PATH, &session_id,
				 G_TYPE_INVALID);
	if (!ret) {
		/* DBus might time out, which is okay */
		if (g_error_matches (error, DBUS_GERROR, DBUS_GERROR_NO_REPLY)) {
			g_debug ("DBUS timed out, but recovering");
			goto out;
		}

		/* an actual error */
		g_warning ("Couldn't sent GetCurrentSession: %s", error->message);
		session_id = NULL;
		goto out;
	}

	session_id = g_strdup (session_id);

out:
	if (error != NULL)
		g_error_free (error);
	if (connection != NULL)
		dbus_g_connection_unref (connection);
	if (proxy != NULL)
		g_object_unref (proxy);

	return session_id;
}

static gboolean
get_seats (void)
{
	DBusGConnection *connection;
	DBusGProxy *proxy;
	GType g_type_array;
	GPtrArray *seats = NULL;
	GError *error = NULL;
	const char *object_path;
	gboolean ret;
	int i;

	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
	if (error != NULL) {
		g_warning ("Failed to get bus: %s", error->message);
		g_error_free (error);
		return FALSE;
	}

	proxy = dbus_g_proxy_new_for_name (connection,
					   "org.freedesktop.ConsoleKit",
					   "/org/freedesktop/ConsoleKit/Manager",
					   "org.freedesktop.ConsoleKit.Manager");

	g_type_array = dbus_g_type_get_collection ("GPtrArray", DBUS_TYPE_G_OBJECT_PATH);
	ret = dbus_g_proxy_call (proxy, "GetSeats", &error,
				 G_TYPE_INVALID,
				 g_type_array, &seats,
				 G_TYPE_INVALID);

	if (!ret) {
		g_warning ("GetSeats Failed: %s", error->message);
		g_error_free (error);
		return FALSE;
	}

	if (seats == NULL) {
		g_debug ("No Seat exists");
		return FALSE;
	}

	for (i = 0; i < seats->len; i++) {
		object_path = (const char *) g_ptr_array_index (seats, i);
		g_print ("Seat%d: %s\n", i+1, object_path);
	}

	return TRUE;
}

int
main ()
{
	char *session_id;

	g_type_init ();

	session_id = get_current_session ();
	g_print ("%s\n", session_id);

	get_seats ();

	g_free (session_id);
}
