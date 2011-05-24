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

int
main ()
{
	char sender[1024];
	pid_t caller_pid;

	g_type_init ();

	g_print ("sender name:");
	scanf ("%s", sender);

	caller_pid = get_caller_info (sender);

	g_print ("Pid: %u\n", caller_pid);

	return 0;
}
