#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;

class Book{
    private:
        string title, author,ISBN;
        int numberOfCopies, year;
        float rating;

    public:

        Book(string t, string a, int n, string i, float r, int y): title(t), author(a), numberOfCopies(n), ISBN(i), rating(r), year(y){}

        string getTitle(){
            return title;
        }

        string getISBN(){
                return ISBN;
        }

        string getAuthor(){
                return author;
        }

        int getNumOfCopies(){
            return numberOfCopies;
        }

        float getRating(){
            return rating;
        }

        int getYear(){
            return year;
        }
        
        bool operator <(const Book& other) const{//compare the title
            string thisTitleLower = title;
            string otherTitleLower = other.title;
            transform(thisTitleLower.begin(), thisTitleLower.end(), thisTitleLower.begin(), ::tolower);
            transform(otherTitleLower.begin(), otherTitleLower.end(), otherTitleLower.begin(), ::tolower);
            return thisTitleLower < otherTitleLower;
        }

        void displayBook(){
            cout << "   Title: " << title << endl << "   Author: " << author << endl << "   ISBN: " << ISBN << endl << "   Number of Copies: " << numberOfCopies <<endl <<"   Rating: " << rating << endl << "   Published year: " << year << endl;
            return; 
        }
};

class Library{//binary search tree
    private:
        class Node{
            public:
                Book* data;
                Node* left = nullptr;
                Node* right = nullptr;
                Node(Book* a){
                    data = a;
                }
        };
        Node* root;

    public:

        Library(){
            root = nullptr;
        }


        Node* getRoot(){
            return root;
        }

        void setRoot(Node* a){
            root = a;
        }

        Node* addBook(Node* r, Book* a){//add the book according to the alphabetical order
            if (r == nullptr){
                Node* newNode = new Node(a);
                return newNode;
            }

            if (*a < *(r -> data)){
                r -> left = addBook(r -> left, a); 
            }
            else{
                r -> right = addBook(r -> right, a);
            }
            return r;//return the root of the tree
        }
        
        void search(string t){
            if (root == nullptr) {
                cout << "The library is empty." << endl;
                return;
            }

            Node* curr = root;
            while (curr != nullptr && curr->data->getTitle() != t) {
                if (curr->data->getTitle() < t) {
                    curr = curr->right;
                } else {
                    curr = curr->left;
                }
            }
            if (curr != nullptr){
                cout << "This book is available!" << endl;
                curr -> data->displayBook();
                return;
            }
            cout << "This book is unavailable in the library!" << endl;
        }
        
        bool remove(string t) {
            if (root == nullptr) {
                cout << "The library is empty." << endl;
                return false;
            }

            Node* curr = root;
            Node* prev = nullptr;
            while (curr != nullptr && curr->data->getTitle() != t) {
                prev = curr;
                if (curr->data->getTitle() < t) {
                    curr = curr->right;
                } else {
                    curr = curr->left;
                }
            }

            if (curr == nullptr) {
                cout << "Could not find the book." << endl;
                return false;
            }

            // Case 1: Node has no children
            if (curr->left == nullptr && curr->right == nullptr) {
                if (curr == root) {
                    root = nullptr;
                } else if (prev->right == curr) {
                    prev->right = nullptr;
                } else {
                    prev->left = nullptr;
                }
                delete curr;
            }
            // Case 2: Node has one child
            else if (curr->left == nullptr) {
                if (curr == root) {
                    root = curr -> right;
                } else if (prev -> right == curr) {
                    prev->right = curr -> right;
                } else {
                    prev->left = curr -> right;
                }
                delete curr;
            } else if (curr->right == nullptr) {
                if (curr == root) {
                    root = curr->left;
                } else if (prev->right == curr) {
                    prev->right = curr->left;
                } else {
                    prev->left = curr->left;
                }
                delete curr;
            }
            // Case 3: Node has two children
            else {
                Node* successor = curr->right;
                Node* successorParent = curr;
                while (successor->left != nullptr) {
                    successorParent = successor;
                    successor = successor->left;
                }

                if (curr == root) {
                    root = successor;
                } else if (prev->right == curr) {
                    prev->right = successor;
                } else {
                    prev->left = successor;
                }

                if (successorParent != curr) {
                    successorParent->left = successor->right;
                    successor->right = curr->right;
                }

                successor->left = curr->left;
                delete curr;
            }

            cout << "Deletion completed!" << endl;
        }
            

        void range(float a, float b, Node* root){

            if (root == nullptr){//if there is no book in the library, return nothing
                return;
            }
            Node* curr = root;//create a new Node that points to the root
            range(a,b, curr -> left);//recusive function for the left side of the tree
            if (curr -> data -> getRating() >= a && curr -> data -> getRating() <= b){ // check if the current book has the rating in the range of rating
                cout << curr -> data -> getTitle() << " " << curr -> data -> getRating() << endl;//print out the title and the rating
            }
            range(a,b, curr -> right);//recursive function for the right side of the tree
        }

        void displayOrdered(Node* r){//Display the books in alphabetical order
            if (r == nullptr) return; //if the root is null, return nothing
            displayOrdered(r -> left);//recursive function on the left side of the root
            cout << endl;;
            Book a = *(r -> data);//get the book data from the current node
            cout << endl;
            a.displayBook();//print out the book
            displayOrdered(r -> right);//recurive function on the right side of the root
            //since we us inorder traversal, the title is printed out in the alphabetical order
            return;
        }

        void deleteBooks(Node* r) {
                if (r == nullptr) {
                    return;
                }

                // Recursively delete books in the left and right subtrees
                deleteBooks(r->left);
                deleteBooks(r->right);

                // Delete the Book object stored in the node
                delete r -> data;

                // Set the left and right child pointers to null
                r->left = nullptr;
                r->right = nullptr;

                // Delete the node
                delete r;
        }

        void certain_year(Node* temp,int y){ //creating a temporary node and using the y value as the wanted year
            if (temp == nullptr) return; //checking to see if there are any books/nodes in the tree or not 
            //traversing through the tree recursively looking at the left side as long as the value is not null
                certain_year(temp->left,y);
            if(temp -> data -> getYear()==y){  //accessing the year by going through the data of the book and calling on the get year function and seeing if it is equal to the wanted year or not
                    temp->data->displayBook(); //if the year of the book is equal to the wanted year then this line prints the book and its attributes    
                    cout << endl;
            }
            //traversing through the tree recursively looking at the right side as long as the value is not null
                certain_year(temp->right,y);
        }


};

int main(){
    int T = 0;//number of the selection of the menu
    string title, author, ISBN;
    float rating;
    int noOfCopies, year;
    Library l;

    while (1){
        cout << "___ LIBRARY MANAGEMENT SYSTEM MENU ___\n1.Add a book\n2.Add a database of book (use .csv file)\n3.Delete a book\n4.Search for a book\n5.Dislay book in sorted order\n6.Rating range\n7.Book(s) published in a specific year\n8.Exit the library system\nChoose an option: ";
        string input;
        getline(cin,input);
        try{
            T = stoi(input);
            switch(T){
                case 1:{
                    cout << "Input the title: ";//get title
                    getline(cin, title);
                    cout << "Input the author: ";//get author
                    getline(cin, author);
                    cout << "Input the ISBN: ";//get isbn
                    getline(cin, ISBN);
                    cout << "Input the number of copies: ";//get no. of copies
                    cin >> noOfCopies;
                    cout << "Input the rating: ";//get rating
                    cin >> rating;
                    cout << "Input the published year ";//get published year
                    cin >> year;
                    cin.ignore();
                    Book* newBook = new Book(title, author, noOfCopies, ISBN, rating, year);//creating a new book with the attributes from above
                    l.setRoot(l.addBook(l.getRoot(), newBook));
                    cout << "The book is added into the library." << endl; //print statement wehn addition of book into library is succesful
                    break;
                }
                
                case 2:{
                    ifstream f;//create an ifstream obj to read the file
                    string file;
                    cout << "Input the file name: ";
                    getline(cin, file);
                    f.open(file);//read the file books.csv
                    if (f.is_open()) {
                        string line;
                        getline(f, line); // Skip the first line

                        while (getline(f, line)) {
                            char* strToSplit = strdup(line.c_str()); // Convert string to C-style string

                            char* items = strtok(strToSplit, ",");
                            int count = 0;

                            while (items != nullptr) {
                                if (count == 0) {
                                    title = items;
                                } else if (count == 1) {
                                    author = items;
                                } else if (count == 6) {
                                    ISBN = items;
                                } else if (count == 5) {
                                    noOfCopies = atoi(items); // Convert C-style string to int
                                }
                                else if (count == 3){
                                    rating = atof(items);
                                }
                                else if (count == 2){
                                    year = atoi(items);
                                }
                                items = strtok(nullptr, ",");                         
                                count++;
                            }

                            // Create a new Book object and add it to the library
                            Book* newBook = new Book(title, author, noOfCopies, ISBN,rating, year);
                            l.setRoot(l.addBook(l.getRoot(), newBook));

                            free(strToSplit); // Free the dynamically allocated memory
                    }

                    f.close(); // Close the file
                    } 
                    else {
                        cout << "Failed to open the file." << endl;
                    }
                    cout << endl << endl;
                    break;
                }

                case 3:{
                    string t;
                    cout << "Input the title: (Case sensitive)";//input the title you want to remove
                    getline(cin,t);
                    cout << endl;
                    l.remove(t);
                    cout << endl << endl;
                    break;
                }

                case 4:{
                    string t;
                    cout << "Input the title:";//input the title you want to search
                    getline(cin,t);
                    cout << endl;
                    l.search(t);
                    cout << endl << endl;
                    break;
                }
                
                case 5:{
                    cout << "Books displayed in alphabetical order:\n";
                    l.displayOrdered(l.getRoot());//display everybook in alphabetical order
                    cout << endl << endl;
                    break;
                }

                case 6:{
                    float low, high;
                    cout << "Input the lowest rating you want:" << endl;//input the lowerbound of the range
                    cin >> low;
                    cin.ignore(); // Clear the newline character from the input buffer
                    cout << "Input the highest rating you want:" << endl;//input the upperbound of the range
                    cin >> high;
                    cin.ignore(); // Clear the newline character from the input buffer
                    cout << endl << "The books that are in the rating range from " << low << " to " << high << ": " << endl;
                    cout << endl;
                    l.range(low,high,l.getRoot());//print out every book that has the rating in the range in alphabetical order
                    cout << endl << endl;
                    break;
                }

                case 7:{
                  int year; //creating a variable to hold the year wanted from the user
                    cout <<"input wanted year to see books from:" <<endl; //asking for the input for the wanted year from the user
                    cin>>year; //putting the wanted value for year into the variable for year
                    cin.ignore();
                    cout << endl;
                    l.certain_year(l.getRoot(),year); //calling the function for certain year with the root of the tree and the wanted year value 
                    break;
                }

                case 8:{
                    cout << "Thank you for using our system! Hope you have a great day!" << endl << endl;
                    l.deleteBooks(l.getRoot());//delete every book
                    exit(10);
                    break; 

                }

                default:{
                    continue;
                    break;
                }
                    
            }
        }catch (const std::invalid_argument& e) {
            cerr << "\nError: Invalid input. Please enter a valid option." << endl << endl;
            // Handle the error condition (e.g., display an error message, ask for input again)
        } catch (const std::out_of_range& e) {
            cerr << "\nError: Input out of range. Please enter a valid option." << endl << endl;
            // Handle the error condition (e.g., display an error message, ask for input again)
        }
    }
    return 0;

}



