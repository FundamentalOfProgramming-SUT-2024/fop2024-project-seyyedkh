#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

/**** structs and const ****/


const int TEXTBOX_HEIGHT = 3, TEXTBOX_WIDTH = 50;
const int BUTTON_HEIGHT = 3, BUTTON_WIDTH = 10;
const int LABLE_LEN = 12, MAX_N = 50;
const int MAX_USER = 500;


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
    WINDOW* box;
    position start;
    int height;
    int width;
    position start_line;
} button;


typedef struct user {
    int id;
    char* username;
    char* password;
    char* email;
} user;

user* user_list;
int size = 0;

/**** structs and const ****/


/**** basic functions ****/


void print_massage (char* massage) {
    position start;
    start.y = LINES - 3;
    start.x = (COLS - strlen(massage)) / 2;
    mvprintw (start.y, start.x, massage);
}


int find_in_string (char* string, char ch) {
    int len = strlen (string);
    for (int i = 0; i < len; i++)
        if (string[i] == ch)
            return i;
    return len;
}



/**** first operation for structs ****/



/** textbox **/

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

/** textbox **/



/** user **/

void create_user (user* a_user, int id, char* username, char* password, char* email) {
    a_user -> id = id;
    a_user -> username = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> username, username);
    a_user -> password = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> password, password);
    a_user -> email = (char*) malloc (MAX_N * sizeof(char));
    strcpy (a_user -> email, email);
}

/** user **/



/**** first operation for structs ****/


/**** basic functions ****/



/**** register helper functions ****/


int check_username (char* username) {
    for (int i = 0; i < size; i++)
        if (strcmp (username, (user_list + i) -> username) == 0)
            return 0;
    return 1;
}


int check_password (char* password) {
    int length = strlen (password);
    int length_check = (length >= 7);

    int upper_check = 0;
    for (int i = 0; i < length; i++)
        if (password[i] >= 'A' && password[i] <= 'Z')
            upper_check = 1;

    int lower_check = 0;
    for (int i = 0; i < length; i++)
        if (password[i] >= 'a' && password[i] <= 'z')
            lower_check = 1;

    int number_check = 0;
    for (int i = 0; i < length; i++)
        if (password[i] >= '0' && password[i] <= '9')
            number_check = 1;

    return length_check && upper_check && lower_check && number_check;
}


int check_email (char* email) {
    int length = strlen (email);
    //if (find_in_string (email, ' ') || find_in_string (email, '/') || find_in_string (email, '*') || find_in_string (email, '-'))
    //    return 0;
    int index = find_in_string (email, '@');
    for (int i = index + 2; i < length - 1; i++)
        if (email[i] == '.')
            return 1;
    return 0;
}



/**** register helper functions ****/



/**** main functions ****/


void read_files () {

    user_list = (user*) malloc (MAX_USER * sizeof (user));

    FILE* username_file = fopen ("files/username_list.txt", "r");
    FILE* password_file = fopen ("files/password_list.txt", "r");
    FILE* email_file = fopen ("files/email_list.txt", "r");


    int i = 0;
    for (i = 0; getc (username_file) != EOF; i ++) {
        getc (password_file);
        getc (email_file);
        int id;
        char username [MAX_N];
        char password [MAX_N];
        char email [MAX_N];
        fscanf (username_file, "%d %[\n]", id, username);
        fscanf (password_file, "%d %[\n]", id, password);
        fscanf (email_file, "%d %[\n]", id, email);
        create_user (user_list + i, id, username, password, email);
    }
    size = i;


    fclose (username_file);
    fclose (password_file);
    fclose (email_file);
}


void register_menu () {
    clear ();
    refresh ();

    textbox username, password, email;
    int start_x = (COLS - TEXTBOX_WIDTH) / 2;// - LABLE_LEN;

    int start_y = 6;
    create_textbox (&username, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "username:");

    start_y = 11;
    create_textbox (&password, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "password:");


    start_y = 16;
    create_textbox (&email, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "email:");

    refresh ();

    noecho ();
    int ch;
    int size;


    tag_username:

    keypad (username . box, TRUE);
    size = strlen (username . content);
    wmove (username . box, username . start_line . y, username . start_line . x + size);
    while (ch = wgetch (username . box))
        switch (ch) {
            case KEY_BACKSPACE:
                if (size == 0)
                    break;
                username . content [--size] = '\0';
                mvwaddch (username . box, username . start_line . y, username . start_line . x + size, ' ');
                wmove (username . box, username . start_line . y, username . start_line . x + size);
                break;
            case KEY_LEFT:

            case KEY_UP:
                goto tag_email;
            case '\n':
            case '\t':
            case KEY_DOWN:
                goto tag_password;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
                mvwaddch (username . box, username . start_line . y, username . start_line . x + size, ch);
                username . content [size++] = ch;
        }


    tag_password:

    keypad (password . box, TRUE);
    size = strlen (password . content);
    wmove (password . box, password . start_line . y, password . start_line . x + size);
    while (ch = wgetch (password . box))
        switch (ch) {
            case KEY_BACKSPACE:
                if (size == 0)
                    break;
                username . content [--size] = '\0';
                mvwaddch (password . box, password . start_line . y, password . start_line . x + size, ' ');
                wmove (password . box, password . start_line . y, password . start_line . x + size);
                break;
            case KEY_UP:
                goto tag_username;
            case '\n':
            case '\t':
            case KEY_DOWN:
                goto tag_email;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
                mvwaddch (password . box, password . start_line . y, password . start_line . x + size, ch);
                password . content [size++] = ch;
        }


    tag_email:

    keypad (email . box, TRUE);
    size = strlen (email . content);
    wmove (email . box, email . start_line . y, email . start_line . x + size);
    while (ch = wgetch (email . box))
        switch (ch) {
            case KEY_BACKSPACE:
                if (size == 0)
                    break;
                username . content [--size] = '\0';
                mvwaddch (email . box, email . start_line . y, email . start_line . x + size, ' ');
                wmove (email . box, email . start_line . y, email . start_line . x + size);
                break;
            case KEY_UP:
                goto tag_password;
            case '\n':
                goto tag_send;
            case '\t':
            case KEY_DOWN:
                goto tag_username;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
                mvwaddch (email . box, email . start_line . y, email . start_line . x + size, ch);
                email . content [size++] = ch;
        }


    tag_send:

    noecho();

    if (!check_username (username . content)) {
        print_massage ("This username is existed");
        getch ();
        register_menu ();
        return;
    }

    else if (!check_password (password . content)) {
        print_massage ("This password is not valid");
        getch ();
        register_menu ();
        return;
    }

    else if (!check_email (email . content)) {
        print_massage ("The E-mail format is not valid");
        getch ();
        register_menu ();
        return;
    }

    else {

    }
}


/**** main functions ****/


void run_project () {
    initscr();
    cbreak();
    refresh();

    read_files ();

    register_menu ();

    getch();
    clear();
    endwin();
}


/**** main functions ****/



int main(int argc, char *argv[]){
    run_project();
    return 0;
}
