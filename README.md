# bbhost-battery-plugin

A plugin for [bbhost](https://github.com/droogie/bbhost) on Linux. It shows a
banner over the game while the battery of the Xbox controller is low.

## What it does

- Every 2 seconds, it reads `/sys/class/power_supply/gip*/capacity_level`.
- If the level is `Low` or `Critical`, it shows `Battery: Low` (or
  `Battery: Critical`) over the game.
- At any other level, it shows nothing.

It uses only the official plugin API (`on_frame` and `show_message`). It does
not change the game and it does not patch the host.

## Requirements

- bbhost for Linux with plugin API version 8 or newer. Tested with
  bbhost-linux-v0.2.18 (API version 11).
- An Xbox controller on the [xone](https://github.com/dlundqvist/xone) driver.
  This driver makes the `gip*` power supply device.
- `gcc` and `make`.

## Build and install

```
make
make install
```

`make install` copies `battery.so` to `~/.config/bbhost/plugins`. For a
different folder, run `make install PLUGIN_DIR=/path/to/plugins`.

Start bbhost again. The log shows `plugin: battery 1.2.0 loaded`.

The plugin is not signed. If `plugins.signed_only` is set in `bbhost.toml`,
bbhost does not load it.

## Limits

- The driver reports a level word (`Full`, `High`, `Normal`, `Low`), not a
  percent.
- The banner position and style come from bbhost. The plugin API has no call
  for positioned text.
- The plugin reads the first `gip*` device only.

## Files

- `battery.c`: the plugin.
- `bbhost_plugin.h`: the bbhost plugin API header, copied from
  bbhost-linux-v0.2.18.

## License

GNU GPL, version 3 or later. See `LICENSE`. `bbhost_plugin.h` is part of
bbhost, which has the same license.
