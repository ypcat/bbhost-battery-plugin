/* Shows a banner while the Xbox controller's battery (xone driver) is low. */
#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bbhost_plugin.h"

#ifndef LEVEL_GLOB
#define LEVEL_GLOB "/sys/class/power_supply/gip*/capacity_level"
#endif
#define REFRESH_SECONDS 2
#define SHOW_SECONDS 3.0f

BB_PLUGIN_EXPORT const BbPluginInfo bb_plugin_info = {
    BB_PLUGIN_API_VERSION, 0, "battery", "Controller Battery", "1.2.0", "claude",
    "Shows a banner while the Xbox controller's battery is low."};

static const BbHostApi* host;
static time_t last_read;

static void frame(void* user) {
    (void)user;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (last_read && now.tv_sec - last_read < REFRESH_SECONDS)
        return;
    last_read = now.tv_sec;

    glob_t found;
    if (glob(LEVEL_GLOB, 0, NULL, &found) != 0)
        return;
    char level[32] = "";
    FILE* f = fopen(found.gl_pathv[0], "r");
    globfree(&found);
    if (!f)
        return;
    if (fgets(level, sizeof level, f))
        level[strcspn(level, "\n")] = 0;
    fclose(f);
    if (strcmp(level, "Low") != 0 && strcmp(level, "Critical") != 0)
        return;

    char text[64];
    snprintf(text, sizeof text, "Battery: %s", level);
    host->show_message(text, SHOW_SECONDS);
}

BB_PLUGIN_EXPORT int bb_plugin_init(const BbHostApi* api) {
    if (api->version < 8) {
        api->log("battery: needs plugin API 8 or newer, host has %u", api->version);
        return 1;
    }
    host = api;
    return 0;
}

BB_PLUGIN_EXPORT int bb_plugin_image(const BbHostApi* api) {
    return api->on_frame(frame, NULL);
}
