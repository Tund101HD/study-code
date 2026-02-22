#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ========================================================================= */
/* Section I: Dynamic memory allocation                                      */
/* ========================================================================= */

void run_section_I(void)
{
    int *p_n = NULL;
    int n;
    int *array = NULL;
    int i;
    int new_size;

    printf("=== Section I: Dynamic memory allocation ===\n");
    printf("Enter an integer N: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input.\n\n");
        return;
    }

    p_n = (int *)malloc(sizeof(int));
    if (p_n == NULL) {
        printf("malloc failed for N.\n\n");
        return;
    }
    *p_n = n;

    printf("Stored N = %d at address %p\n", *p_n, (void *)p_n);

    array = (int *)calloc((size_t)(*p_n), sizeof(int));
    if (array == NULL) {
        printf("calloc failed for array.\n");
        free(p_n);
        printf("\n");
        return;
    }
    for (i = 0; i < *p_n; i++) {
        array[i] = i * i;
    }

    new_size = (*p_n) * 2;
    int *tmp = (int *)realloc(array, (size_t)new_size * sizeof(int));
    if (tmp == NULL) {
        printf("realloc failed, keeping original array of size %d.\n", *p_n);
        /* array is still valid here */
        new_size = *p_n;
    } else {
        array = tmp;
    }

    for (i = *p_n; i < new_size; i++) {
        array[i] = i * i;
    }

    printf("Array contents (size = %d):\n", new_size);
    for (i = 0; i < new_size; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }
    free(array);
    free(p_n);

    printf("Memory freed. Section I complete.\n\n");
}

/* ========================================================================= */
/* Section II: Linked list vs array insertion                                */
/* ========================================================================= */

typedef struct Node {
    double value;
    struct Node *next;
} Node;

Node *create_node(double value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;

    return node;
}

/* Append node at end of list */
int append_node(Node **head, double value)
{
    Node *new_node;
    Node *current;

    if (head == NULL) {
        return -1;
    }

    new_node = create_node(value);
    if (new_node == NULL) {
        return -1;
    }

    if (*head == NULL) {
        *head = new_node;
    } else {
        current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    return 0;
}

/* Insert node as 3rd node (index 2, 0-based) in linked list */
int insert_node_at_index_2(Node **head, double value)
{
    Node *new_node;
    Node *current;
    int index = 0;

    if (head == NULL) {
        return -1;
    }

    new_node = create_node(value);
    if (new_node == NULL) {
        return -1;
    }

    if (*head == NULL) {
        /* List empty, new node becomes head */
        *head = new_node;
        return 0;
    }

    current = *head;

    while (current != NULL && index < 1) {
        current = current->next;
        index++;
    }

    if (current == NULL) {
        /* List shorter than 2 nodes, append at end */
        free(new_node);
        return append_node(head, value);
    }

    new_node->next = current->next;
    current->next = new_node;

    return 0;
}
void print_list(const Node *head)
{
    int index = 0;

    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    while (head != NULL) {
        printf("Node %d: %.2f\n", index, head->value);
        head = head->next;
        index++;
    }
}

void free_list(Node **head)
{
    Node *current;
    Node *next;

    if (head == NULL) {
        return;
    }

    current = *head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}

void run_section_II(void)
{
    Node *head = NULL;
    int i;
    int array[51];
    int random_value;
    int insert_pos = 3;
    int array_length = 50;

    printf("=== Section II: Linked list vs array insertion ===\n");


    srand((unsigned int)time(NULL));


    for (i = 0; i < 50; i++) {
        double value = (double)(rand() % 100);
        if (append_node(&head, value) != 0) {
            printf("Error: could not append node.\n");
            free_list(&head);
            printf("\n");
            return;
        }
    }


    for (i = 0; i < 50; i++) {
        array[i] = rand() % 100;
    }
    random_value = rand() % 100;

    /* Array insertion at index 3:
       shift elements 3..49 to 4..50 (from end to front) */
    for (i = array_length; i > insert_pos; i--) {
        array[i] = array[i - 1];
    }
    array[insert_pos] = random_value;
    array_length++; /* Now 51 elements used */
    if (insert_node_at_index_2(&head, (double)random_value) != 0) {
        printf("Error: could not insert node at index 2.\n");
        free_list(&head);
        printf("\n");
        return;
    }

    printf("\nArray contents:\n");
    for (i = 0; i < array_length; i++) {
        printf("array[%d] = %d\n", i, array[i]);
    }

    printf("\nLinked list contents:\n");
    print_list(head);

    /*
       Insertion into an array at the beginning or middle is generally more
       time-consuming than insertion into a linked list, because all
       subsequent elements in the array must be shifted by one position,
       leading to O(n) moves. For a linked list, if we already have a pointer
       to the node before the insertion position, insertion itself is O(1).
       (However, finding the position in the list is also O(n) if we have to
       traverse it.)
    */

    /* 10. Free allocated memory */
    free_list(&head);

    printf("\nSection II complete.\n\n");
}

typedef struct Song {
    char *title;
    char *artist;
    struct Song *next;
} Song;

typedef struct {
    Song *head;
} Playlist;

static char *duplicate_string(const char *src)
{
    size_t length;
    char *copy;

    if (src == NULL) {
        return NULL;
    }

    length = strlen(src) + 1;
    copy = (char *)malloc(length * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, src, length);

    return copy;
}

static Song *create_song(const char *title, const char *artist)
{
    Song *song = (Song *)malloc(sizeof(Song));
    if (song == NULL) {
        return NULL;
    }

    song->title = duplicate_string(title);
    song->artist = duplicate_string(artist);
    song->next = NULL;

    if (song->title == NULL || song->artist == NULL) {
        free(song->title);
        free(song->artist);
        free(song);
        return NULL;
    }

    return song;
}

static void free_song(Song *song)
{
    if (song == NULL) {
        return;
    }

    free(song->title);
    free(song->artist);
    free(song);
}

void init_playlist(Playlist *playlist)
{
    if (playlist == NULL) {
        return;
    }

    playlist->head = NULL;
}

int add_song(Playlist *playlist, const char *title, const char *artist)
{
    Song *new_song;
    Song *current;

    if (playlist == NULL || title == NULL || artist == NULL) {
        return -1;
    }

    new_song = create_song(title, artist);
    if (new_song == NULL) {
        return -1;
    }

    if (playlist->head == NULL) {
        playlist->head = new_song;
    } else {
        current = playlist->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_song;
    }

    return 0;
}

void print_playlist(const Playlist *playlist)
{
    const Song *current;

    if (playlist == NULL) {
        return;
    }

    current = playlist->head;

    if (current == NULL) {
        printf("Playlist is empty.\n");
        return;
    }

    while (current != NULL) {
        printf("Title: %s, Artist: %s\n", current->title, current->artist);
        current = current->next;
    }
}

int delete_firstSong(Playlist *playlist)
{
    Song *first;

    if (playlist == NULL || playlist->head == NULL) {
        return -1;
    }

    first = playlist->head;
    playlist->head = first->next;

    free_song(first);

    return 0;
}

void delete_playlist(Playlist *playlist)
{
    Song *current;
    Song *next_song;

    if (playlist == NULL) {
        return;
    }

    current = playlist->head;
    while (current != NULL) {
        next_song = current->next;
        free_song(current);
        current = next_song;
    }

    playlist->head = NULL;
}

int count_songs(const Playlist *playlist)
{
    const Song *current;
    int count = 0;

    if (playlist == NULL) {
        return 0;
    }

    current = playlist->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

void run_section_III(void)
{
    Playlist playlist;
    int result;

    printf("=== Section III: Playlist Manager ===\n\n");

    init_playlist(&playlist);

    result = add_song(&playlist, "Crawling", "Linkin Park");
    if (result != 0) {
        printf("Error: could not add song.\n");
    }
    result = add_song(&playlist, "Layla", "Eric Clapton");
    if (result != 0) {
        printf("Error: could not add song.\n");
    }
    result = add_song(&playlist, "Esperanto", "Max Herre");
    if (result != 0) {
        printf("Error: could not add song.\n");
    }

    printf("Initial playlist:\n");
    print_playlist(&playlist);
    printf("\n");
    printf("Deleting first song...\n");
    if (delete_firstSong(&playlist) != 0) {
        printf("Playlist is empty, nothing to delete.\n");
    }
    printf("\nPlaylist after deleting first song:\n");
    print_playlist(&playlist);
    printf("\n");

    printf("Deleting entire playlist...\n");
    delete_playlist(&playlist);

    printf("Playlist deleted. Number of songs: %d\n\n", count_songs(&playlist));
}

int main(void)
{
    printf("==== Lab 7 ====\n\n");

    run_section_I();
    run_section_II();
    run_section_III();

    printf("Lab 7 complete.\n");

    return 0;
}
