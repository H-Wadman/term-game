#include <stdbool.h>

struct Player
{
    bool has_visited_glade;
    bool has_visited_cabin;
    bool has_visited_well;
    bool has_key;
    bool has_forest_map;
} static player; //NOLINT

struct Settings
{
    bool katte_mode_enabled;
};

static struct Settings settings = {.katte_mode_enabled = false}; //NOLINT

bool is_katte_mode() { return settings.katte_mode_enabled; }

void set_katte_mode(bool enable) { settings.katte_mode_enabled = enable; }

bool player_visited_glade_val() { return player.has_visited_glade; }

void player_visited_glade_set() { player.has_visited_glade = true; }

bool player_visited_cabin_val() { return player.has_visited_cabin; }

void player_visited_cabin_set() { player.has_visited_cabin = true; }

bool player_visited_well_val() { return player.has_visited_well; }

void player_visited_well_set() { player.has_visited_well = true; }

bool player_has_forest_map_val() { return player.has_forest_map; }

void player_has_forest_map_set() { player.has_forest_map = true; }

bool player_has_key_val() { return player.has_key; }

void player_has_key_set() { player.has_key = true; }
