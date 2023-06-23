#include "define.h"
#include "books.h"
#include "students.h"

#define MAX_USERS 100 // макс кол пользоваталей
#define MAX_LEN 100 // макс длина логина и пароля

// структура для хранения инфо о пользователе
typedef struct {
    char login[MAX_LEN];
    char password[MAX_LEN];
    int access_to_books;
    int access_to_students;
} User;

// массив будет содержат данные типа user - пользователелей
User users[MAX_USERS]; 
int num_users = 0; // колич пользователей в массиве

// функция чтения пользователей из файла 
void read_users(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }
    char line[MAX_LEN*6]; // максимальная длина строки в файле
    while (fgets(line, MAX_LEN*6, fp) != NULL) {
        char *token = strtok(line, ";");
        strcpy(users[num_users].login, token);
        token = strtok(NULL, ";");
        strcpy(users[num_users].password, token);
        token = strtok(NULL, ";");
        users[num_users].access_to_books = atoi(token);
        token = strtok(NULL, ";");
        users[num_users].access_to_students = atoi(token);
        num_users++;
    }
    fclose(fp);
}

// функция для записи пользователей в файл
void write_users(char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < num_users; i++) {
        fprintf(fp, "%s;%s;%d;%d\n", users[i].login, users[i].password, users[i].access_to_books, users[i].access_to_students);
    }
    fclose(fp);
}

// функция для проверки логина и пароля
int check_login(char *login, char *password) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].login, login) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // возвращаем индекс пользователья в массиве
        }
    }
    return -1; // если нет такого пользователья то -1
}

// добавление пользователья
void add_user() { 
    if (num_users == MAX_USERS) { 
        printf("Error: maximum number of users reached\n"); 
        return; 
    } 
    char login[MAX_LEN]; 
    char password[MAX_LEN]; 
    int access_to_books; 
    int access_to_students; 
    printf("Enter login: "); 
    scanf("%s", login); 
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].login, login) == 0) {
            printf("Error: user with this login already exists\n");
            return;
        }
    }
    printf("Enter password: "); 
    scanf("%s", password); 
    printf("Access to books (0 - no, 1 - yes): "); 
    scanf("%d", &access_to_books); 
    printf("Access to students (0 - no, 1 - yes): "); 
    scanf("%d", &access_to_students); 
    strcpy(users[num_users].login, login); 
    strcpy(users[num_users].password, password); 
    users[num_users].access_to_books = access_to_books; 
    users[num_users].access_to_students = access_to_students; 
    num_users++; 
    printf("User added successfully\n"); 
}




// меню для доступа и к студентам и к библиотеке
void menu_access_to_both() {
    printf("Menu:\n");
    printf("1. Books\n");
    printf("2. Students\n");
    printf("3. Add new user\n");
    printf("0. Exit\n");
}




int main() {
    read_users("users.csv");
    char login[MAX_LEN];
    char password[MAX_LEN];
    int user_index;
    do {
        printf("Enter login: ");
        scanf("%s", login);
        printf("Enter password: ");
        scanf("%s", password);
        user_index = check_login(login, password);  // получем индекс пользователя в массиве 
        if (user_index == -1) {
            printf("Error: incorrect login or password\n");
        }
    } while (user_index == -1);


    // доступ и к книгам и к студентам 
    if (users[user_index].access_to_books && users[user_index].access_to_students) {
        int choice;
        do {
            menu_access_to_both();
            printf("Enter choice: ");
            scanf("%d", &choice);getchar();
            switch (choice) {       // даем выбор 
                case 1: {   int choice;
                    // выбор 1 - доступ к книгам  
                    do {
                        
                        puts("1. Access to books.");
                        puts("0. Returning to main menu.");
                        printf("Enter choice: ");
                        scanf("%d", &choice);
                        switch (choice) {
                            case 1: {
                                Books();
                                break;
                            }
                            case 0:
                                printf("Returning to main menu...\n");
                                break;
                            default:
                                printf("Error: invalid choice. Try again!\n");
                                break;
                        }
                    } while (choice != 0);
                    break;
                }
                // выбор 2 - доступ к студентам
                case 2: {
                        int choice;
                     do {
                        puts("1. Access to students.");
                        puts("0. Returning to main menu.");
                        printf("Enter choice: ");
                        scanf("%d", &choice);getchar();
                        switch (choice) {
                            case 1: {
                                Students();
                                break;
                            }
                            case 0:
                                printf("Returning to main menu...\n");
                                break;
                            default:
                                printf("Error: invalid choice. Try again!\n");
                                break;
                        }
                    } while (choice != 0);
                    break;
                }
                case 3: {
                    int choice;
                    do {
                        puts("1 - Add User.");
                        puts("0 - Return to main menu.");
                        scanf("%d",&choice);getchar();
                        switch (choice)
                        {
                        case 1:
                            add_user();
                            write_users("users.csv");
                            break;
                        case 0:
                             printf("Returning to main menu...\n");
                        default:
                            printf("Error: invalid choice. Try again!\n");
                            break;
                        }
                    }while(choice != 0);
                    break;
                }
                case 0: {
                    printf("Exiting...\n");
                    puts("GoodBye!");
                    exit(0);
                    break;
                }
                default: {
                    printf("Error: invalid choice\n");
                    break;
                }
            }
        } while (choice != 0);

        // доступ только к книгам
    } else if (users[user_index].access_to_books) {
            int choice;
            do {
                puts("1. Access to books");
                puts("0. Exit.");
                printf("Enter choice: ");
                scanf("%d", &choice);getchar();
                switch (choice) {

                    case 1: {
                        Books();
                        break;
                    }
                    case 0:
                        printf("Exiting...\n");
                        break;
                    default:
                        printf("Error: invalid choice. Try again!\n");
                        break;
                }
            } while (choice != 0);
        
    } else if (users[user_index].access_to_students) {
        int choice;
        do {
            puts("1 - Access to students.");
            puts("0 - Exit.");
            printf("Enter choice: ");
            scanf("%d", &choice);getchar();
            switch (choice) {
                case 1:
                    Students();
                    break;
                case 0:
                    printf("Exiting Goodbye...\n");
                    break;
                default:
                    printf("Error: invalid choice\n");
                    break;
            }
        } while (choice != 0);
    }
    else{
        puts("Error in file");
        exit(1);
    }
    ///write_users("users.csv"); /////////
    return 0;
}