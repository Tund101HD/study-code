#include "../include/playlist.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Playlist * create_playlist_with_song(const char *name, int is_from_artist, Song *song) {
    Playlist* new_playlist = (Playlist*)malloc(sizeof(Playlist));
    if (new_playlist == NULL) {
        return NULL;
    }

    new_playlist->name = strdup(name);
    new_playlist->is_from_artist = is_from_artist;
    new_playlist->songs = song;
    new_playlist->size = 1;

    return new_playlist;
}

/**
 * @brief Create a new playlist with songs from another playlist already in it.
 *
 * @param name           Name of the new playlist.
 * @param is_from_artist If this is a playlist consisting of only a single artist.
 * @param to_merge       The old playlist to merge into the new one.
 * @return               The new @ref Playlist "playlist" instance.
 *
 * @note This function takes ownership of @p to_merge and frees it.
 * @see Playlist
 * @see merge_playlist
 */
Playlist * create_playlist_with_songs(const char *name, int is_from_artist, Playlist *to_merge) {
    if (to_merge == NULL || to_merge->songs == NULL || to_merge->size == 0) { // only merge if playlist has songs.
        return NULL;
    }
    Playlist *new_playlist = (Playlist*)malloc(sizeof(Playlist));
    if (new_playlist == NULL) {
        return NULL;
    }

    new_playlist->name = strdup(name);
    new_playlist->is_from_artist = is_from_artist;
    new_playlist->songs = to_merge->songs;
    new_playlist->size = to_merge->size;
    free(to_merge->name);
    free(to_merge);

    return new_playlist;
}

Playlist * merge_playlist(const char* new_name, Playlist *merge_into, const Playlist *to_merge) {
    if (merge_into == NULL || to_merge == NULL) {
        return NULL;
    }
    if (to_merge->songs==NULL || to_merge->size<=0) {
        return merge_into;
    }
    Song* current_head = (merge_into->songs)+sizeof(Song)*merge_into->size;


}

Playlist * create_empty_playlist(const char *name, int is_from_artist) {
    Playlist *new_playlist = (Playlist*)malloc(sizeof(Playlist));
    if (new_playlist == NULL) {
        return NULL;
    }

    new_playlist->name = strdup(name);
    new_playlist->is_from_artist = is_from_artist;
    new_playlist->songs = NULL;
    new_playlist->size = 0;

    return new_playlist;
}

Playlist * add_song_to_playlist(Playlist *playlist, const Song *song) {
}

Playlist * remove_song_from_playlist(Playlist *playlist, const Song *song) {
}

Playlist * insert_song_at_position(Playlist *playlist, const Song *song, int position) {
}

Playlist * shuffle_playlist(Playlist *playlist) {
}

int playlist_size(const Playlist *playlist) {
}

void print_playlist(const Playlist *playlist) {
}
