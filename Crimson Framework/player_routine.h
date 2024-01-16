#ifndef PLAYER_ROUTINE_H
#define PLAYER_ROUTINE_H

// Helper functions
void player_air_restore_angle(child_player* ent);

void player_routine_update(child_player* ent);

void player_routine_none(child_player* ent);
void player_routine_control(child_player* ent);

#endif // !PLAYER_ROUTINE_H
