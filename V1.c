#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

/**** structs and const ****/


const int TEXTBOX_HEIGHT = 3, TEXTBOX_WIDTH = 50;
const int BUTTON_HEIGHT = 3, BUTTON_WIDTH = 15;
const int MINI_BUTTON_HEIGHT = 3, MINI_BUTTON_WIDTH = 10;
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
    refresh();
}


int find_in_string (char* string, char ch) {
    int len = strlen (string);
    for (int i = 0; i < len; i++)
        if (string[i] == ch)
            return i;
    return -1;
}



/**** first operation for structs ****/



/** textbox and button **/


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
    if (is_reverse)
        wattron (box -> button, A_REVERSE);
    if (is_bold)
        wattron (box -> button, A_BOLD);
    mvwprintw (box -> button, box -> start_line . y, box -> start_line . x, box -> lable);
    wrefresh (box -> button);
    if (is_reverse)
        wattroff (box -> button, A_REVERSE);
    if (is_bold)
        wattroff (box -> button, A_BOLD);
}


/** textbox and button **/



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
    if (find_in_string (password, ' ') + 1)
        return 0;

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
    if (find_in_string (email, ' ') + 1 || find_in_string (email, '/') + 1 || find_in_string (email, '*') + 1 || find_in_string (email, '-') + 1)
        return 0;

    int length = strlen (email);

    int index = find_in_string (email, '@');
    for (int i = index + 2; i < length - 1; i++)
        if (email[i] == '.')
            return 1;
    return 0;
}


void add_user (char* username, char* password, char* email) {

    FILE* username_file = fopen ("files/username_list.txt", "a");
    FILE* password_file = fopen ("files/password_list.txt", "a");
    FILE* email_file = fopen ("files/email_list.txt", "a");

    refresh ();
    create_user (user_list + size, size + 1, username, password, email);
    size++;
    int id = size;

    fprintf (username_file, "%d %s\n", id, username);
    fprintf (password_file, "%d %s\n", id, password);
    fprintf (email_file, "%d %s\n", id, email);

    fclose (username_file);
    fclose (password_file);
    fclose (email_file);

}


/**** register helper functions ****/



/**** login helper functions ****/


int check_login_with_password (char* username, char* password) {
    int user_id = 0;

    for (int i = 0; i < size; i++)
        if (strcmp (user_list[i] . username, username) == 0)
            user_id = user_list[i] . id;

    if (user_id)
        if (strcmp (user_list [user_id - 1] . password, password) == 0)
            return user_id;
        else
            return 0;

    return -1;
}


/**** login helper functions ****/



/**** main functions ****/


void read_files () {
    user_list = (user*) malloc(MAX_USER * sizeof(user));

    FILE* username_file = fopen("files/username_list.txt", "r");
    FILE* password_file = fopen("files/password_list.txt", "r");
    FILE* email_file = fopen("files/email_list.txt", "r");

    int i, ch, id;
    char username[MAX_N], password[MAX_N], email[MAX_N];

    for (i = 0; (ch = getc (username_file)) != EOF; i++) {
        ungetc(ch, username_file);

        fscanf(username_file, "%d %s", &id, username);
        fscanf(password_file, "%d %s", &id, password);
        fscanf(email_file, "%d %s", &id, email);

        getc (username_file);
        getc (password_file);
        getc (email_file);

        create_user(user_list + i, id, username, password, email);
    }
    size = i;


    fclose(username_file);
    fclose(password_file);
    fclose(email_file);
}


int first_page () {
    clear ();
    refresh ();
    noecho ();

    button Register, Login;
    int start_x = (COLS - BUTTON_WIDTH) / 2;// - LABLE_LEN;

    int start_y1 = 8;
    create_button (&Register, start_y1, start_x, "Register", 0);

    int start_y2 = 15;
    create_button (&Login, start_y2, start_x, "Log in", 0);


    keypad (stdscr, TRUE);
    int selected = 1;
    while (1) {
        if (selected == 1) {
            create_button (&Register, start_y1, start_x, "Register", 1);
            create_button (&Login, start_y2, start_x, "Log in", 0);
        }
        else {
            create_button (&Register, start_y1, start_x, "Register", 0);
            create_button (&Login, start_y2, start_x, "Log in", 1);
        }
        move (0, 0);

        int ch = getch ();
        switch (ch) {
            case '\n':
                goto tag_send;
            case KEY_UP:
            case KEY_DOWN:
            case '\t':
                selected = 3 - selected;
        }
    }


    tag_send:

    return selected;
}


int register_menu () {
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
    int size, ind;


    tag_username:

    keypad (username . box, TRUE);
    ind = size = strlen (username . content);
    wmove (username . box, username . start_line . y, username . start_line . x + size);
    while (ch = wgetch (username . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
                for (int i = ind; i < size; i++)
                    username . content [i - 1] = username . content [i];
                username . content [--size] = '\0';
                ind--;
                break;
            case KEY_LEFT:
                if (ind > 0)
                    ind--;
                break;
            case KEY_RIGHT:
                if (ind < size)
                    ind++;
                break;
            case KEY_UP:
                goto tag_email;
            case '\n':
            case '\t':
            case KEY_DOWN:
                goto tag_password;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
                for (int i = size - 1; i >= ind; i--)
                    username . content [i + 1] = username . content [i];
                username . content [ind++] = ch;
                size++;
        }
        show_window (username . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (username . box, username . start_line . y, username . start_line . x, "%s", username . content);
        wmove (username . box, username . start_line . y, username . start_line . x + ind);
        wrefresh (username . box);
    }

    tag_password:

    keypad (password . box, TRUE);
    ind = size = strlen (password . content);
    wmove (password . box, password . start_line . y, password . start_line . x + size);
    while (ch = wgetch (password . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
                for (int i = ind; i < size; i++)
                    password . content [i - 1] = password . content [i];
                password . content [--size] = '\0';
                ind--;
                break;
            case KEY_LEFT:
                if (ind > 0)
                    ind--;
                break;
            case KEY_RIGHT:
                if (ind < size)
                    ind++;
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
                for (int i = size - 1; i >= ind; i--)
                    password . content [i + 1] = password . content [i];
                password . content [ind++] = ch;
                size++;
        }
        show_window (password . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (password . box, password . start_line . y, password . start_line . x, "%s", password . content);
        wmove (password . box, password . start_line . y, password . start_line . x + ind);
        wrefresh (password . box);
    }

    tag_email:

    keypad (email . box, TRUE);
    ind = size = strlen (email . content);
    wmove (email . box, email . start_line . y, email . start_line . x + size);
    while (ch = wgetch (email . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
                for (int i = ind; i < size; i++)
                    email . content [i - 1] = email . content [i];
                email . content [--size] = '\0';
                ind--;
                break;
            case KEY_LEFT:
                if (ind > 0)
                    ind--;
                break;
            case KEY_RIGHT:
                if (ind < size)
                    ind++;
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
                for (int i = size - 1; i >= ind; i--)
                    email . content [i + 1] = email . content [i];
                email . content [ind++] = ch;
                size++;
        }
        show_window (email . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (email . box, email . start_line . y, email . start_line . x, "%s", email . content);
        wmove (email . box, email . start_line . y, email . start_line . x + ind);
        wrefresh (email . box);
    }

    tag_send:

    noecho();

    if (!check_username (username . content)) {
        print_massage ("This username is existed");
        getch ();
        return 0;
    }

    else if (!check_password (password . content)) {
        print_massage ("This password is not valid");
        getch ();
        return 0;
    }

    else if (!check_email (email . content)) {
        print_massage ("The E-mail format is not valid");
        getch ();
        return 0;
    }

    else {
        add_user (username . content, password . content, email . content);
        print_massage ("Seccusfully! Press any key to go to first page!");
        getch ();
        return 1;
    }
}


int login_menu () {
    clear ();
    refresh ();

    textbox username, password, email;
    int start_x = (COLS - TEXTBOX_WIDTH) / 2;// - LABLE_LEN;

    int start_y = 7;
    create_textbox (&username, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "username:");

    start_y = 13;
    create_textbox (&password, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "password:");

    refresh ();

    noecho ();
    int ch;
    int size, ind;


    tag_username:

    keypad (username . box, TRUE);
    ind = size = strlen (username . content);
    wmove (username . box, username . start_line . y, username . start_line . x + size);
    while (ch = wgetch (username . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
            for (int i = ind; i < size; i++)
                username . content [i - 1] = username . content [i];
            username . content [--size] = '\0';
            ind--;
            break;
            case KEY_LEFT:
                if (ind > 0)
                    ind--;
            break;
            case KEY_RIGHT:
                if (ind < size)
                    ind++;
            break;
            case KEY_UP:
            case '\n':
            case '\t':
            case KEY_DOWN:
                goto tag_password;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
            for (int i = size - 1; i >= ind; i--)
                username . content [i + 1] = username . content [i];
            username . content [ind++] = ch;
            size++;
        }
        show_window (username . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (username . box, username . start_line . y, username . start_line . x, "%s", username . content);
        wmove (username . box, username . start_line . y, username . start_line . x + ind);
        wrefresh (username . box);
    }

    tag_password:

    keypad (password . box, TRUE);
    ind = size = strlen (password . content);
    wmove (password . box, password . start_line . y, password . start_line . x + size);
    while (ch = wgetch (password . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
            for (int i = ind; i < size; i++)
                password . content [i - 1] = password . content [i];
            password . content [--size] = '\0';
            ind--;
            break;
            case KEY_LEFT:
                if (ind > 0)
                    ind--;
            break;
            case KEY_RIGHT:
                if (ind < size)
                    ind++;
            break;
            case KEY_UP:
            case '\t':
            case KEY_DOWN:
                goto tag_username;
            case '\n':
                goto tag_send;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
            for (int i = size - 1; i >= ind; i--)
                password . content [i + 1] = password . content [i];
            password . content [ind++] = ch;
            size++;
        }
        show_window (password . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (password . box, password . start_line . y, password . start_line . x, "%s", password . content);
        wmove (password . box, password . start_line . y, password . start_line . x + ind);
        wrefresh (password . box);
    }

    tag_send:

    noecho();

    int result = check_login_with_password (username . content, password . content);


    if (result == -1) {
        print_massage ("The username is not existed");
        getch ();
        return 0;
    }

    else if (result == 0) {
        print_massage ("The password is not correct");
        getch ();
        return 0;
    }

    else {
        clear ();
        position start;
        start.y = LINES / 2 - 5;
        start.x = (COLS - strlen ("Hello dear ! Wellcome to Rogou!") - strlen (username . content)) / 2;
        mvprintw (start.y, start.x, "Hello dear %s! Wellcome to Rogou!", username . content);
        refresh();
        getch ();
    }
}


int pregmae_menu () {
    clear ();
    refresh ();
    noecho ();

    button new_game, resume_game, scoreboard;
    button easy, medium, hard;
    button red, blue, black;
    position start;

    start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 5;
    create_button (&new_game, start.y, start.x, "New Game", 0);

    start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 9;
    create_button (&resume_game, start.y, start.x, "Resume Game", 0);

    start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 13;
    create_button (&scoreboard, start.y, start.x, "Scoreboard", 0);


    start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 18;
    create_mini_button (&easy, start.y, start.x, "Easy", 0, 0);

    start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 18;
    create_mini_button (&medium, start.y, start.x, "Medium", 0, 0);

    start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 18;
    create_mini_button (&hard, start.y, start.x, "Hard", 0, 0);


    start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 22;
    create_mini_button (&black, start.y, start.x, "Black", 0, 0);

    start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 22;
    create_mini_button (&red, start.y, start.x, "Red", 0, 0);

    start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 22;
    create_mini_button (&blue, start.y, start.x, "Blue", 0, 0);


    move (0, 0);


    keypad (stdscr, TRUE);
    int level = 1;
    int color = 1;
    int selected = 1;


    while (1){

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 5;
        create_button (&new_game, start.y, start.x, "New Game", 0);

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 9;
        create_button (&resume_game, start.y, start.x, "Resume Game", 0);

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 13;
        create_button (&scoreboard, start.y, start.x, "Scoreboard", 0);


        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 18;
        create_mini_button (&easy, start.y, start.x, "Easy", 0, 0);

        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 18;
        create_mini_button (&medium, start.y, start.x, "Medium", 0, 0);

        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 18;
        create_mini_button (&hard, start.y, start.x, "Hard", 0, 0);


        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 22;
        create_mini_button (&black, start.y, start.x, "Black", 0, 0);

        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 22;
        create_mini_button (&red, start.y, start.x, "Red", 0, 0);

        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 22;
        create_mini_button (&blue, start.y, start.x, "Blue", 0, 0);


        switch (level) {
            case 1:
                start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 18;
                create_mini_button (&easy, start.y, start.x, "Easy", 0, 1);
                break;
            case 2:
                start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 18;
                create_mini_button (&medium, start.y, start.x, "Medium", 0, 1);
                break;
            case 3:
                start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 18;
                create_mini_button (&hard, start.y, start.x, "Hard", 0, 1);
                break;
        }


        switch (color) {
            case 1:
                start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 22;
                create_mini_button (&black, start.y, start.x, "Black", 0, 1);
                break;
            case 2:
                start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 22;
                create_mini_button (&red, start.y, start.x, "Red", 0, 1);
                break;
            case 3:
                start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 22;
                create_mini_button (&blue, start.y, start.x, "Blue", 0, 1);
                break;
        }


        switch (selected) {
            case 1:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 5;
                create_button (&new_game, start.y, start.x, "New Game", 1);
                break;
            case 2:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 9;
                create_button (&resume_game, start.y, start.x, "Resume Game", 1);
                break;
            case 3:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 13;
                create_button (&scoreboard, start.y, start.x, "Scoreboard", 1);
                break;
            case 4:
                switch (level) {
                    case 1:
                        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 18;
                        create_mini_button (&easy, start.y, start.x, "Easy", 1, 1);
                        break;
                    case 2:
                        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 18;
                        create_mini_button (&medium, start.y, start.x, "Medium", 1, 1);
                        break;
                    case 3:
                        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 18;
                        create_mini_button (&hard, start.y, start.x, "Hard", 1, 1);
                        break;
                }
                break;
            case 5:
                switch (color) {
                    case 1:
                        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 22;
                        create_mini_button (&black, start.y, start.x, "Black", 1, 1);
                        break;
                    case 2:
                        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 22;
                        create_mini_button (&red, start.y, start.x, "Red", 1, 1);
                        break;
                    case 3:
                        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 22;
                        create_mini_button (&blue, start.y, start.x, "Blue", 1, 1);
                        break;
                }
                break;
        }

        move (0, 0);


        int ch = getch();

        switch (ch) {
            case KEY_UP:
                selected = (selected == 1) ? 5 : selected - 1;
                break;
            case '\t':
            case KEY_DOWN:
                selected = selected % 5 + 1;
                break;
            case '\n':
                if (selected > 3)
                    selected = selected % 5 + 1;
                else
                    return selected;
                break;
            case KEY_RIGHT:
                if (selected == 4)
                    level = level % 3 + 1;
                else if (selected == 5)
                    color = color % 3 + 1;
                break;
            case KEY_LEFT:
                if (selected == 4)
                    level = (level == 1) ? 3 : level - 1;
                else if (selected == 5)
                    color = (color == 1) ? 3 : color - 1;
                break;
        }
    }

}


/**** main functions ****/


void run_project () {
    initscr();
    cbreak();
    refresh();

    read_files ();


    tag_first:

    if (first_page() == 2)
        goto tag_login;


    tag_register:

    while (! register_menu ());
    goto tag_first;


    tag_login:

    int user_id = 0;
    while (! user_id)
        user_id = login_menu ();


    tag_pregame:

    pregmae_menu ();

    getch();
    clear();
    endwin();
}


/**** main functions ****/



int main(int argc, char *argv[]){
    run_project();
    return 0;
}
