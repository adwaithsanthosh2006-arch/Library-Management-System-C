#include <stdio.h>
#include <stdlib.h>

#define MAX_BOOKS 100

typedef struct {
    int bookID;
    char title[100];
    char author[100];
    int totalCopies;
    int availableCopies;
} Book;

int loadBooks(Book books[], const char *filename) {
    FILE *file = fopen("Books.txt", "r");
    if (!file) {
        printf("Error opening file!\n");
        return 0;
    }

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

int main() {
    Book books[MAX_BOOKS];
    int totalBooks = loadBooks(books, "Books.txt");

    printf("📚 Loaded %d books from library database!\n", totalBooks);

    // Print a sample book
    if (totalBooks > 0) {
        printf("First Book -> ID: %d | Title: %s | Author: %s | Copies: %d/%d\n",
               books[0].bookID,
               books[0].title,
               books[0].author,
               books[0].availableCopies,
               books[0].totalCopies);
    }

    return 0;
}
