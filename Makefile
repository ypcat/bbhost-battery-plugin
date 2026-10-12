CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra
PLUGIN_DIR ?= $(HOME)/.config/bbhost/plugins

battery.so: battery.c bbhost_plugin.h
	$(CC) -shared -fPIC $(CFLAGS) -o $@ battery.c

install: battery.so
	install -D -m 644 battery.so $(PLUGIN_DIR)/battery.so

clean:
	rm -f battery.so

.PHONY: install clean
