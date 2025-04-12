#pragma once

#include <stdbool.h>

bool is_katte_mode(void);

bool set_katte_mode(bool enable);

bool player_visited_glade_val(void);

void player_visited_glade_set(void);

bool player_visited_cabin_val(void);

void player_visited_cabin_set(void);

bool player_has_key_val(void);

bool player_has_forest_map_val(void);

void player_has_forest_map_set(void);

void player_has_key_set(void);

bool player_visited_well_val(void);

void player_visited_well_set(void);
