#include "define.h"

#define TABLE_SIZE 100   //размер хеш таблицы студентов

// структура student
typedef struct Student { 
    char id[10]; 
    char surname[50]; 
    char name[50]; 
    char patronymic[50]; 
    char faculty[50];  
    char specialty[50]; 
} Student;

// хеш таблица студентов
typedef struct HashTableStudents { 
    Student *students[TABLE_SIZE]; 
} HashTableStudents;


// функции
int hash(char *id);
Student *create_student(char *id, char *surname, char *name, char *patronymic, char *faculty, char *specialty);
void add_student(HashTableStudents *table, Student *student);
void delete_student(HashTableStudents *table, char *id);
Student *find_student(HashTableStudents *table, char *id);
void edit_student(HashTableStudents *table, char *id);
void print_menu_students();
void show_student_by_id(HashTableStudents *table, char *id);
void clear_table(HashTableStudents *table);
void import_database(HashTableStudents *table, char *filename);
void export_database(HashTableStudents *table, char *filename);
void show_all_students(HashTableStudents *table);
void Students();