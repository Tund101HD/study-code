/**
* @file playlist.h
 * @brief Playlist type and related functions.
 */
#pragma once
#include "types.h"

#define MAX_SONGS 100

Playlist* create_playlist_with_song(const char* name, int is_from_artist, Song* song);
Playlist* create_playlist_with_songs(const char* name, int is_from_artist, Playlist* to_merge);
Playlist* merge_playlist(const char* new_name, Playlist* merge_into, const Playlist* to_merge);
Playlist* create_empty_playlist(const char* name, int is_from_artist);
Playlist* add_song_to_playlist(Playlist* playlist, const Song* song);
Playlist* remove_song_from_playlist(Playlist* playlist, const Song* song);
Playlist* insert_song_at_position(Playlist* playlist, const Song* song, int position);
Playlist* shuffle_playlist(Playlist* playlist);

int playlist_size(const Playlist* playlist);
void print_playlist(const Playlist* playlist);
void delete_playlist(Playlist* playlist);
