#include "define.h"
#include "students.h"

// хеш функция
int hash(char *id) { 
    int sum = 0; 
    for (int i = 0; i < strlen(id); i++) { 
        sum += id[i]; 
    } 
    return sum % TABLE_SIZE; 
}

// создаем нововго студента
Student *create_student(char *id, char *surname, char *name, char *patronymic, char *faculty, char *specialty) { 
    Student *student = (Student *) malloc(sizeof(Student)); // выделяем память под новый студент
    if (student == NULL){
        puts("Error: newstudent == NULL");
        exit(1);
    }
    strcpy(student->id, id); 
    strcpy(student->surname, surname);              //заполняем поля
    strcpy(student->name, name); 
    strcpy(student->patronymic, patronymic); 
    strcpy(student->faculty, faculty); 
    strcpy(student->specialty, specialty); 
    return student; 
}

// добавляем нового студента в хеш таблицу
void add_student(HashTableStudents *table, Student *student) { 
    int index = hash(student->id); 
    int count = 0; 
    while (table->students[index] != NULL) {  
        index = (index + 1) % TABLE_SIZE;
        count++;
        if (count == TABLE_SIZE) { //если не осталось места
            printf("Error: table is full\n");
            return;
        }
    } 
    table->students[index] = student; 
}


// удаляем студента 
void delete_student(HashTableStudents *table, char *id) { 
    int index = hash(id); 
    while (table->students[index] != NULL) { 
        if (strcmp(table->students[index]->id, id) == 0) {  
            table->students[index] = NULL;
            printf("Student deleted successfully\n"); 
            return; 
        } 
        index = (index + 1) % TABLE_SIZE; 
    } 
    printf("Error: student with ID %s not found\n", id); 
}

// нахождение студента по номеру зачетной книжки
Student *find_student(HashTableStudents *table, char *id) { 
    int index = hash(id); 
    while (table->students[index] != NULL) { 
        if (strcmp(table->students[index]->id, id) == 0) { 
            return table->students[index]; 
        } 
        index = (index + 1) % TABLE_SIZE; 
    } 
    return NULL; 
}

// функция изменения инфо о студенте
void edit_student(HashTableStudents *table, char *id) { 
    Student *student = find_student(table, id); 
    if (student != NULL) { 
        int choice; 
        do { 
            printf("What field do you want to edit?\n"); 
            printf("1. Surname\n"); 
            printf("2. Name\n"); 
            printf("3. Patronymic\n"); 
            printf("4. Faculty\n"); 
            printf("5. Specialty\n"); 
            printf("6. Exit\n"); 
            printf("Enter your choice: "); 
            scanf("%d", &choice); 
            switch (choice) { 
                case 1: 
                    printf("Enter new surname: "); 
                    scanf("%s", student->surname); 
                    break; 
                case 2: 
                    printf("Enter new name: "); 
                    scanf("%s", student->name); 
                    break; 
                case 3: 
                    printf("Enter new patronymic: "); 
                    scanf("%s", student->patronymic); 
                    break; 
                case 4: 
                    printf("Enter new faculty: "); 
                    scanf("%s", student->faculty); 
                    break; 
                case 5: 
                    printf("Enter new specialty: "); 
                    scanf("%s", student->specialty); 
                    break; 
                case 6: 
                    printf("Exiting edit mode...\n"); 
                    break; 
                default: 
                    printf("Invalid choice. Please enter a number between 1 and 6.\n"); 
                    printf("Enter your choice: "); 
                    scanf("%d", &choice); 
                    break; 
            } 
        } while (choice != 6); 
    } 
    else { 
        printf("Error: student with ID %s not found\n", id); 
    } 
}

// меню для студентов
void print_menu_students() { 
    printf("\n"); 
    printf("1. Add new student\n"); 
    printf("2. Delete student by ID\n"); 
    printf("3. Edit student information\n"); 
    printf("4. Show student by ID\n");
    printf("5. Show all students\n"); 
    printf("6. Save and Exit.\n"); 
}
// показ инфо о студенте по зачетке
void show_student_by_id(HashTableStudents *table, char *id) { 
    Student *student = find_student(table, id); 
    if (student != NULL) { 
        printf("ID: %s\n", student->id); 
        printf("Surname: %s\n", student->surname); 
        printf("Name: %s\n", student->name); 
        printf("Patronymic: %s\n", student->patronymic); 
        printf("Faculty: %s\n", student->faculty); 
        printf("Specialty: %s\n", student->specialty); 
    } 
    else { 
        printf("Error: student with ID %s not found\n", id); 
    } 
}
// чистка хеш таблицы
void clear_table(HashTableStudents *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table->students[i] != NULL) {
            free(table->students[i]); 
            table->students[i] = NULL; 
        }
    }
    free(table);
}


// импортируем из файла в хеш таблицу

void import_database(HashTableStudents *table, char *filename) { 
    FILE *fp = fopen(filename, "r"); 
    if (fp == NULL) { 
        printf("Error: cannot open file %s\n", filename); 
        return; 
    } 
    char line[200]; 
    while (fgets(line, 200, fp) != NULL) { 
        /*if (line[0] == '\n') {
            continue; 
        } */
        int len = strlen(line); 
        if (len > 0 && line[len-1] == '\n') { 
            line[len-1] = '\0'; // удаляем символ переноса строки
        } 
        if (line[0] == '\0') { 
            // проаускаем пустые строки
            continue; 
            }
        char *id = strtok(line, ";"); 
        char *surname = strtok(NULL, ";"); 
        char *name = strtok(NULL, ";"); 
        char *patronymic = strtok(NULL, ";"); 
        char *faculty = strtok(NULL, ";"); 
        char *specialty = strtok(NULL, ";"); 

        //  проверяем чтоб ни одно поле не было равно нулью
        if (id == NULL || surname == NULL || name == NULL || patronymic == NULL || faculty == NULL || specialty == NULL) {
            printf("Error: invalid data format in file %s\n", filename);
            return;
        }

        Student *student = create_student(id, surname, name, patronymic, faculty, specialty); 
        add_student(table, student); 
    } 
    fclose(fp);   
}

// функция экпорта в файл
void export_database(HashTableStudents *table, char *filename) { 
    FILE *fp = fopen(filename, "w"); 
    if (fp == NULL) { 
        printf("Error: cannot open file %s\n", filename); 
        return; 
    } 
    for (int i = 0; i < TABLE_SIZE; i++) { 
        if (table->students[i] != NULL) { 
            fprintf(fp, "%s;%s;%s;%s;%s;%s\n", 
                    table->students[i]->id,  
                    table->students[i]->surname, 
                    table->students[i]->name, 
                    table->students[i]->patronymic, 
                    table->students[i]->faculty, 
                    table->students[i]->specialty); 
        } 
    } 
    fclose(fp); 
}

void show_all_students(HashTableStudents *table) { 
    for (int i = 0; i < TABLE_SIZE; i++) { 
        if (table->students[i] != NULL) { 
            printf("ID: %s\n", table->students[i]->id); 
            printf("Surname: %s\n", table->students[i]->surname); 
            printf("Name: %s\n", table->students[i]->name); 
            printf("Patronymic: %s\n", table->students[i]->patronymic); 
            printf("Faculty: %s\n", table->students[i]->faculty); 
            printf("Specialty: %s\n", table->students[i]->specialty); 
            printf("\n"); 
        } 
    } 
}


// 

void Students()
{
     char *filename ="students.csv";
    HashTableStudents table;
    for (int i = 0; i < TABLE_SIZE; i++) { 
        table.students[i] = NULL; 
    }  
    import_database(&table, filename); 
    int choice; 
    char id[10], surname[50], name[50], patronymic[50], faculty[50], specialty[50]; 
    do { 
        print_menu_students(); 
        printf("Enter your choice: "); 
        scanf("%d", &choice); getchar();
        switch (choice) { 
            case 1:{ 
                printf("Enter student ID: "); 
               scanf("%[^\n]", id);
                if (find_student(&table, id) != NULL) { 
                    printf("Error: student with ID %s already exists\n", id); 
                } 
                else { 
                    printf("Enter student surname: ");getchar(); 
                    scanf("%[^\n]", surname);
                    printf("Enter student name: "); getchar();
                    scanf("%[^\n]", name);
                    printf("Enter student patronymic: "); getchar();
                    scanf("%[^\n]", patronymic);
                    printf("Enter student faculty: "); getchar();
                    scanf("%[^\n]", faculty);
                    printf("Enter student specialty: "); getchar();
                    scanf("%[^\n]", specialty);getchar();
                    Student *student = create_student(id, surname, name, patronymic, faculty, specialty); 
                    add_student(&table, student); 
                    export_database(&table,filename);
                    printf("Student added successfully\n"); 
                } 
                break;
            } 
            case 2: { 
                printf("Enter student ID: "); 
                scanf("%[^\n]", id);
                delete_student(&table, id); 
                export_database(&table,filename);
                printf("Student deleted successfully\n"); 
                break; 
            }
            case 3: {
                printf("Enter student ID: "); 
                scanf("%[^\n]", id);getchar();
                edit_student(&table, id); 
                export_database(&table,filename);
                    break;
            } 
                case 4: {
                    printf("Enter student ID: "); 
                    scanf("%[^\n]", id); getchar();
                    show_student_by_id(&table, id); 
                    break;
                }
                case 5:{
                    show_all_students(&table); 
                    break; 
                } 
                case 6: {
                    printf("Exiting program...\n"); 
                    export_database(&table,filename);
                    printf("Database exported successfully\n"); 
                    break; 
                }
                default: {
                    printf("Invalid choice. Please enter a number between 1 and 7.\n"); 
                    break; 
                }
                
        } 
    } 
    while (choice != 6); 

    //чистка хеш таблицы
    clear_table(&table);
}

/*
int main() {
    Students();
    return 0; 
}*/