#include "define.h"

#define TABLE_SIZE 100

typedef struct Book {
    char isbn[20];
    char authors[100];
    char title[100];
    int total;
    int available;
} Book;

  // Хеш таблица для хранения книг - выступает в роли буффера
typedef struct HashTable {
    Book *books[TABLE_SIZE];
} HashTable;


//  func


int hash(char *isbn);
Book *create_book(char *isbn, char *authors, char *title, int total, int available);
void add_book(HashTable *table, Book *book);
void delete_book(HashTable *table, char *isbn);
Book *find_book(HashTable *table, char *isbn) ;
void edit_book(HashTable *table, char *isbn, char *authors, char *title, int total, int available);
void change_book_quantity(HashTable *table, char *isbn, int total, int available);
void import_database_books(HashTable *table, char *filename);
void export_database_books(HashTable *table, char *filename);
void show_all_books(HashTable *table);
void menu_books();
void show_book_by_title(HashTable *table, char *title);
void Books();
