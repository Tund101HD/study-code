/**
* @file types.h
 * @brief Core data structures: Artist, Song, Playlist.
 */

#pragma once

typedef struct Artist Artist;

typedef struct Song {
    const Artist* artist;
    const char* title;
    const char* description;
    struct Song *next;
} Song;

struct Artist {
    char* name;
    Song* songs;
    Artist* next;
};

/**
 * @brief Represents a playlist of songs.
 * A playlist has a name, a flag indicating if it contains songs
 * from a single artist, a pointer to the first song, and its size.
 */
typedef struct Playlist {
    const char* name;
    int is_from_artist;
    Song* songs;
    int size;
} Playlist;
