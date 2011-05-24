CFLAGS=`pkg-config --cflags --libs glib-2.0 dbus-glib-1`

all: session caller_info

session: get_session.c
	gcc $(CFLAGS) get_session.c -o get_session

caller_info: get_caller_info.c
	gcc $(CFLAGS) get_caller_info.c -o get_caller_info

clean:
	rm -f get_session caller_info
