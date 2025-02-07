#include "basic.h"


int is_file_empty (const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0)
        return st.st_size == 0;
    else
        return -1;
}


void make_colors () {
    start_color ();
    use_default_colors ();

    init_pair (1, COLOR_BLUE, -1);
    init_pair (2, COLOR_RED, -1);
    init_pair (3, COLOR_YELLOW, -1);
    init_pair (4, COLOR_GREEN, -1);
}


void print_massage (char* massage, int y) {
    position start;
    start.y = LINES - y;
    start.x = (COLS - strlen(massage)) / 2;
    mvprintw (start.y, start.x, massage);
    refresh();
}


int find_in_string (char* string, char ch) {
    int len = strlen (string);
    for (int i = 0; i < len; i++)
        if (string[i] == ch)
            return i;
    return -1;
}


void show_window (WINDOW* win, int height, int width, int vertical_margin, int horizental_margin, int corner, int in_window) {
    wborder (win, vertical_margin, vertical_margin, horizental_margin, horizental_margin, corner, corner, corner, corner);
    for (int i = 1; i + 1 < height; i++)
        for (int j = 1; j + 1 < width; j++)
            mvwaddch (win, i, j, in_window);
    wrefresh (win);
}


void create_textbox (textbox* box, int start_y, int start_x) {
    box -> box = newwin (TEXTBOX_HEIGHT, TEXTBOX_WIDTH, start_y, start_x);
    box -> start . y = start_y;
    box -> start . x = start_x;
    box -> width = TEXTBOX_WIDTH;
    box -> height = TEXTBOX_HEIGHT;
    box -> start_line . y = 1;
    box -> start_line . x = 2;
    box -> content = (char*) malloc (MAX_N * sizeof (char));
    for (int i = 0; i < MAX_N; i++)
        box -> content[i] = '\0';
    show_window (box -> box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
}


void create_button (button* box, int start_y, int start_x, char* lable, int is_reverse) {
    box -> button = newwin (BUTTON_HEIGHT, BUTTON_WIDTH, start_y, start_x);
    box -> start . y = start_y;
    box -> start . x = start_x;
    box -> width = BUTTON_WIDTH;
    box -> height = BUTTON_HEIGHT;
    box -> start_line . y = 1;
    box -> start_line . x = (BUTTON_WIDTH - strlen (lable)) / 2;
    box -> lable = (char*) malloc (LABLE_LEN * sizeof (char));
    strcpy (box -> lable, lable);
    show_window (box -> button, BUTTON_HEIGHT, BUTTON_WIDTH, 0, 0, 0, ' ');
    if (is_reverse)
        wattron (box -> button, A_REVERSE);
    mvwprintw (box -> button, box -> start_line . y, box -> start_line . x, box -> lable);
    wrefresh (box -> button);
    if (is_reverse)
        wattroff (box -> button, A_REVERSE);
}


void create_mini_button (button* box, int start_y, int start_x, char* lable, int is_reverse, int is_bold) {
    box -> button = newwin (MINI_BUTTON_HEIGHT, MINI_BUTTON_WIDTH, start_y, start_x);
    box -> start . y = start_y;
    box -> start . x = start_x;
    box -> width = MINI_BUTTON_WIDTH;
    box -> height = MINI_BUTTON_HEIGHT;
    box -> start_line . y = 1;
    box -> start_line . x = (MINI_BUTTON_WIDTH - strlen (lable)) / 2;
    box -> lable = (char*) malloc (LABLE_LEN * sizeof (char));
    strcpy (box -> lable, lable);
    show_window (box -> button, MINI_BUTTON_HEIGHT, MINI_BUTTON_WIDTH, 0, 0, 0, ' ');
    make_colors();
    if (is_reverse)
        wattron (box -> button, A_REVERSE | A_BOLD);
    else if (is_bold)
        wattron (box -> button, A_BOLD | BLUE);
    mvwprintw (box -> button, box -> start_line . y, box -> start_line . x, box -> lable);
    wrefresh (box -> button);
    if (is_reverse)
        wattroff (box -> button, A_REVERSE | A_BOLD);
    else if (is_bold)
        wattroff (box -> button, A_BOLD | BLUE);
}


void create_user (user* a_user, int id, char* username, char* password, char* email) {
    a_user -> id = id;
    a_user -> username = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> username, username);
    a_user -> password = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> password, password);
    a_user -> email = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> email, email);
    a_user -> gold = 0;
    a_user -> point = 0;
}


int max (int num1, int num2) {
    if (num1 > num2)
        return num1;
    return num2;
}


int min (int num1, int num2) {
    if (num1 < num2)
        return num1;
    return num2;
}


int manhatan_distance (position pos1, position pos2) {
    int vertical_distance = pos1.y - pos2.y;
    if (vertical_distance < 0)
        vertical_distance *= -1;
    int horizental_distance = pos1.x - pos2.x;
    if (horizental_distance < 0)
        horizental_distance *= -1;
    return vertical_distance + horizental_distance;
}


position change_to_pos (int y, int x) {
    position pos;
    pos.y = y;
    pos.x = x;
    return pos;
}


position move_pos (position pos, int direction) {
    if (direction == 0)
        pos.y--;
    else if (direction == 1)
        pos.y++;
    else if (direction == 2)
        pos.x--;
    else
        pos.x++;
    return pos;
}


position above_door (room room, int direction) {
    switch (direction) {
        case UP:
            return change_to_pos (room.start.y-1, room.start.x + room.up_door);
        case DOWN:
            return change_to_pos (room.start.y + room.height, room.start.x + room.down_door);
        case LEFT:
            return change_to_pos (room.start.y + room.left_door, room.start.x-1);
        case RIGHT:
            return change_to_pos (room.start.y + room.right_door, room.start.x + room.width);
    }
}


position above_secret_door (room room, int direction) {
    switch (direction) {
        case UP:
            return change_to_pos (room.start.y-1, room.start.x + room.secret_door_position);
        case DOWN:
            return change_to_pos (room.start.y + room.height, room.start.x + room.secret_door_position);
        case LEFT:
            return change_to_pos (room.start.y + room.secret_door_position, room.start.x-1);
        case RIGHT:
            return change_to_pos (room.start.y + room.secret_door_position, room.start.x + room.width);
    }
}


int check_position (position pos) {
    return (pos.y >= 12 && pos.y <= 21 && pos.x >= 54 && pos.x <= 71);
}
