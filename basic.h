#pragma once

#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define TEXTBOX_HEIGHT 3
#define TEXTBOX_WIDTH 50

#define BUTTON_HEIGHT 3
#define BUTTON_WIDTH 15

#define MINI_BUTTON_HEIGHT 3
#define MINI_BUTTON_WIDTH 10

#define LABLE_LEN 12
#define MAX_N 50
#define MAX_USER 500

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define NORMAL 1
#define NIGHT_MARE 2
#define SPELL 3
#define TREASURE 4

#define BLUE COLOR_PAIR(1)
#define RED COLOR_PAIR(2)
#define YELLOW COLOR_PAIR(3)
#define GREEN COLOR_PAIR(4)
#define BLACK COLOR_PAIR(5)

#define FULL_HEALTH(x) 50 * x
#define HUNGRY_LIMIT 25

#define PILLAR 0
#define IN_ROOM 1
#define CORIDOR 2
#define DOOR 3
#define WINDOWS 4
#define STAIR 5
#define SECRET_DOOR 7
#define TRAP 8
#define FOOD 9
#define GOLD 10
#define BLACK_GOLD 11
// Telesm:
#define T_HEALTH 12
#define T_SPEED 13
#define T_DAMAGE 14
// Selah:
#define GUN 14
#define MACE 15
#define DAGGER 16
#define MAGIC_WAND 17
#define NORMAL_ARROW 18
#define SWORD 19

#define SIGN_PILLAR 'O'
#define SIGN_IN_ROOM '.'
#define SIGN_CORIDOR '#'
#define SIGN_DOOR ' '
#define SIGN_WINDOWS '+'
#define SIGN_STAIR '<'
#define SIGN_SECRET_DOOR '?'
#define SIGN_PLAYER '0'
#define SIGN_TRAP '^'
#define SIGN_FOOD '-'
#define SIGN_GOLD '*'
// Selah:
#define SIGN_MACE '1'
#define SIGN_DAGGER '2'
#define SIGN_MAGIC_WAND '3'
#define SIGN_NORMAL_ARROW '4'
#define SIGN_SWORD '5'
// Telesm:
#define SIGN_T_HEALTH 'h'
#define SIGN_T_SPEED 's'
#define SIGN_T_DAMAGE 'd'

#define NORMAL_FOOD 1
#define SUPER_FOOD 2
#define MAGIC_FOOD 3
#define BAD_FOOD 4


#define CONTINUE 1
#define END 2


typedef struct position {
    int x, y;
} position;


typedef struct textbox {
    WINDOW* box;
    position start;
    int height;
    int width;
    position start_line;
    char* content;
    char* lable;
} textbox;


typedef struct button {
    WINDOW* button;
    position start;
    int height;
    int width;
    position start_line;
    char* lable;
} button;


typedef struct user {
    int id;
    char* username;
    char* password;
    int gold;
    int point;
    int exp1;
    int first_time;
    char* email;
} user;


typedef struct food {
    char* name;
    int type;
} food;


typedef struct gun {
    int number;
    int power;
    int range;
    int type;
} gun;


typedef struct telesm {
    int type;
    int remain_day;
} telesm;


typedef struct room {
    position start;
    int height;
    int width;
    int up_door;
    int down_door;
    int left_door;
    int right_door;
    int window_existance;
    int window_direction;
    int window_position;
    int secret_door_existance;
    int secret_door_direction;
    int secret_door_position;
    int secret_door_appear;
    int theme;
    int** matrix;
    WINDOW* window;
} room;


typedef struct floor {
    int mood;
    room* room;
    int** matrix;
    int** appear;
    position first_position;
    position stair_position;
} floor;


typedef struct game {
    int level;
    int color;
    int gold;
    int health;
    int hungry;
    int floor_number;
    int room_number;
    position player;
    char* name;
    floor* floor;
} game;

int is_file_empty (const char *filename);

void make_colors ();

void show_window (WINDOW* win, int height, int width, int vertical_margin, int horizental_margin, int corner, int in_window);

void create_textbox (textbox* box, int start_y, int start_x);

void create_button (button* box, int start_y, int start_x, char* lable, int is_reverse);

void create_mini_button (button* box, int start_y, int start_x, char* lable, int is_reverse, int is_bold);

void create_user (user* a_user, int id, char* username, char* password, char* email, int gold, int point, int exp1, int first_time);


void print_massage (char* massage, int y);

int find_in_string (char* string, char ch);

int max (int num1, int num2);

int min (int num1, int num2);

int manhatan_distance (position pos1, position pos2);

position change_to_pos (int y, int x);

position move_pos (position pos, int direction);

position above_door (room room, int direction);

position above_secret_door (room room, int direction);

int check_position (position pos);

int calc_room_number (position pos);
