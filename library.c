#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_BOOKS 100

typedef struct {
    char name[50];
    char username[20];
    char password[20];
} Student;

typedef struct {
    char title[100];
    char author[100];
    int id;
    int issued;
    int purchased;
    char version[50];  // Added version field
    int publication_year;  // Added publication year field
} Book;

Student students[MAX_USERS];
Book books[MAX_BOOKS];
int student_count = 0;
int book_count = 0;

void clear_screen() {
    // Clear screen based on the operating system
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    int choice, i, j;
    int student_index, book_id, admin_logged_in = 0;

    FILE *student_file = fopen("students.txt", "r");
    while (fscanf(student_file, "%s %s %s", students[student_count].name, students[student_count].username, students[student_count].password) != EOF) {
        student_count++;
    }
    fclose(student_file);

    FILE *book_file = fopen("books.txt", "r");
    while (fscanf(book_file, "%s %s %d %d %d %s %d", books[book_count].title, books[book_count].author, &books[book_count].id, &books[book_count].issued, &books[book_count].purchased, books[book_count].version, &books[book_count].publication_year) != EOF) {
        book_count++;
    }
    fclose(book_file);

    while (1) {
        // Clear the screen after each main menu
        printf("\t\n-------- || Library Management System || ----------\n");
        printf("\t    *************************\n");

        printf("\n\t1. Register as Student\n");
        printf("\t2. Student Login\n");
        printf("\t3. Admin Login\n");
        printf("\t4. Exit\n");
        printf("\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            clear_screen();
            if (student_count < MAX_USERS) {
                printf("Enter your name: ");
                getchar();
                fgets(students[student_count].name, 50, stdin);
                students[student_count].name[strcspn(students[student_count].name, "\n")] = 0;  // Remove newline

                printf("Enter your username: ");
                scanf("%s", students[student_count].username);
                printf("Enter your password: ");
                scanf("%s", students[student_count].password);

                student_count++;

                student_file = fopen("students.txt", "w");
                for (i = 0; i < student_count; i++) {
                    fprintf(student_file, "%s %s %s\n", students[i].name, students[i].username, students[i].password);
                }
                clear_screen();
                fclose(student_file);

                printf("\n\tRegistration successful!\n");
            }
            else {
                printf("\n\tStudent registration limit reached.\n");
            }
        } else if (choice == 2) {
            clear_screen();
            char username[20], password[20];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            student_index = -1;
            for (i = 0; i < student_count; i++) {
                if (strcmp(students[i].username, username) == 0 && strcmp(students[i].password, password) == 0) {
                    student_index = i;
                    break;
                }
            }
            clear_screen();

            if (student_index != -1) {
                printf("\n\tLogin successful! Welcome %s.\n", students[student_index].name);
                printf("\t    ==================\n");

                while (1) {
                    printf("\n\t ||Student Panel ||\n");
                    printf("\t  ****************\n");
                    printf("\n\t1. View available books\n");
                    printf("\t2. Issue a book\n");
                    printf("\t3. Return a book\n");
                    printf("\t4. Logout\n");

                    printf("\n*\n");

                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    if (choice == 1) {
                        clear_screen();  // Clear screen for books listing
                        printf("\nAvailable Books:\n");
                        printf("----------------\n");
                        int available_books = 0;
                        for (i = 0; i < book_count; i++) {
                            if (!books[i].issued && !books[i].purchased) {
                                printf("ID: %d, Title: %s, Author: %s, version: %s, Year: %d\n", books[i].id, books[i].title, books[i].author, books[i].version, books[i].publication_year);
                                available_books++;
                            }
                        }
                        if (available_books == 0) {
                            printf("No available books at the moment.\n");
                        }
                    } else if (choice == 2) {
                        clear_screen();
                        printf("Enter book ID to issue: ");
                        scanf("%d", &book_id);

                        if (book_id > 0 && book_id <= book_count && !books[book_id - 1].issued && !books[book_id - 1].purchased) {
                            books[book_id - 1].issued = 1;
                            printf("\n\tBook issued successfully!\n");

                            book_file = fopen("books.txt", "w");
                            for (i = 0; i < book_count; i++) {
                                fprintf(book_file , "%s %s %d %d %d %s %d\n", books[i].title, books[i].author, books[i].id, books[i].issued, books[i].purchased, books[i].version, books[i].publication_year);
                            }
                            fclose(book_file);
                        } else {
                            printf("\n\tInvalid book ID or book already issued/purchased.\n");
                        }
                    } else if (choice == 3) {
                        clear_screen();
                        printf("Enter book ID to return: ");
                        scanf("%d", &book_id);

                        if (book_id > 0 && book_id <= book_count && books[book_id - 1].issued) {
                            books[book_id - 1].issued = 0;
                            printf("\n\tBook returned successfully!\n");

                            book_file = fopen("books.txt", "w");
                            for (i = 0; i < book_count; i++) {
                                fprintf(book_file, "%s %s %d %d %d %s %d\n", books[i].title, books[i].author, books[i].id, books[i].issued, books[i].purchased, books[i].version, books[i].publication_year);
                            }
                            fclose(book_file);
                        } else {
                            printf("Invalid book ID or book not issued.\n");
                        }
                    } else if (choice == 4) {
                        printf("\n\tLogging out...\n");
                        break;
                    } else {
                        printf("\n\tInvalid choice!\n");
                    }
                }
                clear_screen();
            } else {
                printf("\n\tInvalid login credentials.\n");
            }
        } else if (choice == 3) {
            clear_screen();// Admin Login
            char username[20], password[20];
            printf("Enter admin username: ");
            scanf("%s", username);
            printf("Enter admin password: ");
            scanf("%s", password);
            clear_screen();
            if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                printf("\n\tAdmin login successful!\n");
                admin_logged_in = 1;

                while (admin_logged_in) {
                    printf("\n\t || Admin Panel ||\n");
                    printf("\t  ***************\n");
                    printf("1. Add new book\n");
                    printf("2. View all books\n");
                    printf("3. View issued books\n");
                    printf("4. Remove a book\n");
                    printf("5. Logout\n");
                    printf("\n");

                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    if (choice == 1) {
                        clear_screen();

                        if (book_count < MAX_BOOKS) {
                            getchar();
                            printf("Enter book title: ");
                            fgets(books[book_count].title, 100, stdin);
                            books[book_count].title[strcspn(books[book_count].title, "\n")] = 0;  // Remove newline

                            printf("Enter book author: ");
                            fgets(books[book_count].author, 100, stdin);
                            books[book_count].author[strcspn(books[book_count].author, "\n")] = 0;  // Remove newline

                            printf("Enter book version: ");
                            fgets(books[book_count].version, 50, stdin);
                            books[book_count].version[strcspn(books[book_count].version, "\n")] = 0;  // Remove newline

                            printf("Enter publication year: ");
                            scanf("%d", &books[book_count].publication_year);

                            books[book_count].id = book_count + 1;

                            book_count++;

                            book_file = fopen("books.txt", "w");
                            for (i = 0; i < book_count; i++) {
                                fprintf(book_file, "%s %s %d %d %d %s %d\n", books[i].title, books[i].author, books[i].id, books[i].issued, books[i].purchased, books[i].version, books[i].publication_year);
                            }
                            clear_screen();
                            fclose(book_file);

                            printf("\n\tNew book added successfully!\n");
                        } else {
                            printf("\n\tLibrary is full, cannot add more books.\n");
                        }
                    } else if (choice == 2) {
                        clear_screen();  // Clear screen before showing books
                        printf("\nAll Books:\n");

                        for (i = 0; i < book_count; i++) {
                            printf("ID: %d, Title: %s, Author: %s, version: %s, Year: %d\n", books[i].id, books[i].title, books[i].author, books[i].version, books[i].publication_year);
                        }
                    } else if (choice == 3) {
                        clear_screen();  // Clear screen for issued books
                        printf("\nIssued Books:\n");
                        int issued_books = 0;
                        for (i = 0; i < book_count; i++) {
                            if (books[i].issued) {
                                printf("ID: %d, Title: %s, Author: %s, version: %s, Year: %d\n", books[i].id, books[i].title, books[i].author, books[i].version, books[i].publication_year);
                                issued_books++;
                            }
                        }
                        if (issued_books == 0) {
                            printf("No books have been issued yet.\n");
                        }
                    } else if (choice == 4) {
                        clear_screen();
                        printf("Enter the ID of the book to remove: ");
                        scanf("%d", &book_id);

                        if (book_id > 0 && book_id <= book_count) {
                            for (i = book_id - 1; i < book_count - 1; i++) {
                                books[i] = books[i + 1];
                            }
                            book_count--;

                            book_file = fopen("books.txt", "w");
                            for (i = 0; i < book_count; i++) {
                                fprintf(book_file, "%s %s %d %d %d %s %d\n", books[i].title, books[i].author, books[i].id, books[i].issued, books[i].purchased, books[i].version, books[i].publication_year);
                            }
                            fclose(book_file);

                            printf("\n\tBook removed successfully!\n");
                        } else {
                            printf("\n\tInvalid book ID.\n");
                        }
                    } else if (choice == 5) {
                        clear_screen();
                        printf("\n\tLogging out...\n");
                        admin_logged_in = 0;
                    } else {
                        printf("\n\tInvalid choice!\n");
                    }
                }
            } else {
                printf("\n\tInvalid admin credentials.\n");
            }
        } else if (choice == 4) {
            printf( "\n\tExiting system...\n");
            break;
        } else {
            printf("\n\tInvalid choice! Try again.\n");
        }
    }

    return 0;
}

