#include "define.h"
#include "books.h"

  // хеш функция
int hashBook(char *isbn) {
    int sum = 0;
    for (int i = 0; i < strlen(isbn); i++) {
        sum += isbn[i];
    }
    return sum % TABLE_SIZE;
}

  // функция создания новой книги
Book *create_book(char *isbn, char *authors, char *title, int total, int available) {
    Book *book = (Book *) malloc(sizeof(Book));
    strcpy(book->isbn, isbn);
    strcpy(book->authors, authors);
    strcpy(book->title, title);
    book->total = total;
    book->available = available;
    return book;
}

    // функция добавления новой книги в хеш таблицу
void add_book(HashTable *table, Book *book) {
    int index = hashBook(book->isbn);
    while (table->books[index] != NULL) {   
        index = (index + 1) % TABLE_SIZE;
    }
    table->books[index] = book;
}

 // функция удаления книги по номеру ISBN - из хеш таблицы
void delete_book(HashTable *table, char *isbn) {
    int index = hashBook(isbn);
    while (table->books[index] != NULL) {
        if (strcmp(table->books[index]->isbn, isbn) == 0) {
            table->books[index] = NULL;  
            return;
        }
        index = (index + 1) % TABLE_SIZE;
    }
}

  // функция поиска книги по ISBN в хеш таблице
Book *find_book(HashTable *table, char *isbn) {
    int index = hashBook(isbn);
    while (table->books[index] != NULL) {
        if (strcmp(table->books[index]->isbn, isbn) == 0) {  // поиск по номеру ISBN
            return table->books[index];
        }
        index = (index + 1) % TABLE_SIZE;
    }
    return NULL;
}

    // функция редактироваия  информации о книге по номеру ISBN
void edit_book(HashTable *table, char *isbn, char *authors, char *title, int total, int available) {
    Book *book = find_book(table, isbn);
    if (book != NULL) {   // проверка на если есть, тогда изменяем
        strcpy(book->authors, authors);
        strcpy(book->title, title);
        book->total = total;
        book->available = available;
    }    
}


    // функция редактирования количества книг по номеру ISBN в таблице
void change_book_quantity(HashTable *table, char *isbn, int total, int available) {
    Book *book = find_book(table, isbn);
    if (book != NULL) {
        book->total = total;
        book->available = available;
    }
}



 //функция импорта базы данных из файла csv в хеш таблицу table
void import_database_books(HashTable *table, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }
    char line[200];
    while (fgets(line, 200, fp) != NULL) {
        char *isbn = strtok(line, ";");
        char *authors = strtok(NULL, ";");
        char *title = strtok(NULL, ";");
        int total = atoi(strtok(NULL, ";"));
        int available = atoi(strtok(NULL, ";"));
        Book *book = create_book(isbn, authors, title, total, available);
        add_book(table, book);
    }
    fclose(fp);
}


    // функция экпорта хеш таблицы - буффера в файл 
void export_database_books(HashTable *table, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        return;
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table->books[i] != NULL) {
            fprintf(fp, "%s;%s;%s;%d;%d\n", table->books[i]->isbn, table->books[i]->authors, table->books[i]->title, table->books[i]->total, table->books[i]->available);
        }
    }
    fclose(fp);
}

// вывод инфо про всех книг 
void show_all_books(HashTable *table) { 
    printf("List of all books:\n"); 
    for (int i = 0; i < TABLE_SIZE; i++) { 
        if (table->books[i] != NULL) { 
            printf("ISBN: %s\n", table->books[i]->isbn); 
            printf("Authors: %s\n", table->books[i]->authors); 
            printf("Title: %s\n", table->books[i]->title); 
            printf("Total: %d\n", table->books[i]->total); 
            printf("Available: %d\n", table->books[i]->available); 
            printf("\n"); 
        }
    } 
}

 // меню
void menu_books() {
    printf("\n");
    printf("1. Add new book\n");
    printf("2. Delete book by ISBN\n");
    printf("3. Edit book information\n");
    printf("4. Change book quantity by ISBN\n");
    printf("5. Export database to file\n");
    printf("6. Show book by title\n");
    printf("7. Show all books.\n");
    printf("8. Exit.\n");
}

// показ информации о книге по ее названию
void show_book_by_title(HashTable *table, char *title) { 
    int found = 0; 
    for (int i = 0; i < TABLE_SIZE; i++) { 
        if (table->books[i] != NULL && strcmp(table->books[i]->title, title) == 0) {  
            printf("ISBN: %s\n", table->books[i]->isbn); 
            printf("Authors: %s\n", table->books[i]->authors); 
            printf("Title: %s\n", table->books[i]->title); 
            printf("Total: %d\n", table->books[i]->total); 
            printf("Available: %d\n", table->books[i]->available); 
            printf("\n"); found = 1; 
        } 
    } 
    if (!found) { 
        printf("Error: book with title %s not found\n", title); 
    } 
}

void Books(){
    HashTable table;
    char *filename = "books.csv";

    for (int i = 0; i < TABLE_SIZE; i++) {
        table.books[i] = NULL;
    }
    
    import_database_books(&table, filename);
    int choice;
    do {
        menu_books();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                char isbn[20];
                char authors[100];
                char title[100];
                int total;
                int available;
                printf("Enter ISBN: ");
                scanf("%s", isbn);
                if (find_book(&table, isbn) != NULL) {
                    printf("Error: book with ISBN %s already exists\n", isbn);
                    break;
                }
                printf("Enter authors: ");
                getchar();
               scanf("%[^\n]", authors);

                printf("Enter title: ");
                getchar();
                scanf("%[^\n]", title);
                printf("Enter total number of books: ");
                getchar();
                scanf("%d", &total);
                printf("Enter number of available books: ");
                getchar();
                scanf("%d", &available);
                Book *book = create_book(isbn, authors, title, total, available);
                add_book(&table, book);
                export_database_books(&table,filename);
                printf("Book with ISBN %s has been added\n", isbn);
                break;
            }
            case 2: {
                char isbn[20];
                printf("Enter ISBN: ");
                scanf("%s", isbn);
                if (find_book(&table, isbn) == NULL) {
                    printf("Error: book with ISBN %s not found\n", isbn);
                    break;
                }
                delete_book(&table, isbn);
                printf("Book with ISBN %s has been deleted\n", isbn);
                export_database_books(&table,filename);
                break;
            }
            case 3: {
                char isbn[20];
                char authors[100];
                char title[100];
                int total;
                int available;
                printf("Enter ISBN: ");
                scanf("%s", isbn);
                if (find_book(&table, isbn) == NULL) {
                    printf("Error: book with ISBN %s not found\n", isbn);
                    break;
                }
                printf("Enter authors: ");
                getchar();
                scanf("%[^\n]", authors);
                printf("Enter title: ");
                getchar();
                scanf("%[^\n]", title);
                printf("Enter total number of books: ");
                scanf("%d", &total);
                printf("Enter number of available books: ");
                scanf("%d", &available);
                edit_book(&table, isbn, authors, title, total, available);
                printf("Book with ISBN %s has been edited\n", isbn);
                export_database_books(&table,filename);
                break;
            }
            case 4: {
                char isbn[20];
                int total;
                int available;
                printf("Enter ISBN: ");
                scanf("%s", isbn);
                if (find_book(&table, isbn) == NULL) {
                    printf("Error: book with ISBN %s not found\n", isbn);
                    break;
                }
                printf("Enter total number of books: ");
                scanf("%d", &total);
                printf("Enter number of available books: ");
                scanf("%d", &available);
                change_book_quantity(&table, isbn, total, available);
                export_database_books(&table,filename);
                printf("Quantity of book with ISBN %s has been changed\n", isbn);
                break;
            }
            case 5: {
                char filename[] = "books.csv";
                export_database_books(&table, filename);
                printf("Database has been exported to file %s\n", filename);
                break;
            }
           
            case 6: {
                char title[100]; 
                printf("Enter title: "); 
                getchar(); 
                scanf("%[^\n]", title); // считывает до тех пор пака не встретится eof или '\n'
                show_book_by_title(&table, title); 
                break;
            }

            case 7: {
                show_all_books(&table); 
                break; 
            }
            case 8: {
                char filename[] = "books.csv";
                export_database_books(&table, filename);
                printf("Database has been saved to file %s\n", filename);
                printf("Exiting...\n");
                printf("GoodBye!\n");
                break;
            }
            default: {
                printf("Error: Invalid choice\n");
                break;
            }
        }
    } while (choice != 8);
}
/*
int main() {
    Books();
    return 0;
}*/
