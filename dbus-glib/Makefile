CFLAGS=`pkg-config --cflags --libs glib-2.0 dbus-glib-1`

all: session

session: get_session.c
	gcc $(CFLAGS) get_session.c -o get_session

clean:
	rm -f get_session
