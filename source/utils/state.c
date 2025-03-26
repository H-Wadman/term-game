#include <stdbool.h>

struct Player
{
    bool has_visited_glade;
    bool has_visited_cabin;
    bool has_visited_well;
    bool has_key;
};

static struct Player player; //NOLINT

bool player_visited_glade_val() { return player.has_visited_glade; }

void player_visited_glade_set() { player.has_visited_glade = true; }

bool player_visited_cabin_val() { return player.has_visited_cabin; }

void player_visited_cabin_set() { player.has_visited_cabin = true; }

bool player_visited_well_val() { return player.has_visited_well; }

void player_visited_well_set() { player.has_visited_well = true; }

bool player_has_key_val() { return player.has_key; }

void player_has_key_set() { player.has_key = true; }
