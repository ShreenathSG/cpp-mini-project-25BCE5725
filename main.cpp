#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class Book {
private:
    int bookID;
    string title;
    string author;
    string status;
    int issuedTo;

public:
    Book() {}

    Book(int id, string t, string a) {
        bookID = id;
        title = t;
        author = a;
        status = "Available";
        issuedTo = -1;
    }

    int getID() { return bookID; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getStatus() { return status; }

    void display() {
        cout << "-------------------------------------\n";
        cout << "ID: " << bookID << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Status: " << status;
        if (status == "Issued")
            cout << " (RegNo: " << issuedTo << ")";
        cout << "\n-------------------------------------\n";
    }

    void issueBook(int regNo) {
        status = "Issued";
        issuedTo = regNo;
    }

    void returnBook() {
        status = "Available";
        issuedTo = -1;
    }

    void updateDetails(string t, string a) {
        title = t;
        author = a;
    }

    string toFile() {
        return to_string(bookID) + "," + title + "," + author + "," + status + "," + to_string(issuedTo);
    }

    void fromFile(string line) {
        stringstream ss(line);
        string temp;

        getline(ss, temp, ',');
        bookID = stoi(temp);

        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, status, ',');

        getline(ss, temp, ',');
        issuedTo = stoi(temp);
    }
};

class Library {
private:
    vector<Book> books;

public:
    Library() {
        load();
    }

    void loading() {
        cout << "Processing";
        for (int i = 0; i < 3; i++) {
            cout << ".";
            for (long j = 0; j < 100000000; j++);
        }
        cout << endl;
    }

    void pauseScreen() {
        cout << "\n-------------------------------------\n";
        cout << "Enter any number to return to menu: ";
        int temp;
        cin >> temp;
    }

    void addBook() {
        int id;
        string title, author;

        cout << "Enter Book ID: ";
        cin >> id;

        for (auto &b : books) {
            if (b.getID() == id) {
                cout << "Duplicate Book ID not allowed\n";
                return;
            }
        }

        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);

        books.push_back(Book(id, title, author));
        loading();
        save();

        cout << "Book added successfully\n";
    }

    void issueBook() {
        int id, reg;
        cout << "Enter Book ID: ";
        cin >> id;

        for (auto &b : books) {
            if (b.getID() == id) {
                if (b.getStatus() == "Issued") {
                    cout << "Book already issued\n";
                    return;
                }
                cout << "Enter Student RegNo: ";
                cin >> reg;
                b.issueBook(reg);
                loading();
                save();
                cout << "Book issued\n";
                return;
            }
        }
        cout << "Book not found\n";
    }

    void returnBook() {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;

        for (auto &b : books) {
            if (b.getID() == id) {
                if (b.getStatus() == "Available") {
                    cout << "Book is not issued\n";
                    return;
                }
                b.returnBook();
                loading();
                save();
                cout << "Book returned\n";
                return;
            }
        }
        cout << "Book not found\n";
    }

    void searchBook() {
        int choice;
        cout << "1. Search by ID\n2. Search by Title Keyword\n";
        cin >> choice;

        if (choice == 1) {
            int id;
            cout << "Enter Book ID: ";
            cin >> id;

            for (auto &b : books) {
                if (b.getID() == id) {
                    b.display();
                    return;
                }
            }
            cout << "Book not found\n";
        } else if (choice == 2) {
            string key;
            cin.ignore();
            cout << "Enter keyword: ";
            getline(cin, key);

            int found = 0;
            for (auto &b : books) {
                if (b.getTitle().find(key) != string::npos) {
                    b.display();
                    found = 1;
                }
            }
            if (!found)
                cout << "No matching books found\n";
        } else {
            cout << "Invalid option\n";
        }
    }

    void showIssuedBooks() {
        int found = 0;
        for (auto &b : books) {
            if (b.getStatus() == "Issued") {
                b.display();
                found = 1;
            }
        }
        if (!found)
            cout << "No issued books\n";
    }

    void showCount() {
        int avail = 0, issued = 0;

        for (auto &b : books) {
            if (b.getStatus() == "Available")
                avail++;
            else
                issued++;
        }

        cout << "Available Books: " << avail << endl;
        cout << "Issued Books: " << issued << endl;
    }

    void deleteBook() {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;

        for (int i = 0; i < books.size(); i++) {
            if (books[i].getID() == id) {
                books.erase(books.begin() + i);
                save();
                cout << "Book deleted\n";
                return;
            }
        }
        cout << "Book not found\n";
    }

    void updateBook() {
        int id;
        cout << "Enter Book ID: ";
        cin >> id;

        for (auto &b : books) {
            if (b.getID() == id) {
                string t, a;
                cin.ignore();
                cout << "Enter new title: ";
                getline(cin, t);
                cout << "Enter new author: ";
                getline(cin, a);

                b.updateDetails(t, a);
                save();
                cout << "Book updated\n";
                return;
            }
        }
        cout << "Book not found\n";
    }

    void showAllBooks() {
        if (books.empty()) {
            cout << "No books available\n";
            return;
        }

        for (auto &b : books) {
            b.display();
        }
    }

    void save() {
        ofstream file("library.txt");
        for (auto &b : books) {
            file << b.toFile() << endl;
        }
        file.close();
    }

    void load() {
        ifstream file("library.txt");
        string line;

        while (getline(file, line)) {
            Book b;
            b.fromFile(line);
            books.push_back(b);
        }
        file.close();
    }

    void menu() {
        int choice;

        while (1) {
            cout << "\n=====================================\n";
            cout << "        LIBRARY MANAGEMENT\n";
            cout << "=====================================\n";
            cout << "1. Add Book\n";
            cout << "2. Issue Book\n";
            cout << "3. Return Book\n";
            cout << "4. Search Book\n";
            cout << "5. Show Issued Books\n";
            cout << "6. Show Count\n";
            cout << "7. Delete Book\n";
            cout << "8. Update Book\n";
            cout << "9. Show All Books\n";
            cout << "10. Exit\n";
            cout << "=====================================\n";
            cout << "Enter your choice: ";

            cin >> choice;

            switch (choice) {
                case 1: addBook(); break;
                case 2: issueBook(); break;
                case 3: returnBook(); break;
                case 4: searchBook(); break;
                case 5: showIssuedBooks(); break;
                case 6: showCount(); break;
                case 7: deleteBook(); break;
                case 8: updateBook(); break;
                case 9: showAllBooks(); break;
                case 10: return;
                default: cout << "Invalid choice\n";
            }

            pauseScreen();
        }
    }
};

int main() {
    cout << "=====================================\n";
    cout << "   MINI LIBRARY MANAGEMENT SYSTEM\n";
    cout << "=====================================\n";

    Library lib;
    lib.menu();

    return 0;
}

