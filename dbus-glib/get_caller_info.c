#include <stdio.h>
#include <glib.h>
#include <unistd.h>
#include <dbus/dbus-glib.h>

static pid_t
get_caller_info (const char *sender)
{
	gboolean res;
	GError *error;
	DBusGConnection *connection;
	DBusGProxy *bus_proxy;
	pid_t calling_pid;

	res = FALSE;
	bus_proxy = NULL;

	if (sender == NULL) {
		goto out;
	}

	error = NULL;
	connection = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);
	if (connection == NULL) {
		if (error != NULL) {
			g_warning ("error getting session bus: %s", error->message);
			g_error_free (error);
		}
		goto out;
	}

	bus_proxy = dbus_g_proxy_new_for_name (connection,
					       DBUS_SERVICE_DBUS,
					       DBUS_PATH_DBUS,
					       DBUS_INTERFACE_DBUS);

        error = NULL;
	if (! dbus_g_proxy_call (bus_proxy, "GetConnectionUnixProcessID", &error,
				 G_TYPE_STRING, sender,
				 G_TYPE_INVALID,
				 G_TYPE_UINT, &calling_pid,
				 G_TYPE_INVALID)) {
		g_debug ("GetConnectionUnixProcessID() failed: %s", error->message);
		g_error_free (error);
		goto out;
	}

	res = TRUE;
out:
	if (bus_proxy != NULL) {
		g_object_unref (bus_proxy);
	}
	return calling_pid;
}

static char *
get_session (pid_t calling_pid)
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

	ret = dbus_g_proxy_call (proxy, "GetSessionForUnixProcess", &error,
				 G_TYPE_UINT, calling_pid,
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
		g_warning ("Couldn't sent GetSessionForUnixProcess: %s", error->message);
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

int
main ()
{
	char sender[1024];
	pid_t caller_pid;
	char *session_id;

	g_type_init ();

	g_print ("sender name:");
	scanf ("%s", sender);

	caller_pid = get_caller_info (sender);

	g_print ("Pid: %u\n", caller_pid);

	session_id = get_session (caller_pid);

	g_print ("Session: %s\n", session_id);

	g_free (session_id);

	return 0;
}
