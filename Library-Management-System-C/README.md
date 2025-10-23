📚 Library Management System (C Project)
A simple Library Management System in C built for academic learning and practice.
This project demonstrates file handling, structures, menu-driven programming, and user authentication.
The system allows students to search, borrow, and return books, while admins can manage the library database and track transactions.
  
  ✨ Features
👨‍🎓 Student

1.Login with Student ID

2.View all available books

3.Search books by title

4.Borrow and return books

5.Transactions automatically logged

🛠️ Admin

1.Secure login with password (admin123)

2.Add new books to library

3.Delete books

4.Update book details

5.View all student transactions

📝 Tracking

1.Every borrow/return is logged in Transactions.txt with:

2.Student ID

3.Action (BORROW/RETURN)

4.Book ID, Title, Author

5.Timestamp

⚙️ Tech Details
Language: C

Data Storage: Books.txt (for book data), Transactions.txt (for logs)

Concepts Used:

File Handling (fopen, fscanf, fprintf)

Structures (struct Book)

String Handling (strcpy, strstr, etc.)

Menu-driven loops (switch, while)

To run:-

# Compile

gcc menu.c -o lms

-o menu

📌 Future Enhancements

Borrow limit per student

Fine calculation for late returns

Store data in binary files or a SQL database

GUI/Website interface

