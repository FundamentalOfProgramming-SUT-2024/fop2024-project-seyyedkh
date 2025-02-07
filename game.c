#include "basic.h"


user* user_list;
game play;
int size = 0;


/*******************************************   register   *****************************************************/


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


char* random_password_generate () {
    char* password = (char*) malloc (10);
    while (1) {
        for (int i=0; i<10; i++) {
            if (rand () % 4 == 0)
                password[i] = '0' + rand () % 10;
            else if (rand () % 2 == 0)
                password[i] = 'A' + rand () % 26;
            else
                password[i] = 'a' + rand () % 26;
        }
        if (check_password (password))
            break;
    }
    return password;
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
    create_user (user_list + size, size + 1, username, password, email, 0, 0, 0, 0);
    size++;
    int id = size;

    fprintf (username_file, "%d %s\n", id, username);
    fprintf (password_file, "%d %s\n", id, password);
    fprintf (email_file, "%d %s\n", id, email);

    fclose (username_file);
    fclose (password_file);
    fclose (email_file);

    chdir ("users");
    mkdir (username, 0777);
    chdir (username);
    mkdir ("games", 0777);

    FILE* user_file = fopen ("properties.txt", "w");
    fprintf (user_file, "gold: %d\npoint: %d\nexp1: %d\nfirst time: %d\n", 0, 0, 0, 0);
    fflush (user_file);
    fclose (user_file);

    chdir ("../..");
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
            case 27:
                return -1;
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
            case 27:
                return -1;
            case KEY_F(1):
                strcpy (password . content, random_password_generate ());
                ind = size = strlen (password . content);
                break;
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
            case 27:
                return -1;
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
        print_massage ("This username is existed", 3);
        getch ();
        return 0;
    }

    else if (!check_password (password . content)) {
        print_massage ("This password is not valid", 3);
        getch ();
        return 0;
    }

    else if (!check_email (email . content)) {
        print_massage ("The E-mail format is not valid", 3);
        getch ();
        return 0;
    }

    else {
        add_user (username . content, password . content, email . content);
        print_massage ("Seccusfully! Press any key to go to first page!", 3);
        getch ();
        return 1;
    }
}


/*******************************************   register   *****************************************************/


/*******************************************    login     *****************************************************/


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


int check_login_with_email (char* username, char* email) {
    int user_id = 0;

    for (int i = 0; i < size; i++)
        if (strcmp (user_list[i] . username, username) == 0)
            user_id = user_list[i] . id;

    if (user_id)
        if (strcmp (user_list [user_id - 1] . email, email) == 0)
            return user_id;
    else
        return 0;

    return -1;
}


int login_menu () {
    int send = 1;
    tag_1:

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
            case 27:
                return -1;
            case KEY_F(1):
                send = 2;
                goto tag_2;
                break;
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
            case 27:
                return -1;
            case KEY_F(1):
                send = 2;
                goto tag_2;
                break;
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

    tag_2:

    clear ();
    refresh ();

    start_x = (COLS - TEXTBOX_WIDTH) / 2;// - LABLE_LEN;

    start_y = 7;
    create_textbox (&username, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "username:");

    start_y = 13;
    create_textbox (&email, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "email:");

    refresh ();

    noecho ();

    tag_username2:

    keypad (username . box, TRUE);
    ind = size = strlen (username . content);
    wmove (username . box, username . start_line . y, username . start_line . x + size);
    while (ch = wgetch (username . box)) {
        switch (ch) {
            case 27:
                return -1;
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
                goto tag_email;
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

    tag_email:

    keypad (email . box, TRUE);
    ind = size = strlen (email . content);
    wmove (email . box, email . start_line . y, email . start_line . x + size);
    while (ch = wgetch (email . box)) {
        switch (ch) {
            case 27:
                return -1;
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
            case '\t':
            case KEY_DOWN:
                goto tag_username2;
            case '\n':
                goto tag_send;
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

    int result;
    if (send == 1)
        result = check_login_with_password (username . content, password . content);
    else
        result = check_login_with_email (username . content, email . content);


    if (result == -1) {
        print_massage ("The username is not existed", 3);
        getch ();
        return 0;
    }

    else if (result == 0 && send == 1) {
        print_massage ("The password is not correct", 3);
        getch ();
        return 0;
    }

    else if (result == 0) {
        print_massage ("The email is not correct", 3);
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
        return result;
    }
}


void logout (user* player) {
    chdir ("users");
    chdir (player->username);

    FILE* user_file = fopen ("properties.txt", "w");
    fprintf (user_file, "gold: %d\npoint: %d\n", player->gold, player->point);
    fflush (user_file);
    fclose (user_file);

    chdir ("../..");
}


/*******************************************    login     *****************************************************/




/**************************************************************************************************************/


void read_files () {
    user_list = (user*) malloc(MAX_USER * sizeof(user));

    FILE* username_file = fopen("files/username_list.txt", "r");
    FILE* password_file = fopen("files/password_list.txt", "r");
    FILE* email_file = fopen("files/email_list.txt", "r");

    int i, ch, id;
    char username[MAX_N], password[MAX_N], email[MAX_N];
    int gold, point, exp1, exp2, first_time;

    for (i = 0; (ch = getc (username_file)) != EOF; i++) {
        ungetc(ch, username_file);

        fscanf(username_file, "%d %s", &id, username);
        fscanf(password_file, "%d %s", &id, password);
        fscanf(email_file, "%d %s", &id, email);

        chdir ("users");
        chdir (username);

        FILE* user_file = fopen ("properties.txt", "r");
        fscanf (user_file, "gold: %d\npoint: %d\nexp1: %d\nfirst time: %d", &gold, &point, &exp1, &first_time);
        fclose (user_file);

        chdir ("../..");


        getc (username_file);
        getc (password_file);
        getc (email_file);

        create_user(user_list + i, id, username, password, email, gold, point, exp1, first_time);
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


int pregmae_menu () {
    clear ();
    refresh ();
    noecho ();

    button new_game, resume_game, scoreboard;
    position start;

    keypad (stdscr, TRUE);
    int selected = 1;


    while (1) {

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 6;
        create_button (&new_game, start.y, start.x, "New Game", 0);

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 11;
        create_button (&resume_game, start.y, start.x, "Resume Game", 0);

        start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 16;
        create_button (&scoreboard, start.y, start.x, "Scoreboard", 0);


        switch (selected) {
            case 1:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 6;
                create_button (&new_game, start.y, start.x, "New Game", 1);
                break;
            case 2:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 11;
                create_button (&resume_game, start.y, start.x, "Resume Game", 1);
                break;
            case 3:
                start.x = (COLS - BUTTON_WIDTH) / 2, start.y = 16;
                create_button (&scoreboard, start.y, start.x, "Scoreboard", 1);
                break;
        }

        move (0, 0);


        int ch = getch();

        switch (ch) {
            case KEY_UP:
                selected = (selected == 1) ? 3 : selected - 1;
                break;
            case '\t':
            case KEY_DOWN:
                selected = selected % 3 + 1;
                break;
            case '\n':
                return selected;
                break;
            case 27:
                return 4;
        }
    }

}


void setting_menu () {
    clear ();
    refresh ();
    noecho ();

    button easy, medium, hard;
    button red, blue, black;
    position start;

    keypad (stdscr, TRUE);
    play.level = 1;
    play.color = 1;
    int selected = 1;


    while (1) {

        print_massage ("Setting:", 30);

        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 7;
        create_mini_button (&easy, start.y, start.x, "Easy", 0, 0);

        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 7;
        create_mini_button (&medium, start.y, start.x, "Medium", 0, 0);

        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 7;
        create_mini_button (&hard, start.y, start.x, "Hard", 0, 0);


        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 11;
        create_mini_button (&black, start.y, start.x, "Black", 0, 0);

        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 11;
        create_mini_button (&red, start.y, start.x, "Red", 0, 0);

        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 11;
        create_mini_button (&blue, start.y, start.x, "Blue", 0, 0);


        switch (play.level) {
            case 1:
                start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 7;
                create_mini_button (&easy, start.y, start.x, "Easy", 0, 1);
                break;
            case 2:
                start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 7;
                create_mini_button (&medium, start.y, start.x, "Medium", 0, 1);
                break;
            case 3:
                start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 7;
                create_mini_button (&hard, start.y, start.x, "Hard", 0, 1);
                break;
        }


        switch (play.color) {
            case 1:
                start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 11;
                create_mini_button (&black, start.y, start.x, "Blue", 0, 1);
                break;
            case 2:
                start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 11;
                create_mini_button (&red, start.y, start.x, "Red", 0, 1);
                break;
            case 3:
                start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 11;
                create_mini_button (&blue, start.y, start.x, "Yellow", 0, 1);
                break;
        }


        switch (selected) {
            case 1:
                switch (play.level) {
                    case 1:
                        start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 7;
                        create_mini_button (&easy, start.y, start.x, "Easy", 1, 1);
                        break;
                    case 2:
                        start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 7;
                        create_mini_button (&medium, start.y, start.x, "Medium", 1, 1);
                        break;
                    case 3:
                        start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 7;
                        create_mini_button (&hard, start.y, start.x, "Hard", 1, 1);
                        break;
                }
                break;
                    case 2:
                        switch (play.color) {
                            case 1:
                                start.x = (COLS - 3 * MINI_BUTTON_WIDTH - 3) / 2, start.y = 11;
                                create_mini_button (&black, start.y, start.x, "Blue", 1, 1);
                                break;
                            case 2:
                                start.x = (COLS - 1 * MINI_BUTTON_WIDTH - 1) / 2, start.y = 11;
                                create_mini_button (&red, start.y, start.x, "Red", 1, 1);
                                break;
                            case 3:
                                start.x = (COLS + 1 * MINI_BUTTON_WIDTH + 1) / 2, start.y = 11;
                                create_mini_button (&blue, start.y, start.x, "Yellow", 1, 1);
                                break;
                        }
                        break;
        }


        move (0, 0);


        int ch = getch();

        switch (ch) {
            case KEY_UP:
                selected = (selected == 1) ? 2 : selected - 1;
                break;
            case '\t':
            case KEY_DOWN:
                selected = selected % 2 + 1;
                break;
            case '\n':
                if (selected < 2)
                    selected = selected % 2 + 1;
                else
                    return;
                break;
            case KEY_RIGHT:
                if (selected == 1)
                    play.level = play.level % 3 + 1;
                else if (selected == 2)
                    play.color = play.color % 3 + 1;
                break;
            case KEY_LEFT:
                if (selected == 1)
                    play.level = (play.level == 1) ? 3 : play.level - 1;
                else if (selected == 2)
                    play.color = (play.color == 1) ? 3 : play.color - 1;
                break;
        }
    }
}


void scoreboard (user* player) {
    ;
}


/**************************************************************************************************************/


void fprint_mat (FILE* file, int** matrix, int height, int width) {
    for (int i=0; i<height; i++)
        for (int j=0; j<width; j++)
            if (j == width-1)
                fprintf (file, "%d\n", matrix[i][j]);
    else
        fprintf (file, "%d ", matrix[i][j]);
}


void save_room (FILE* file, room* room) {
    fprintf (file, "theme: %d\n", room->theme);

    fprintf (file, "start: %d %d\ndeminitions: %d %d\n", room->start.y, room->start.x, room->height, room->width);
    fprintf (file, "doors(udlr): %d %d %d %d\n", room->up_door, room->down_door, room->left_door, room->right_door);

    if (room->window_existance)
        fprintf (file, "window: %d direction: %d number: %d\n", room->window_existance, room->window_direction, room->window_position);
    else
        fprintf (file, "window: %d\n", room->window_existance);

    if (room->secret_door_existance)
        fprintf (file, "secret door: %d direction: %d number: %d appear: %d\n", room->secret_door_existance, room->secret_door_direction, room->secret_door_position, room->secret_door_appear);
    else
        fprintf (file, "secret door: %d\n", room->secret_door_existance);

    fprint_mat (file, room->matrix, room->height, room->width);
}


void save_floor (floor* floor) {

    FILE* floor_file = fopen ("properties.txt", "w");
    FILE* room_file[7];
    room_file[0] = fopen ("room1.txt", "w");
    room_file[1] = fopen ("room2.txt", "w");
    room_file[2] = fopen ("room3.txt", "w");
    room_file[3] = fopen ("room4.txt", "w");
    room_file[4] = fopen ("room5.txt", "w");
    room_file[5] = fopen ("room6.txt", "w");
    room_file[6] = fopen ("room7.txt", "w");

    for (int i=0; i<6; i++)
        save_room (room_file[i], floor->room+i);
    if (floor->mood != 4)
        save_room (room_file[6], floor->room+6);


    fprintf (floor_file, "mood: %d\n", floor->mood);
    fprintf (floor_file, "first: (%d, %d)\n", floor->first_position.y, floor->first_position.x);
    fprintf (floor_file, "stair: (%d, %d)\n", floor->stair_position.y, floor->stair_position.x);

    clear ();



    refresh ();
    getch ();


    fprint_mat (floor_file, floor->matrix, LINES-1, COLS);
    fprint_mat (floor_file, floor->appear, LINES-1, COLS);

    fflush (floor_file);
    fclose (floor_file);
    for (int i=0; i<6; i++) {
        fflush (room_file[i]);
        fclose (room_file[i]);
    }
}


void save_game (user* player, game play) {
    chdir ("fop2024-project-seyyedkh");
    chdir ("fop2024-project-seyyedkh");
    chdir ("users");
    chdir (player -> username);
    chdir ("games");

    struct stat statbuf;
    if (stat (play.name, &statbuf))
        mkdir (play.name, 0777);
    chdir (play.name);


    mkdir ("floor1", 0777);
    chdir ("floor1");
    save_floor (&play.floor[0]);
    chdir ("..");


    mkdir ("floor2", 0777);
    chdir ("floor2");
    save_floor (&play.floor[1]);
    chdir ("..");


    mkdir ("floor3", 0777);
    chdir ("floor3");
    save_floor (&play.floor[2]);
    chdir ("..");


    mkdir ("floor4", 0777);
    chdir ("floor4");
    save_floor (&play.floor[3]);
    chdir ("..");

    FILE* game_file = fopen ("properties.txt", "w");

    fprintf (game_file, "level: %d\ncolor: %d\n", play.level, play.color);
    fprintf (game_file, "gold: %d\nhealth: %d\nhungry: %d\n", play.gold, play.health, play.hungry);
    fprintf (game_file, "floor number: %d\nroom number: %d\n", play.floor_number, play.room_number);
    fprintf (game_file, "player position: (%d, %d)\n", play.player.y, play.player.x);

    chdir ("../../../..");

    fclose (game_file);
}


void remove_floor () {
    remove ("properties.txt");
    remove ("room1.txt");
    remove ("room2.txt");
    remove ("room3.txt");
    remove ("room4.txt");
    remove ("room5.txt");
    remove ("room6.txt");
    remove ("room7.txt");
}


void remove_game (user* player, game play) {
    chdir ("users");
    chdir (player -> username);
    chdir ("games");
    chdir (play.name);

    chdir ("floor1");
    remove_floor ();
    chdir ("..");

    chdir ("floor2");
    remove_floor ();
    chdir ("..");

    chdir ("floor1");
    remove_floor ();
    chdir ("..");

    chdir ("floor1");
    remove_floor ();
    chdir ("..");

    remove ("properties.txt");
    rmdir (play.name);
}


/**************************************************************************************************************/


void mini_show_cell (floor* floor, position pos) {
    int room_number = calc_room_number (pos);
    room* room = &floor->room[room_number-1];
    floor->appear[pos.y][pos.x] = 1;
}


void show_cell (floor* floor, position pos) {
    int room_number = calc_room_number (pos);
    room* room = &floor->room[room_number-1];
    if (floor->matrix[pos.y][pos.x] && floor->matrix[pos.y][pos.x] != CORIDOR && room->theme != NIGHT_MARE) {
        for (int i=0; i<room->start.y; i++)
            for (int j=0; j<room->start.x; j++)
                floor->appear[i][j] = 1;
        return;
    }
}


void inshow_cell (floor* floor, position pos) {
    int room_number = calc_room_number (pos);
    room* room = &floor->room[room_number-1];
    floor->appear[pos.y][pos.x]--;
}


void make_coridor (int** matrix, position pos1, position pos2) {
    /*matrix[pos1.y][pos1.x] = 2;
    matrix[pos2.y][pos2.x] = 2;
    if (manhatan_distance (pos1, pos2) <= 1)
        return;
    position pos = move_pos (pos1, this_direction);

    if (matrix[pos.y][pos.x] != 1 && matrix[pos.y][pos.x] * x == 0 && manhatan_distance (pos, pos2) < manhatan_distance (pos1, pos2)) {
        if (rand () % 5 || (rand () % 3 && this_direction == prev_direction))
            make_coridor (matrix, pos, pos2, this_direction, this_direction, x);
        else
            make_coridor (matrix, pos1, pos2, prev_direction, (this_direction + 1) % 4, x);
    }
    else if (matrix[pos.y][pos.x] != 1 && matrix[pos.y][pos.x] * x == 0 && rand () % 10 == 0) {
        make_coridor (matrix, pos, pos2, this_direction, this_direction, x);
    }
    else if (matrix[pos.y][pos.x] != 1 && matrix[pos.y][pos.x] * x == 0 && prev_direction == this_direction && rand () % 5) {
        make_coridor (matrix, pos, pos2, this_direction, this_direction, x);
    }
    else {
        make_coridor (matrix, pos1, pos2, prev_direction, (this_direction + 1) % 4, x);
    }

    if (matrix[pos.y][pos.x] == 1 || (x == 0 && matrix[pos.y][pos.x] == 2))
        make_coridor (matrix, pos1, pos2, prev_direction, (this_direction + 1) % 4, x);
    else if (matrix[pos.y][pos.x] != 1 && x == 0 && manhatan_distance (pos, pos2) < manhatan_distance (pos1, pos2))
        make_coridor (matrix, pos, pos2, this_direction, this_direction, x);
    else if (matrix[pos.y][pos.x] == 0)
        make_coridor (matrix, pos, pos2, this_direction, this_direction, x);
    else
        make_coridor (matrix, pos1, pos2, prev_direction, (this_direction + 1) % 4, x);
//    */
    int direction;
    int j=0;
    matrix[pos2.y][pos2.x] = 2;
    while (manhatan_distance(pos1, pos2)) {
        mvprintw (33, 0, "%d %d %d %d %d", pos1.y, pos1.x, pos2.y, pos2.x, j++);
        refresh ();
        matrix[pos1.y][pos1.x] = 2;
        mvaddch (pos1.y, pos1.x, '2');
        if (rand () % 4 == 0)
            direction = rand () % 4;
        int is = 0;
        for (int i = direction, j = 0; 1; i = (i+1) % 4, j++) {
            position pos = move_pos (pos1, i);
            mvprintw (33, 0, "%d %d %d %d %d %d", pos.y, pos.x, pos2.y, pos2.x, i, j++);
//            getch ();
            refresh ();
            if (j >= 8 && matrix[pos.y][pos.x] == 2);
            else if (j >= 4 && matrix[pos.y][pos.x] == 0);
            else if (matrix[pos.y][pos.x] || matrix[pos.y+1][pos.x] == 1 || matrix[pos.y-1][pos.x] == 1 || matrix[pos.y][pos.x+1] == 1 || matrix[pos.y][pos.x-1] == 1)
                continue;
            if (manhatan_distance (pos, pos2) < manhatan_distance (pos1, pos2)) {
                direction = i;
                pos1 = pos;
                is = 1;
                break;
            }
        }
    }
    // 2,25 <-> 11,31
}
/*
make_coridor (floor->matrix, above_door (floor->room[0], DOWN), above_door(floor->room[3], UP), DOWN, DOWN);
make_coridor (floor->matrix, above_door (floor->room[0], RIGHT), above_door(floor->room[4], UP), LEFT, LEFT);
make_coridor (floor->matrix, above_door (floor->room[1], UP), above_door(floor->room[2], LEFT), UP, UP);
make_coridor (floor->matrix, above_door (floor->room[1], DOWN), above_door(floor->room[5], LEFT), DOWN, DOWN);
make_coridor (floor->matrix, above_door (floor->room[2], RIGHT), above_door(floor->room[5], RIGHT), RIGHT, RIGHT);
make_coridor (floor->matrix, above_door (floor->room[3], RIGHT), above_door(floor->room[4], LEFT), RIGHT, RIGHT);
make_coridor (floor->matrix, above_door (floor->room[4], RIGHT), above_door(floor->room[5], DOWN), RIGHT, RIGHT);
*/

void make_room (floor* floor, room* room, position pos, int number, int theme) {
    room->theme = theme;

    first:
    if (number == 7) {
        room->start.y = rand() % 3 + pos.y;
        room->start.x = rand() % 10 + pos.x;
        room->height = rand() % max (1, (2 + pos.y - room->start.y)) + 5;
        room->width = rand() % max (1, 9 + pos.x - room->start.x) + 5;
    }
    else {
        room->start.y = rand() % 10 + pos.y;
        room->start.x = rand() % 20 + pos.x;
        room->height = rand() % max (1, 9 + pos.y - room->start.y) + 5;
        room->width = rand() % max (1, 19 + pos.x - room->start.x) + 10;
    }

    if (number != 7)
        if (check_position (room->start))
            goto first;
        else if (check_position (change_to_pos (room->start.y+room->height, room->start.x)))
            goto first;
        else if (check_position (change_to_pos (room->start.y, room->start.x+room->width)))
            goto first;
        else if (check_position (change_to_pos (room->start.y+room->height, room->start.x+room->width)))
            goto first;

    room->up_door = 0, room->down_door = 0, room->left_door = 0, room->right_door = 0;


    room->matrix = (int**) malloc (room->height * sizeof (int*));
    for (int i=0; i<room->height; i++) {
        room->matrix[i] = (int*) malloc (room->width * sizeof (int));
        for (int j=0; j<room->width; j++)
            if (i > 1 && j > 1 && i < room->height-2 && j < room->width-2){
                int num = rand () % 200;
                if (num == 0)
                    room->matrix[i][j] = 1;
                else if (num + 7 <= 13)
                    room->matrix[i][j] = num + 7;
                else
                    room->matrix[i][j] = 0;
            }
            else if (i == 0 || j == 0 || i == room->height-1 || j == room->width-1){
                room->matrix[i][j] = 1;
            }
            else {
                room->matrix[i][j] = 0;
            }
    }


    switch (number) {
        case 1:
            if (floor->mood == 1 || floor->mood == 3)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 2 || floor->mood == 1 || floor->mood == 4)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 3)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 2 || floor->mood == 4)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 2:
            if (floor->mood == 2 || floor->mood == 4)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 1 || floor->mood == 3 || floor->mood == 4)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 1 || floor->mood == 2 || floor->mood == 3)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 3)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 3:
            if (floor->mood == 2)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 3)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 4)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 1 || floor->mood == 4)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 4:
            if (floor->mood == 1 || floor->mood == 2 || floor->mood == 4)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 2 || floor->mood == 3)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 3)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 1 || floor->mood == 3 || floor->mood == 4)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 5:
            if (floor->mood == 1 || floor->mood == 2 || floor->mood == 3 || floor->mood == 4)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 1 || floor->mood == 2)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 2 || floor->mood == 4)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 1 || floor->mood == 4)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 6:
            if (floor->mood == 3)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 2 || floor->mood == 3 || floor->mood == 4)
                room->down_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 1 || floor->mood == 4)
                room->left_door = rand () % (room->height - 2) + 1;
            if (floor->mood == 1 || floor->mood == 4)
                room->right_door = rand () % (room->height - 2) + 1;
            break;
        case 7:
            if (floor->mood == 3)
                room->up_door = rand () % (room->width - 2) + 1;
            if (floor->mood == 1 || floor->mood == 2)
                room->right_door = rand () % (room->height - 2) + 1;
    }

    if (room->up_door)
        room->matrix[0][room->up_door] = DOOR;
    if (room->down_door)
        room->matrix[room->height-1][room->down_door] = DOOR;
    if (room->left_door)
        room->matrix[room->left_door][0] = DOOR;
    if (room->right_door)
        room->matrix[room->right_door][room->width-1] = DOOR;

    if (rand() % 5 == 0) {
        room->window_existance = 1;
        room->window_direction = rand () % 4;
        room->window_position = rand () % 4 + 1;
        if (room->window_direction == UP)
            room->matrix[0][room->window_position] = WINDOWS;
        if (room->window_direction == DOWN)
            room->matrix[room->height-1][room->window_position] = WINDOWS;
        if (room->window_direction == LEFT)
            room->matrix[room->window_position][0] = WINDOWS;
        if (room->window_direction == RIGHT)
            room->matrix[room->window_position][room->width-1] = WINDOWS;
    }
    else {
        room->window_existance = 0;
    }


    if (floor->mood == 1 && number == 3) {
        room->secret_door_existance = 1;
        room->secret_door_appear = 0;
        room->secret_door_direction = DOWN;
        room->secret_door_position = rand () % (room->width) + 1;
        room->matrix[room->height-1][room->secret_door_position] = SECRET_DOOR;
    }
    else if (floor->mood == 2 && number == 6) {
        room->secret_door_existance = 1;
        room->secret_door_appear = 0;
        room->secret_door_direction = UP;
        room->secret_door_position = rand () % (room->width) + 1;
        room->matrix[0][room->secret_door_position] = SECRET_DOOR;
    }
    else if (floor->mood == 3 && number == 3) {
        room->secret_door_existance = 1;
        room->secret_door_appear = 0;
        room->secret_door_direction = LEFT;
        room->secret_door_position = rand () % (room->height) + 1;
        room->matrix[room->secret_door_position][0] = SECRET_DOOR;
    }
    else {
        room->secret_door_existance = 0;
    }

    //clear ();

    for (int i=0; i<room->height; i++) {
        for (int j=0; j<room->width; j++) {
            refresh ();
            floor->matrix[i+room->start.y][j+room->start.x] = room->matrix[i][j];
        }
    }

    room->window = newwin (room->height, room->width, room->start.y, room->start.x);
}


void reverse_mat (floor* floor, room* room) {
    for (int i=0; i<room->height; i++)
        for (int j=0; j<room->width; j++)
            floor->matrix[room->start.y+i][room->start.x+j] = room->matrix[i][j] = (room->matrix[i][j] < 2) ? 1 - room->matrix[i][j] : room->matrix[i][j];

    if (room->up_door)
        floor->matrix[room->start.y][room->start.x+room->up_door] = room->matrix[0][room->up_door] = DOOR;
    if (room->down_door)
        floor->matrix[room->start.y+room->height-1][room->start.x+room->down_door] = room->matrix[room->height-1][room->down_door] = DOOR;
    if (room->left_door)
        floor->matrix[room->start.y+room->left_door][room->start.x] = room->matrix[room->left_door][0] = DOOR;
    if (room->right_door)
        floor->matrix[room->start.y+room->right_door][room->start.x+room->width-1] = room->matrix[room->right_door][room->width-1] = DOOR;

    if (room->secret_door_existance)
        switch (room->secret_door_direction) {
            case UP:
                floor->matrix[room->start.y][room->start.x+room->secret_door_position] = SECRET_DOOR;
                break;
            case DOWN:
                floor->matrix[room->start.y+room->height-1][room->start.x+room->secret_door_position] = SECRET_DOOR;
                break;
            case LEFT:
                floor->matrix[room->start.y+room->secret_door_position][room->start.x] = SECRET_DOOR;
                break;
            case RIGHT:
                floor->matrix[room->start.y+room->secret_door_position][room->start.x+room->width-1] = SECRET_DOOR;
                break;
        }

        if (room->window_existance)
            switch (room->window_direction) {
                case UP:
                    floor->matrix[room->start.y][room->start.x+room->window_position] = WINDOWS;
                    break;
                case DOWN:
                    floor->matrix[room->start.y+room->height-1][room->start.x+room->window_position] = WINDOWS;
                    break;
                case LEFT:
                    floor->matrix[room->start.y+room->window_position][room->start.x] = WINDOWS;
                    break;
                case RIGHT:
                    floor->matrix[room->start.y+room->window_position][room->start.x+room->width-1] = WINDOWS;
                    break;
            }
}


void make_floor (floor* floor, int number) {
    floor->mood = 2;//rand() % 4 + 1;

    floor->matrix = (int**) malloc ((LINES-1) * sizeof (int*));
    for (int i=0; i<LINES-1; i++) {
        floor->matrix[i] = (int*) malloc (COLS * sizeof (int));
        for (int j=0; j<COLS; j++)
            if (i && j && i+2-LINES && j+1-COLS)
                floor->matrix[i][j] = 0;
        else
            floor->matrix[i][j] = 1;
    }

    floor->appear = (int**) malloc ((LINES-1) * sizeof (int*));
    for (int i=0; i<LINES-1; i++) {
        floor->appear[i] = (int*) malloc (COLS * sizeof (int));
        for (int j=0; j<COLS; j++)
            floor->appear[i][j] = 0;
    }

    floor->room = (room*) malloc (7 * sizeof (room));
    for (int i=0; i<6; i++) {
        int theme;
        if (rand () % 20 < 13)
            theme = NORMAL;
        else
            theme = NIGHT_MARE;
        make_room (floor, floor->room+i, change_to_pos (15*(i/3)+2, 30*(i%3)+2), i+1, theme);
    }
    if (floor->mood != 4)
        make_room (floor, floor->room+6, change_to_pos (13, 55), 7, SPELL);


    switch (floor->mood) {
        case 1:
            make_coridor (floor->matrix, above_door (floor->room[0], UP), change_to_pos (floor->room[1].start.y + floor->room[1].left_door, 31));
            make_coridor (floor->matrix, above_door(floor->room[1], LEFT), change_to_pos (floor->room[1].start.y + floor->room[1].left_door, 31));
            make_coridor (floor->matrix, above_door (floor->room[0], DOWN), above_door(floor->room[3], UP));
            make_coridor (floor->matrix, above_door (floor->room[1], DOWN), above_door(floor->room[4], UP));
            make_coridor (floor->matrix, above_door (floor->room[2], RIGHT), change_to_pos (16, 92));
            make_coridor (floor->matrix, above_door(floor->room[5], RIGHT), change_to_pos (16, 92));
            make_coridor (floor->matrix, above_door (floor->room[3], RIGHT), change_to_pos (floor->room[3].start.y + floor->room[3].right_door, 31));
            make_coridor (floor->matrix, above_door(floor->room[4], DOWN), change_to_pos (floor->room[3].start.y + floor->room[3].right_door, 31));
            make_coridor (floor->matrix, above_door (floor->room[4], RIGHT), above_door(floor->room[5], LEFT));
            make_coridor (floor->matrix, above_door (floor->room[6], RIGHT), above_secret_door(floor->room[2], DOWN));
        break;

        case 2:
            position pos;
            pos.y = max (floor->room[0].start.y + floor->room[0].right_door, floor->room[1].start.y + floor->room[1].left_door);
            pos.x = 31;

            make_coridor (floor->matrix, above_door(floor->room[0], RIGHT), pos);
            make_coridor (floor->matrix, pos, above_door(floor->room[1], LEFT));
            make_coridor (floor->matrix, pos, above_door(floor->room[4], UP));
            make_coridor (floor->matrix, above_door (floor->room[1], UP), above_door(floor->room[2], UP));
            make_coridor (floor->matrix, above_door (floor->room[0], DOWN), above_door(floor->room[3], UP));
            make_coridor (floor->matrix, above_door (floor->room[3], DOWN), change_to_pos(30,31));
            make_coridor (floor->matrix, above_door(floor->room[4], LEFT), change_to_pos(30,31));
            make_coridor (floor->matrix, above_door (floor->room[4], DOWN), above_door(floor->room[5], DOWN));
            make_coridor (floor->matrix, above_door (floor->room[6], RIGHT), above_secret_door(floor->room[5], UP));
            break;

        case 3:
            make_coridor (floor->matrix, above_door (floor->room[0], UP), change_to_pos (floor->room[1].start.y + floor->room[1].left_door, 31));
            make_coridor (floor->matrix, above_door(floor->room[1], LEFT), change_to_pos (floor->room[1].start.y + floor->room[1].left_door, 31));
            make_coridor (floor->matrix, above_door (floor->room[0], LEFT), change_to_pos (16, 2));
            make_coridor (floor->matrix, above_door(floor->room[3], LEFT), change_to_pos (16, 2));
            make_coridor (floor->matrix, above_door (floor->room[1], DOWN), change_to_pos (16, 31));
            make_coridor (floor->matrix, above_door(floor->room[3], RIGHT), change_to_pos (16, 31));
            make_coridor (floor->matrix, above_door (floor->room[1], RIGHT), change_to_pos (16, min (50, floor->room[4].start.x + floor->room[4].up_door)));
            make_coridor (floor->matrix, above_door(floor->room[4], UP), change_to_pos (16, min (50, floor->room[4].start.x + floor->room[4].up_door)));
            make_coridor (floor->matrix, above_door (floor->room[2], DOWN), change_to_pos (16, max (72, floor->room[5].start.x + floor->room[5].up_door)));
            make_coridor (floor->matrix, above_door(floor->room[5], UP), change_to_pos (16, max (72, floor->room[5].start.x + floor->room[5].up_door)));
            make_coridor (floor->matrix, above_door (floor->room[3], DOWN), change_to_pos (31, 31));
            make_coridor (floor->matrix, above_door(floor->room[5], DOWN), change_to_pos (31, 31));
            make_coridor (floor->matrix, above_door (floor->room[6], UP), above_secret_door(floor->room[2], LEFT));
            break;

        case 4:
            make_coridor (floor->matrix, above_door (floor->room[0], DOWN), change_to_pos(16, floor->room[0].start.x + floor->room[0].down_door));
            make_coridor (floor->matrix, above_door (floor->room[0], RIGHT), change_to_pos(16, 31));
            make_coridor (floor->matrix, above_door (floor->room[1], UP), change_to_pos(floor->room[2].start.y + floor->room[2].left_door, 61));
            make_coridor (floor->matrix, above_door (floor->room[1], DOWN), change_to_pos(16, 62));
            make_coridor (floor->matrix, above_door (floor->room[2], RIGHT), change_to_pos(16, 92));
            make_coridor (floor->matrix, above_door (floor->room[3], RIGHT), change_to_pos(floor->room[3].start.y + floor->room[3].right_door, 31));
            make_coridor (floor->matrix, above_door (floor->room[4], RIGHT), change_to_pos(31, 62));
            make_coridor (floor->matrix, above_door(floor->room[3], UP), change_to_pos(16, floor->room[0].start.x + floor->room[0].down_door));
            make_coridor (floor->matrix, above_door(floor->room[4], UP), change_to_pos(16, 31));
            make_coridor (floor->matrix, above_door(floor->room[2], LEFT), change_to_pos(floor->room[2].start.y + floor->room[2].left_door, 61));
            make_coridor (floor->matrix, above_door(floor->room[5], LEFT), change_to_pos(16, 62));
            make_coridor (floor->matrix, above_door(floor->room[5], RIGHT), change_to_pos(16, 93));
            make_coridor (floor->matrix, above_door(floor->room[4], LEFT), change_to_pos(floor->room[3].start.y + floor->room[3].right_door, 31));
            make_coridor (floor->matrix, above_door(floor->room[5], DOWN), change_to_pos(31, 62));
            break;
    }



    for (int i=0; i<6; i++)
        reverse_mat (floor, floor->room+i);
    if (floor->mood != 4)
        reverse_mat (floor, floor->room+6);

/*
    clear ();
    for (int i=0; i<LINES-1; i++)
        for (int j=0; j<COLS; j++)
            if (floor->matrix[i][j] < 10)// && floor->appear[i][j])
                mvaddch (i, j, '0'+floor->matrix[i][j]);
    else
        mvaddch (i, j, '-');
    refresh ();
    getch ();
*/
    for (int i=0; i<LINES-1; i++)
        for (int j=0; j<COLS; j++)
            if (!(i && j && i+2-LINES && j+1-COLS))
                floor->matrix[i][j] = 0;



    while (1) {
        floor->first_position.x = rand() % (COLS - 1) + 1;
        floor->first_position.y = rand() % (LINES - 2) + 1;
        if (floor->matrix[floor->first_position.y][floor->first_position.x] == IN_ROOM)
            break;
    }
    while (1) {
        floor->stair_position.x = rand() % (COLS - 1) + 1;
        floor->stair_position.y = rand() % (LINES - 2) + 1;
        if (floor->matrix[floor->stair_position.y][floor->stair_position.x] == IN_ROOM) {
            floor->matrix[floor->stair_position.y][floor->stair_position.x] = STAIR;
            break;
        }
    }

}


void make_game (user* player) {
    tag_game_name:

    clear ();
    refresh ();
    noecho ();

    textbox game_name;
    int start_y = (LINES - TEXTBOX_HEIGHT) / 2 - 3, start_x = (COLS - TEXTBOX_WIDTH) / 2;
    create_textbox (&game_name, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "game name:");
    refresh();

    keypad (game_name . box, TRUE);
    int ind = size = strlen (game_name . content), ch;
    wmove (game_name . box, game_name . start_line . y, game_name . start_line . x + size);
    while (ch = wgetch (game_name . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
                for (int i = ind; i < size; i++)
                    game_name . content [i - 1] = game_name . content [i];
                game_name . content [--size] = '\0';
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
            case '\n':
                goto tag_make_game;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
                for (int i = size; i >= ind; i--)
                    game_name . content [i + 1] = game_name . content [i];
                game_name . content [ind++] = ch;
                size++;
        }
        show_window (game_name . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (game_name . box, game_name . start_line . y, game_name . start_line . x, "%s", game_name . content);
        wmove (game_name . box, game_name . start_line . y, game_name . start_line . x + ind);
        wrefresh (game_name . box);
    }

    tag_make_game:

    play.name = (char*) malloc (MAX_N);
    strcpy (play.name, game_name.content);
    chdir ("users");
    chdir (player -> username);
    chdir ("games");
    struct stat statbuf;
    if (stat (play.name, &statbuf) == 0) {
        print_massage ("This game name already exist", 3);
        getch ();
        chdir ("../../..");
        goto tag_game_name;
    }
//    mkdir (play.name, 0777);
    chdir ("../../..");
    play.floor = (floor*) malloc (4 * sizeof (floor));

    for (int i=0; i<4; i++)
        make_floor (&play.floor[i], i+1);

    play.floor_number = 1;
    play.player.x = play.floor[0].first_position.x;
    play.player.y = play.floor[0].first_position.y;
    play.room_number = calc_room_number (play.player);

    play.gold = 0;
    play.health = FULL_HEALTH (play.level);
    play.hungry = 0;
}


/**************************************************************************************************************/


void read_room (FILE* file, room* room) {
    fscanf (file, "theme: %d", &room->theme);
    fscanf (file, "start:%d %d\ndeminitions:%d %d", &room->start.y, &room->start.x, &room->height, &room->width);
    fscanf (file, "doors(udlr): %d %d %d %d\n", &room->up_door, &room->down_door, &room->left_door, &room->right_door);
    fscanf (file, "window: %d", &room->window_existance);
    if (room->window_existance)
        fscanf (file, "direction: %d number: %d", &room->window_direction, &room->window_position);
    fscanf (file, "secret door: %d", &room->secret_door_existance);
    if (room->secret_door_existance)
        fscanf (file, "direction: %d number: %d appear: %d", &room->secret_door_direction, &room->secret_door_position);
    for (int i=0; i<room->height; i++)
        for (int j=0; j<room->width; j++)
            fscanf (file, "%d", &room->matrix[i][j]);
    room->window = newwin (room->height, room->width, room->start.y, room->start.x);
    printw ("1");
    refresh ();
}


void read_floor (floor* floor) {

    FILE* floor_file = fopen ("properties.txt", "r");
    FILE* room_file[7];
    room_file[0] = fopen ("room1.txt", "r");
    room_file[1] = fopen ("room2.txt", "r");
    room_file[2] = fopen ("room3.txt", "r");
    room_file[3] = fopen ("room4.txt", "r");
    room_file[4] = fopen ("room5.txt", "r");
    room_file[5] = fopen ("room6.txt", "r");
    room_file[6] = fopen ("room7.txt", "r");

    printw ("0");
    floor->room = (room*) malloc (7 * sizeof (room));

    for (int i=0; i<7; i++)
        read_room (room_file[i], floor->room+i);
    if (is_file_empty ("room7.txt") == 1)
        read_room (room_file[6], floor->room+6);


    fscanf (floor_file, "mood: %d\n", &floor->mood);
    fscanf (floor_file, "first: (%d, %d)\n", &floor->first_position.y, &floor->first_position.x);
    fscanf (floor_file, "stair: (%d, %d)\n", &floor->stair_position.y, &floor->stair_position.x);

    printw ("2");
    refresh ();
    floor->matrix = (int**) malloc ((LINES-1) * sizeof (int*));
    for (int i=0; i<LINES-1; i++) {
        floor->matrix[i] = (int*) malloc (COLS * sizeof (int));
        for (int j=0; j<COLS; j++)
            fscanf (floor_file, "%d", &floor->matrix[i][j]);
    }

    printw ("3");
    refresh ();
    floor->appear = (int**) malloc ((LINES-1) * sizeof (int*));
    for (int i=0; i<LINES-1; i++) {
        floor->appear[i] = (int*) malloc (COLS * sizeof (int));
        for (int j=0; j<COLS; j++)
            fscanf (floor_file, "%d", &floor->appear[i][j]);
    }

    printw ("4");
    refresh ();
    fclose (floor_file);
    for (int i=0; i<6; i++)
        fclose (room_file[i]);
}


void read_game (user* player) {
    tag_game_name:

    clear ();
    refresh ();
    noecho ();

    textbox game_name;
    int start_y = (LINES - TEXTBOX_HEIGHT) / 2 - 3, start_x = (COLS - TEXTBOX_WIDTH) / 2;
    create_textbox (&game_name, start_y, start_x);
    mvprintw (start_y + 1, start_x - LABLE_LEN, "game name:");
    refresh();

    keypad (game_name . box, TRUE);
    int ind = size = strlen (game_name . content), ch;
    wmove (game_name . box, game_name . start_line . y, game_name . start_line . x + size);
    while (ch = wgetch (game_name . box)) {
        switch (ch) {
            case KEY_BACKSPACE:
                if (ind == 0)
                    break;
            for (int i = ind; i < size; i++)
                game_name . content [i - 1] = game_name . content [i];
            game_name . content [--size] = '\0';
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
            case '\n':
                goto tag_open_game;
            default:
                if (size > TEXTBOX_WIDTH - 4)
                    break;
            for (int i = size; i >= ind; i--)
                game_name . content [i + 1] = game_name . content [i];
            game_name . content [ind++] = ch;
            size++;
        }
        show_window (game_name . box, TEXTBOX_HEIGHT, TEXTBOX_WIDTH, 0, 0, 0, ' ');
        mvwprintw (game_name . box, game_name . start_line . y, game_name . start_line . x, "%s", game_name . content);
        wmove (game_name . box, game_name . start_line . y, game_name . start_line . x + ind);
        wrefresh (game_name . box);
    }


    tag_open_game:

    struct stat statbuf;
    chdir ("users");
    chdir (player->username);
    chdir ("games");
    if (stat(game_name.content, &statbuf)) {
        chdir ("../../..");
        print_massage ("This game name isn't exist", 3);
        getch ();
        goto tag_game_name;
    }
    chdir (game_name.content);

    move (0, 0);
    FILE* game_file = fopen ("properties.txt", "r");

    fscanf (game_file, "level: %d\ncolor: %d", &play.level, &play.color);
    fscanf (game_file, "gold: %d\nhealth: %d\nhungry: %d", &play.gold, &play.health, &play.hungry);
    fscanf (game_file, "floor number: %d\nroom number: %d", &play.floor_number, &play.room_number);
    fscanf (game_file, "player position: (%d, %d)", &play.player.y, &play.player.x);

    fclose (game_file);

    play.floor = (floor*) malloc (4 * sizeof (floor));

    chdir ("floor1");
    read_floor (&play.floor[0]);
    chdir ("..");
    printw ("5");
    refresh ();


    chdir ("floor2");
    read_floor (&play.floor[1]);
    chdir ("..");
    printw ("5");
    refresh ();


    chdir ("floor3");
    read_floor (&play.floor[2]);
    chdir ("..");
    printw ("5");
    refresh ();


    chdir ("floor4");
    read_floor (&play.floor[3]);
    chdir ("..");
    printw ("5");
    refresh ();
}


/**************************************************************************************************************/


void show_cheat_room (floor* floor, room* room) {
    wclear (room->window);
    show_window (room->window, room->height, room->width, 0, 0, 0, ' ');

    for (int i=0; i<room->height; i++)
        for (int j=0; j<room->width; j++) {

            if (room->theme == NORMAL)
                wattron (room->window, BLUE);
            if (room->theme == NIGHT_MARE)
                wattron (room->window, GREEN);
            if (room->theme == SPELL)
                wattron (room->window, RED);
            if (room->theme == TREASURE)
                wattron (room->window, YELLOW);

            int y = room->start.y+i;
            int x = room->start.x+j;
            switch (room->matrix[i][j]) {
                case PILLAR:
                    if (i == 0 || i == room->height - 1 || j == 0 || j == room->width - 1)
                        break;
                mvwaddch (room->window, i, j, SIGN_PILLAR);
                break;
                case DOOR:
                    mvwaddch (room->window, i, j, SIGN_DOOR);
                    break;
                case WINDOWS:
                    mvwaddch (room->window, i, j, SIGN_WINDOWS);
                    break;
                case STAIR:
                    mvwaddch (room->window, i, j, SIGN_STAIR);
                    break;
                case SECRET_DOOR:
                    if (floor->appear[y][x] == 2)
                        mvwaddch (room->window, i, j, SIGN_SECRET_DOOR);
                break;
                case TRAP:
                    if (floor->appear[y][x] == 2)
                        mvwaddch (room->window, i, j, SIGN_TRAP);
                break;
                case FOOD:
                    mvwaddch (room->window, i, j, SIGN_FOOD);
                    break;
                case GOLD:
                    wattron (room->window, YELLOW);
                    mvwaddch (room->window, i, j, SIGN_GOLD);
                    wattroff (room->window, YELLOW);
                    break;
                case BLACK_GOLD:
                    wattron (room->window, BLACK);
                    mvwaddch (room->window, i, j, SIGN_GOLD);
                    wattroff (room->window, BLACK);
                    break;
                case MACE:
                    mvwaddch (room->window, i, j, SIGN_MACE);
                    break;
                case DAGGER:
                    mvwaddch (room->window, i, j, SIGN_DAGGER);
                    break;
                case MAGIC_WAND:
                    mvwaddch (room->window, i, j, SIGN_MAGIC_WAND);
                    break;
                case NORMAL_ARROW:
                    mvwaddch (room->window, i, j, SIGN_NORMAL_ARROW);
                    break;
                case SWORD:
                    mvwaddch (room->window, i, j, SIGN_SWORD);
                    break;
                case T_HEALTH:
                    mvwaddch (room->window, i, j, SIGN_T_HEALTH);
                    break;
                case T_SPEED:
                    mvwaddch (room->window, i, j, SIGN_T_SPEED);
                    break;
                case T_DAMAGE:
                    mvwaddch (room->window, i, j, SIGN_T_DAMAGE);
                    break;
                default:
                    mvwaddch (room->window, i, j, SIGN_IN_ROOM);
            }
            if (room->theme == NORMAL)
                wattroff (room->window, BLUE);
            if (room->theme == NIGHT_MARE)
                wattroff (room->window, GREEN);
            if (room->theme == SPELL)
                wattroff (room->window, RED);
            if (room->theme == TREASURE)
                wattroff (room->window, YELLOW);
        }
        wrefresh (room->window);
}


void show_cheat_floor (floor* floor) {
    clear ();

    for (int i=0; i<LINES-1; i++)
        for (int j=0; j<COLS; j++)
            if (floor->matrix[i][j] == CORIDOR)
                mvaddch (i, j, SIGN_CORIDOR);

    refresh ();
    for (int i=0; i<6; i++)
        show_cheat_room (floor, floor->room+i);
    if (floor->mood != 4)
        show_cheat_room (floor, floor->room+6);

    getch ();
}


void show_room (floor* floor, room* room, int number) {
    wclear (room->window);
    show_window (room->window, room->height, room->width, 0, 0, 0, ' ');

    for (int i=0; i<room->height; i++)
        for (int j=0; j<room->width; j++) {

            if (room->theme == NORMAL)
                wattron (room->window, BLUE);
            if (room->theme == NIGHT_MARE)
                wattron (room->window, GREEN);
            if (room->theme == SPELL)
                wattron (room->window, RED);
            if (room->theme == TREASURE)
                wattron (room->window, YELLOW);

            int y = room->start.y+i;
            int x = room->start.x+j;
            if (floor->appear[y][x] == 0) {
                mvwaddch (room->window, i, j, ' ');
                continue;
            }
            switch (room->matrix[i][j]) {
                case PILLAR:
                    if (i == 0 || i == room->height - 1 || j == 0 || j == room->width - 1)
                        break;
                mvwaddch (room->window, i, j, SIGN_PILLAR);
                break;
                case DOOR:
                    mvwaddch (room->window, i, j, SIGN_DOOR);
                    break;
                case WINDOWS:
                    mvwaddch (room->window, i, j, SIGN_WINDOWS);
                    break;
                case STAIR:
                    mvwaddch (room->window, i, j, SIGN_STAIR);
                    break;
                case SECRET_DOOR:
                    if (floor->appear[y][x] == 2)
                        mvwaddch (room->window, i, j, SIGN_SECRET_DOOR);
                break;
                case TRAP:
                    if (floor->appear[y][x] == 2)
                        mvwaddch (room->window, i, j, SIGN_TRAP);
                break;
                case FOOD:
                    mvwaddch (room->window, i, j, SIGN_FOOD);
                    break;
                case GOLD:
                    wattron (room->window, YELLOW);
                    mvwaddch (room->window, i, j, SIGN_GOLD);
                    wattroff (room->window, YELLOW);
                    break;
                case BLACK_GOLD:
                    wattron (room->window, BLACK);
                    mvwaddch (room->window, i, j, SIGN_GOLD);
                    wattroff (room->window, BLACK);
                    break;
                case MACE:
                    mvwaddch (room->window, i, j, SIGN_MACE);
                    break;
                case DAGGER:
                    mvwaddch (room->window, i, j, SIGN_DAGGER);
                    break;
                case MAGIC_WAND:
                    mvwaddch (room->window, i, j, SIGN_MAGIC_WAND);
                    break;
                case NORMAL_ARROW:
                    mvwaddch (room->window, i, j, SIGN_NORMAL_ARROW);
                    break;
                case SWORD:
                    mvwaddch (room->window, i, j, SIGN_SWORD);
                    break;
                case T_HEALTH:
                    mvwaddch (room->window, i, j, SIGN_T_HEALTH);
                    break;
                case T_SPEED:
                    mvwaddch (room->window, i, j, SIGN_T_SPEED);
                    break;
                case T_DAMAGE:
                    mvwaddch (room->window, i, j, SIGN_T_DAMAGE);
                    break;
                default:
                    mvwaddch (room->window, i, j, SIGN_IN_ROOM);
            }
            if (room->theme == NORMAL)
                wattroff (room->window, BLUE);
            if (room->theme == NIGHT_MARE)
                wattroff (room->window, GREEN);
            if (room->theme == SPELL)
                wattroff (room->window, RED);
            if (room->theme == TREASURE)
                wattroff (room->window, YELLOW);
        }
        wrefresh (room->window);
}


void show_floor (floor* floor, int number) {
    clear ();

    for (int i=0; i<LINES-1; i++)
        for (int j=0; j<COLS; j++)
            if (floor->matrix[i][j] == CORIDOR && floor->appear[i][j])
                mvaddch (i, j, SIGN_CORIDOR);

    refresh ();
    for (int i=0; i<6; i++)
        show_room (floor, floor->room+i, number);
    if (floor->mood != 4)
        show_room (floor, floor->room+6, number);
}


int play_game (user* player, game play) {
    clear ();
    refresh ();
    move (0, 0);
    int mood = 1;
    int move = 0;
    while (1) {
        refresh ();
        floor* floor = &play.floor[play.floor_number-1];

        for (int i=max(0,play.player.y-2); i<min(LINES-1,play.player.y+3); i++)
            for (int j=max(0,play.player.x-2); j<min(COLS,play.player.x+3); j++)
                mini_show_cell (floor, change_to_pos(i, j));
        show_cell (floor, play.player);

        show_floor (&play.floor[play.floor_number-1], play.floor_number);
        mvaddch (play.player.y, play.player.x, SIGN_PLAYER);
        refresh ();

        if (mood) {
            if (floor->matrix[play.player.y+1][play.player.x] == GOLD)
                ;
        }
        int ch = getch();
        switch (ch) {
            case 27:
                clear ();
                return CONTINUE;
            case '2':
                if (move)
                    while (floor->matrix[play.player.y+1][play.player.x])
                        play.player.y++;
                else
                    if (floor->matrix[play.player.y+1][play.player.x])
                        play.player.y++;
                mood++, move = 0;
                break;
            case '8':
                if (move)
                    while (floor->matrix[play.player.y-1][play.player.x])
                        play.player.y--;
                else
                    if (floor->matrix[play.player.y-1][play.player.x])
                        play.player.y--;
                mood++, move = 0;
                break;
            case '6':
                if (move)
                    while (floor->matrix[play.player.y][play.player.x+1])
                        play.player.x++;
                else
                    if (floor->matrix[play.player.y][play.player.x+1])
                        play.player.x++;
                mood++, move = 0;
                break;
            case '4':
                if (move)
                    while (floor->matrix[play.player.y][play.player.x-1])
                        play.player.x--;
                else
                    if (floor->matrix[play.player.y][play.player.x-1])
                        play.player.x--;
                mood++, move = 0;
                break;
            case '1':
                if (move)
                    while (floor->matrix[play.player.y-1][play.player.x-1])
                        play.player.y--, play.player.x--;
                else
                    if (floor->matrix[play.player.y-1][play.player.x-1])
                        play.player.y--, play.player.x--;
                mood++, move = 0;
                break;
            case '3':
                if (move)
                    while (floor->matrix[play.player.y-1][play.player.x+1])
                        play.player.y--, play.player.x++;
                    else
                        if (floor->matrix[play.player.y-1][play.player.x+1])
                            play.player.y--, play.player.x++;
                mood++, move = 0;
                break;
            case '7':
                if (move)
                    while (floor->matrix[play.player.y+1][play.player.x-1])
                        play.player.y++, play.player.x--;
                else
                    if (floor->matrix[play.player.y+1][play.player.x-1])
                        play.player.y++, play.player.x--;
                mood++, move = 0;
                break;
            case '9':
                if (move)
                    while (floor->matrix[play.player.y+1][play.player.x+1])
                        play.player.y++, play.player.x++;
                else
                    if (floor->matrix[play.player.y+1][play.player.x+1])
                        play.player.y++, play.player.x++;
                mood++, move++;
                break;
            case '5':
                show_cheat_floor (floor);
                break;
            case 'g':
                mood = -1;
                break;
            case 'f':
                move = 1;
                break;
        }
        for (int i=max(0,play.player.y-2); i<min(LINES-1,play.player.y+3); i++)
            for (int j=max(0,play.player.x-2); j<min(COLS,play.player.x+3); j++)
                inshow_cell (floor, change_to_pos(i, j));
        inshow_cell (floor, play.player);
    }
    return CONTINUE;
}


/**************************************************************************************************************/



void run_project () {
    srand (time (NULL));
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    refresh();
    make_colors();
//    move (0, 0);
//    cchar_t heart;
//    setcchar(&heart, L"♥", WA_NORMAL, 0, NULL);
//    add_wch(&heart);
//    add_wch(L'\u2694');
//    getch ();
    clear ();
//    attron(COLOR_PAIR(1));

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
    if (user_id == -1)
        goto tag_first;


    tag_pregame:

    int selected = pregmae_menu ();


    tag_logout:

    if (selected == 4) {
        logout (user_list + user_id - 1);
        goto tag_first;
    }


    tag_new:

    if (selected == 1) {
        setting_menu ();
        make_game (user_list + user_id - 1);
    }

    tag_resume:

    if (selected == 2) {
        read_game (user_list + user_id - 1);
    }

    tag_scoreboard:

    if (selected == 3) {
        scoreboard (user_list + user_id - 1);
        goto tag_pregame;
    }

    tag_game:

    int type = play_game (user_list + user_id - 1, play);
    save_game (user_list + user_id - 1, play);
    if (type == END)
        remove_game (user_list + user_id - 1, play);
    goto tag_pregame;


//    attroff(COLOR_PAIR(1));

    getch();
    clear();
    endwin();
}


int main(int argc, char *argv[]){
    run_project();
    return 0;
}
