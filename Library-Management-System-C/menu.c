#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 200

typedef struct {
    int bookID;
    char title[100];
    char author[100];
    int totalCopies;
    int availableCopies;
} Book;

// ========== File Handling ==========

int loadBooks(Book books[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char header[200];
    fgets(header, sizeof(header), file); // skip header

    int count = 0;
    while (fscanf(file, "%d | %[^|] | %[^|] | %d | %d\n",
                  &books[count].bookID,
                  books[count].title,
                  books[count].author,
                  &books[count].totalCopies,
                  &books[count].availableCopies) == 5) {
        count++;
    }

    fclose(file);
    return count;
}

void saveBooks(Book books[], int totalBooks, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error saving file: %s\n", filename);
        return;
    }

    fprintf(file, "BookID | Title | Author | Total Copies | Available Copies\n");
    for (int i = 0; i < totalBooks; i++) {
        fprintf(file, "%d | %s | %s | %d | %d\n",
                books[i].bookID,
                books[i].title,
                books[i].author,
                books[i].totalCopies,
                books[i].availableCopies);
    }

    fclose(file);
}

// ========== Tracking ==========

void logTransaction(int studentID, const char *action, Book book) {
    FILE *file = fopen("Transactions.txt", "a");
    if (!file) {
        printf("⚠️ Could not open Transactions.txt for logging!\n");
        return;
    }

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = 0; // remove newline

    fprintf(file, "%d | %s | %d | %s | %s | %s\n",
            studentID, action, book.bookID, book.title, book.author, timestamp);

    fclose(file);
}

// ========== Features ==========

void displayBooks(Book books[], int totalBooks) {
    printf("\n📚 Library Book List:\n");
    for (int i = 0; i < totalBooks; i++) {
        printf("%d | %s | %s | %d/%d copies available\n",
               books[i].bookID,
               books[i].title,
               books[i].author,
               books[i].availableCopies,
               books[i].totalCopies);
    }
}

void searchBook(Book books[], int totalBooks, char *title) {
    int found = 0;
    for (int i = 0; i < totalBooks; i++) {
        if (strstr(books[i].title, title) != NULL) {
            printf("✅ Found: %d | %s | %s | %d/%d copies\n",
                   books[i].bookID,
                   books[i].title,
                   books[i].author,
                   books[i].availableCopies,
                   books[i].totalCopies);
            found = 1;
        }
    }
    if (!found) printf("❌ Book not found!\n");
}

void borrowBook(Book books[], int totalBooks, int bookID, const char *filename, int studentID) {
    for (int i = 0; i < totalBooks; i++) {
        if (books[i].bookID == bookID) {
            if (books[i].availableCopies > 0) {
                books[i].availableCopies--;
                printf("✅ Student %d borrowed: %s by %s\n", studentID, books[i].title, books[i].author);
                saveBooks(books, totalBooks, filename);
                logTransaction(studentID, "BORROW", books[i]);
                return;
            } else {
                printf("❌ No copies available for %s\n", books[i].title);
                return;
            }
        }
    }
    printf("❌ Book ID %d not found!\n", bookID);
}

void returnBook(Book books[], int totalBooks, int bookID, const char *filename, int studentID) {
    for (int i = 0; i < totalBooks; i++) {
        if (books[i].bookID == bookID) {
            if (books[i].availableCopies < books[i].totalCopies) {
                books[i].availableCopies++;
                printf("✅ Student %d returned: %s by %s\n", studentID, books[i].title, books[i].author);
                saveBooks(books, totalBooks, filename);
                logTransaction(studentID, "RETURN", books[i]);
                return;
            } else {
                printf("⚠️ All copies already in library!\n");
                return;
            }
        }
    }
    printf("❌ Book ID %d not found!\n", bookID);
}

// ========== Admin Functions (same as before) ==========
// (addBook, deleteBook, updateBook) → keep unchanged

// ========== MAIN ==========

void addBook(Book books[], int *totalBooks, const char *filename) {
    if (*totalBooks >= MAX_BOOKS) {
        printf("❌ Cannot add more books. Library is full!\n");
        return;
    }

    Book newBook;
    printf("Enter Book ID: ");
    scanf("%d", &newBook.bookID);
    getchar();

    printf("Enter Title: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Enter Total Copies: ");
    scanf("%d", &newBook.totalCopies);
    newBook.availableCopies = newBook.totalCopies;

    books[*totalBooks] = newBook;
    (*totalBooks)++;

    saveBooks(books, *totalBooks, filename);
    printf("✅ Book added successfully!\n");
}

// Delete a book
void deleteBook(Book books[], int *totalBooks, int bookID, const char *filename) {
    int found = 0;
    for (int i = 0; i < *totalBooks; i++) {
        if (books[i].bookID == bookID) {
            for (int j = i; j < *totalBooks - 1; j++) {
                books[j] = books[j + 1];
            }
            (*totalBooks)--;
            saveBooks(books, *totalBooks, filename);
            printf("✅ Book with ID %d deleted successfully!\n", bookID);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("❌ Book ID %d not found!\n", bookID);
    }
}

// Update book details
void updateBook(Book books[], int totalBooks, int bookID, const char *filename) {
    for (int i = 0; i < totalBooks; i++) {
        if (books[i].bookID == bookID) {
            printf("Editing Book: %s by %s\n", books[i].title, books[i].author);

            printf("Enter new Title (leave empty to keep same): ");
            char newTitle[100];
            fgets(newTitle, sizeof(newTitle), stdin);
            if (newTitle[0] != '\n') {
                newTitle[strcspn(newTitle, "\n")] = 0;
                strcpy(books[i].title, newTitle);
            }

            printf("Enter new Author (leave empty to keep same): ");
            char newAuthor[100];
            fgets(newAuthor, sizeof(newAuthor), stdin);
            if (newAuthor[0] != '\n') {
                newAuthor[strcspn(newAuthor, "\n")] = 0;
                strcpy(books[i].author, newAuthor);
            }

            printf("Enter new Total Copies (0 to keep same): ");
            int newTotal;
            scanf("%d", &newTotal);
            getchar();
            if (newTotal > 0) {
                books[i].totalCopies = newTotal;
                if (books[i].availableCopies > newTotal) {
                    books[i].availableCopies = newTotal;
                }
            }

            saveBooks(books, totalBooks, filename);
            printf("✅ Book updated successfully!\n");
            return;
        }
    }
    printf("❌ Book ID %d not found!\n", bookID);
}

int main() {
    Book books[MAX_BOOKS];
    const char *filename = "Books.txt";
    int totalBooks = loadBooks(books, filename);

    if (totalBooks == 0) {
        printf("No books loaded. Exiting...\n");
        return 1;
    }

    char role[20], password[20];
    int isAdmin = 0, studentID = -1;

    printf("Login as (admin/student): ");
    scanf("%s", role);

    if (strcmp(role, "admin") == 0) {
        printf("Enter Admin Password: ");
        scanf("%s", password);
        if (strcmp(password, "admin123") == 0) {
            isAdmin = 1;
            printf("✅ Admin login successful!\n");
        } else {
            printf("❌ Wrong password! Exiting...\n");
            exit(0);
        }
    } else {
        printf("Enter your Student ID: ");
        scanf("%d", &studentID);
        printf("✅ Logged in as Student (ID: %d)\n", studentID);
    }
    

    int choice, bookID;
    char searchTitle[100];

    while (1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Display All Books\n");
        printf("2. Search Book by Title\n");
        printf("3. Borrow a Book\n");
        printf("4. Return a Book\n");
        if (isAdmin) {
            printf("5. Add New Book\n");
            printf("6. Delete Book\n");
            printf("7. Update Book\n");
            printf("8. Exit\n");
        } else {
            printf("5. Exit\n");
        }

        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        if (!isAdmin && choice >= 5) {
            printf("👋 Exiting system...\n");
            exit(0);
        }

        switch (choice) {
            case 1:
                displayBooks(books, totalBooks);
                break;
            case 2:
                printf("Enter book title (or part of it): ");
                fgets(searchTitle, sizeof(searchTitle), stdin);
                searchTitle[strcspn(searchTitle, "\n")] = 0;
                searchBook(books, totalBooks, searchTitle);
                break;
            case 3:
                printf("Enter Book ID to borrow: ");
                scanf("%d", &bookID);
                borrowBook(books, totalBooks, bookID, filename, studentID);
                break;
            case 4:
                printf("Enter Book ID to return: ");
                scanf("%d", &bookID);
                returnBook(books, totalBooks, bookID, filename, studentID);
                break;
            case 5:
                if (isAdmin) addBook(books, &totalBooks, filename);
                else { printf("👋 Exiting system...\n"); exit(0); }
                break;
            case 6:
                if (isAdmin) {
                    printf("Enter Book ID to delete: ");
                    scanf("%d", &bookID);
                    deleteBook(books, &totalBooks, bookID, filename);
                }
                break;
            case 7:
                if (isAdmin) {
                    printf("Enter Book ID to update: ");
                    scanf("%d", &bookID);
                    getchar();
                    updateBook(books, totalBooks, bookID, filename);
                }
                break;
            case 8:
                if (isAdmin) {
                    printf("👋 Exiting system...\n");
                    exit(0);
                }
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
