//
// Created by alexi on 5/15/2025.
//
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>



enum class SortBy {Name, Year};
enum class SortOrder {Ascending, Descending};
class Book
{
protected:
    int bookId;
    static int id;
    std::string name;
    std::string author;
    std::string genre;
    int releaseYear;
    bool availability= true;
    int availableCopies;
public:
    Book(std::string  name_, std::string  author_,
        std::string genre_, int releaseYear_, int availableCopies_);
    virtual ~Book()=default;
    int getId() const;
    std::string getAuthor() const;
    std::string getName() const { return name; }
    virtual void display(std::ostream& os) const;
    void borrow();
    void returnB(Book& book);
    int getYear() const { return releaseYear; }
    Book& operator++();
    Book& operator--();

    friend std::istream& operator>>(std::istream& in, Book& book);
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
    int getAvailableCopies() const;
};
int Book::id=0;
Book::Book(std::string name_, std::string  author_, std::string genre_, const int releaseYear_, int availableCopies_): bookId(++id), name(std::move(name_)), author(std::move(author_)), genre(std::move(genre_)), releaseYear(releaseYear_), availableCopies(availableCopies_){}
int Book::getId() const { return bookId; }
std::string Book::getAuthor() const { return author; }
Book& Book::operator++()
{
    ++availableCopies;
    availability=true;
    return *this;
}
Book& Book::operator--()
{
    if (availableCopies>0)
        --availableCopies;
    else
        availability=false;
    return *this;
}
int Book::getAvailableCopies() const { return availableCopies; }
void Book::display(std::ostream& os) const
{
    os << "Book ID: " << bookId
       << " | Title: " << name
       << " | Author: " << author
       << " | Genre: " << genre
       << " | Year: " << releaseYear
       << " | Available copies: " << availableCopies
       << std::endl;
}

std::istream& operator>>(std::istream& in, Book& book)
{
    std::cout << "Book Name: ";
    in >> book.name;
    std::cout << "\nAuthor: ";
    in>>book.author;
    std::cout << "Genre: ";
    in>>book.genre;
    std::cout << "Release Year: ";
    in>>book.releaseYear;
    std::cout << "Available Copies: ";
    in>>book.availableCopies;
    return in;
}
std::ostream& operator<<(std::ostream& os, const Book& book)
{
    book.display(os);
    return os;
}
void Book::borrow()
{
    if (availableCopies>0)
    {
        availableCopies--;
        std::cout<<"Book borrowed! \n";
    }
    else
    {
        availability=false;
        std::cout<<"No copies available! \n";
    }
}
void Book::returnB(Book& book)
{
    availableCopies++;
    availability=true;
    std::cout<<"Book returned! \n";
}

// class Author
// {
//     std::string name;
//     std::vector<std::shared_ptr<Book>> books;
// public:
//     explicit Author(std::string name_);
//     void addBook(const std::shared_ptr<Book>& book);
//     std::string getName() const;
// };
// Author::Author(std::string name_) : name(std::move(name_)){}
// void Author::addBook(const std::shared_ptr<Book>& book){ books.push_back(book);}
// std::string Author::getName() const {return name;}

class Poetry : virtual public Book
{
   const int poemCount;
public:
    Poetry(const std::string& name_, const std::string& author_, std::string genre_, int releaseYear_, int availableCopies_, int poemCount_);
    void display(std::ostream& os) const override;
};
Poetry::Poetry(const std::string& name_, const std::string& author_, std::string genre_, const int releaseYear_, int availableCopies_, const int poemCount_): Book(std::move(name_), author_, std::move(genre_), releaseYear_, availableCopies_), poemCount(poemCount_){}
void Poetry::display(std::ostream& os) const
{
    Book::display(os);
    os << " | Type: Poetry"
       << " | Poems: " << poemCount<< std::endl;
}
class Novel : virtual public Book
{
   const int pages;
    const int chapters;
public:
    Novel(std::string name_, const std::string& author_, std::string genre_, int releaseYear_, int availableCopies_, int pages_, int chapters_);
    void display(std::ostream& os) const override;
};
Novel::Novel(std::string name_, const std::string& author_, std::string genre_, const int releaseYear_, const int availableCopies_, const int pages_, const int chapters_): Book(std::move(name_), author_, std::move(genre_), releaseYear_, availableCopies_), pages(pages_), chapters(chapters_){}
void Novel::display(std::ostream& os) const
{
    Book::display(os);
    os << " | Type: Novel"
       << " | Pages: " << pages
       << " | Chapters: " << chapters << std::endl;
}

class Reader
{   protected:
    std::string firstName;
    std::string lastName;
    int age;
    const int readerId;
    static int totalReaders;

    std::vector<std::shared_ptr<Book>> readBooks;
    std::vector<std::shared_ptr<Book>> borrowedBooks;
public:
    Reader(std::string  firstName_, std::string  lastName_, int age_);
    Reader(const Reader& other);
    Reader& operator=(const Reader& other);
    int getId() const { return readerId; }
    virtual ~Reader()=default;
    virtual int membershipCost() const = 0;
    virtual int maxBooksAllowed()=0;
    int getMembershipCost() const { return membershipCost(); }
    virtual void display(std::ostream& os) const = 0;
    bool borrowBook(const std::shared_ptr<Book>&book);
    void returnBook(Book& book);
    virtual void addReader(std::istream& is);
    //virtual void readMore(std::istream& is)= 0;
    friend std::ostream& operator<<(std::ostream& os, const Reader& reader);
    friend std::istream& operator>>(std::istream& is, Reader& reader);
    std::string getFirstName() const {return firstName;};
    std::string getLastName() const {return lastName;};
    void displayBorrowedBooks() const;
};
int Reader::totalReaders=0;
Reader::Reader(std::string  firstName_, std::string  lastName_, const int age_): firstName(std::move(firstName_)), lastName(std::move(lastName_)), age(age_), readerId(++totalReaders){}
Reader::Reader(const Reader& other): firstName(other.firstName), lastName(other.lastName), age(other.age), readerId(other.readerId){}
Reader& Reader::operator=(const Reader& other)
{   // isi poate schimba numele/prenumele dar nu si varsta si id ul
    firstName=other.firstName;
    lastName=other.lastName;
    return *this;
}
bool Reader::borrowBook(const std::shared_ptr<Book>&book)
{
    if (borrowedBooks.size()==maxBooksAllowed())
    {
        std::cout<<"Borrow limit reached! \n";
        return false;
    }
    if (book->getAvailableCopies() <= 0 )
    {
        std::cout<<"Book not available! \n";
        return false;
    }

    borrowedBooks.push_back(book);
    --(*book);
    std::cout<<firstName<< " "<<lastName<<"borrowed"<<book->getName();
    return true;
}
void Reader::addReader(std::istream& is)
{
    std::cout<<"Enter reader details: \n";
    std::cout<<"First name: ";
    is>>firstName;
    std::cout<<"Last name: ";
    is>>lastName;
    std::cout<<"Age: ";
    is>>age;
}


void Reader::returnBook(Book& book)
{   bool found=false;
    for (auto i=borrowedBooks.begin(); i!=borrowedBooks.end(); ++i)
    {
        if ((*i)->getName()==book.getName())
        {   readBooks.push_back(*i);
            borrowedBooks.erase(i);
            ++book;
            std::cout<<firstName<< " "<<lastName<<"returned "<<book.getName();
            found=true;
            break;
        }
    }
    if (!found)
        std::cout<<"Book not found! \n";
}

void Reader::displayBorrowedBooks() const
{
    if (borrowedBooks.empty())
    {
        std::cout<<"No books borrowed! \n";
        return;
    }
    std::cout<<"Borrowed books: \n";
    for (const auto& book : borrowedBooks)
        book->display(std::cout);
}
std::ostream& operator<<(std::ostream& os, const Reader& reader)
{
    os<< "First name: "<<reader.firstName<<"\n Last name: "<<reader.lastName<<"\n Age:  "<<reader.age;
    return os;
}
std::istream& operator>>(std::istream& is, Reader& reader)
{   reader.addReader(is);
    return is;
}
class Student: public Reader
{
    int grade;
    public:
    Student(std::string  firstName_, std::string  lastName_, int age_, int grade_);
    int membershipCost() const override;
    int maxBooksAllowed() override;
    void addReader(std::istream& is) override;
    void display(std::ostream& os) const override;
//    void readMore(std::istream& is) override;

};
Student::Student(std::string firstName_, std::string lastName_, int age_, int grade_): Reader(std::move(firstName_), std::move(lastName_), age_), grade(grade_){}
int Student::membershipCost() const { return 10;}
int Student::maxBooksAllowed(){ return 5;}
void Student::display(std::ostream& os) const
{
    os<< "ID: "<<getId()<<" Student: "<<firstName<<" "<<lastName<<" grade: "<<grade<<" \n";
}
// void Student::readMore(std::istream& is)
// {
//     std::cout<<"Enter grade: ";
//     is>>grade;
// }
void Student::addReader(std::istream& is)
{   std::cout<<"student";
    Reader::addReader(is);

    std::cout<<"Enter grade: ";
    is>>grade;
}


class AveragePerson: public Reader
{
    public:
    AveragePerson(std::string  firstName_, std::string  lastName_, int age_);
    int membershipCost() const override;
    int maxBooksAllowed() override;
    void display(std::ostream& os) const override;
    void addReader(std::istream& is) override;
    //void readMore(std::istream& is) override;
};
AveragePerson::AveragePerson(std::string firstName_, std::string lastName_, int age_): Reader(std::move(firstName_), std::move(lastName_), age_){}
int AveragePerson::membershipCost() const
{
    if (age<18)
        return 0;
    if (age <=25)
        return 10;
    if (age <=60)
        return 20;
    return 10;
}
int AveragePerson::maxBooksAllowed(){ return 3;}
void AveragePerson::display(std::ostream& os) const
{
    os<<"ID: "<< getId()<< " Average person: "<<firstName<<" "<<lastName<<" age: "<<age<<" \n";
}
//void AveragePerson::readMore(std::istream& is) {};
void AveragePerson::addReader(std::istream& is)
{   std::cout<<"person";
    is>> *this;
    //readMore(is);
}

template<typename T>
class Database
{
    std::vector<std::shared_ptr<T>> items;
    public:
    void addItem(std::shared_ptr<T> item);
    bool removeItem(int itemId);
    void displayItems(std::ostream& os = std::cout);
    auto begin() {return items.begin();}
    auto end() {return items.end();}
    int size();
    std::shared_ptr<T> findId(int id);
    std::vector<std::shared_ptr<Book>> getItems(){return items;};
};
template<typename T>
void Database<T>::addItem(std::shared_ptr<T> item) {items.push_back(std::move(item));}

template <typename T>
bool Database<T>::removeItem(int itemId)
{
    for (auto i=items.begin(); i!=items.end(); ++i)
    {
        if ((*i)->getId()==itemId)
        {
            items.erase(i);
            return true;
        }
    }
    return false;
}

template <typename T>
void Database<T>::displayItems(std::ostream& os)
{
    for (auto i=items.begin(); i!=items.end(); ++i)
        (*i)->display(os);
}
template <typename T>
int Database<T>::size() {return items.size();}

template <typename T>
std::shared_ptr<T> Database<T>::findId(int id)
{
    for (auto& i : items)
        if (i->getId()==id)
            return i;

    return nullptr;
}


class Admin
{   const std::string username = "admin";
    const int password=1234;
    static int passwordTries;
    public:
    Admin()= default;
    bool login() const;
};

int Admin::passwordTries = 0;
bool Admin::login() const
{
    int input;
    std::cout<<"Enter password: ";
    std::cin>>input;
    if (input==password)
    {
        std::cout<<"Login successful! \n";
        return true;
    }
    std::cout<<"Login failed! \n";
    passwordTries++;
    if (passwordTries==3)
        std::cout<<"You have been banned! \n";
    return false;
}

class StudentBuilder
{
    std::string firstName;
    std::string lastName;
    int age{};
    int grade{};
public:
    StudentBuilder();
    StudentBuilder& addFirstName(const std::string& firstName);
    StudentBuilder& addLastName(const std::string& lastName);
    StudentBuilder& addAge(int age);
    StudentBuilder& addGrade(int grade);
    Student build() ;
};
StudentBuilder::StudentBuilder()= default;
StudentBuilder& StudentBuilder::addFirstName(const std::string& firstName)
{   this->firstName=firstName;
    return *this;
}
StudentBuilder& StudentBuilder::addLastName(const std::string& lastName)
{
    this->lastName=lastName;
    return *this;
}
StudentBuilder& StudentBuilder::addAge(const int age)
{
    this->age=age;
    return *this;
}
StudentBuilder& StudentBuilder::addGrade(const int grade)
{
    this->grade=grade;
    return *this;
}
Student StudentBuilder::build()
{    // if bla bla throw eroare
    return {firstName, lastName, age, grade};
}

class PersonBuilder
{
    std::string firstName;
    std::string lastName;
    int age{};
public:
    PersonBuilder ();
    PersonBuilder& addFirstName(const std::string& firstName);
    PersonBuilder& addLastName(const std::string& lastName);
    PersonBuilder& addAge(int age);
    AveragePerson build();
};
PersonBuilder::PersonBuilder()= default;
PersonBuilder& PersonBuilder::addFirstName(const std::string& firstName)
{   this->firstName=firstName;
    return *this;
}
PersonBuilder& PersonBuilder::addLastName(const std::string& lastName)
{
    this->lastName=lastName;
    return *this;
}
PersonBuilder& PersonBuilder::addAge(const int age)
{
    this->age=age;
    return *this;
}
AveragePerson PersonBuilder::build()
{    //if(firstName==""||lastName==""||age==0)
    // throw eroare
    return {firstName, lastName, age};
}
class FilterBooks
{
public:
    static std::vector<std::shared_ptr<Book>> filter(const std::vector<std::shared_ptr<Book>>& books, SortBy sortBy, SortOrder sortOrder);

};

std::vector<std::shared_ptr<Book>> FilterBooks::filter(const std::vector<std::shared_ptr<Book>>& books, SortBy sortBy, SortOrder sortOrder)
{
    std::vector<std::shared_ptr<Book>> temp=books;
    switch (sortBy)
    {
    case SortBy::Name:
        if (sortOrder==SortOrder::Ascending)
            std::sort(temp.begin(), temp.end(), [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b)
                {return a->getName()<b->getName();});
        else
            std::sort(temp.begin(), temp.end(), [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b)
                {return a->getName()>b->getName();});
        break;
    case SortBy::Year:
        if (sortOrder==SortOrder::Ascending)
            std::sort(temp.begin(), temp.end(), [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b)
                {return a->getYear()<b->getYear();});
        else
            std::sort(temp.begin(), temp.end(), [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b)
                {return a->getYear()>b->getYear();});
        break;
    }
    return temp;

}

// Singleton
class Library
{   Database<Book> bookDb;
    Database<Reader> readerDb;
    Admin admin;
    Library()=default;
    static Library* instance;
    public:
    static Library& getInstance();
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    void addData();
    void addReader(std::istream& is);
    void addBook(std::istream& is);
    void removeBook();
    void userMenu();
    void start();
    std::shared_ptr<Reader> loginUser();
    void userMenu2(const std::shared_ptr<Reader>& user);
    static int validateChoice(const std::string& text, int min, int max);


};
Library* Library::instance=nullptr;
Library& Library::getInstance()
{
    if (instance==nullptr)
        instance=new Library();
    return *instance;
}
int Library::validateChoice(const std::string& text, const int min, const int max)
{
    int choice;
    while (true)
    {
        std::cout<<text;
        std::cin>>choice;
        if (std::cin.fail() || choice<min || choice>max+1)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Invalid choice! Please choose a number between "<<min<<" and "<<max<<" ( or press "<< max+1<< " to exit)\n";
        }
        else if (choice==max+1)
            return -1;
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}


void Library::addData()
{
    StudentBuilder builderS;
    PersonBuilder builderP;

    Student s1 = builderS.addFirstName("Alex").addLastName("Popescu").addAge(19).addGrade(11).build();
    Student s2 = builderS.addFirstName("Maria").addLastName("Ionescu").addAge(20).addGrade(12).build();

    AveragePerson p1 = builderP.addFirstName("Ana").addLastName("Marinescu").addAge(45).build();
    AveragePerson p2 = builderP.addFirstName("Mihai").addLastName("Popa").addAge(70).build();

    readerDb.addItem(std::make_shared<Student>(s1));
    readerDb.addItem(std::make_shared<Student>(s2));
    readerDb.addItem(std::make_shared<AveragePerson>(p1));
    readerDb.addItem(std::make_shared<AveragePerson>(p2));

    for (const auto& reader : readerDb )
        reader->display(std::cout);

    bookDb.addItem(std::make_shared<Poetry>("Luceafarul", "Mihai Eminescu", "Poezie romantica", 1883, 3, 98));
    bookDb.addItem(std::make_shared<Novel>("Harry Potter and the Philosopher's Stone ", "J.K Rowling", "Fantasy", 1997, 5, 320, 17));

}
void Library::removeBook()
{
    const int id=validateChoice("Enter the ID of the book you want to delete: ", 1, bookDb.size());
    if (id==-1)
        return ;
    if (bookDb.removeItem(id)==false)
        std::cout<<"No book found with ID: "<<id<<std::endl;
    else
        std::cout<<"Book deleted! \n";
    for (const auto& i : bookDb )
        i->display(std::cout);
}

void Library::addReader(std::istream& is)
{
    int choice;
    std::shared_ptr<Reader> reader;
    while (true)
    {
        std::cout<<"Pick your membership:\n"
                   "1. Student \n"
                   "2. Normal Membership \n"
                   "3. Cancel\n";
        std::cin>>choice;
//chestie cautata pe google (Geeks for geeks)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Invalid choice! Please choose 1 or 2\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            reader = std::make_shared<Student>(" ", " ", 0, 0);
            reader->addReader(is);
            break;
        case 2:
            reader = std::make_shared<AveragePerson>(" ", " ", 0);
            reader->addReader(is);
            break;
        case 3:
            return;
        default:
            std::cout<<"Invalid choice! Please choose 1 or 2\n";
            continue;
        }
        break;
    }
    readerDb.addItem(reader);
    for (const auto& i : readerDb )
        i->display(std::cout);

}
void Library::addBook(std::istream& is)
{
    std::string name_, author_, genre_;
    int releaseYear_, availableCopies_;
    const int type = validateChoice("What do you want to add? (1 for Poetry and 2 for Novel)\n", 1, 2);
    if (type==-1)
        return;

    std::cout<<"Enter book details: \n";
    std::cout<<"Name: ";
    is>>name_;
    std::cout<<"Author: ";
    is>>author_;
    std::cout<<"Genre: ";
    is>>genre_;
    std::cout<<"Release Year: ";
    is>>releaseYear_;
    std::cout<<"Available Copies: ";
    is>>availableCopies_;
    if (type==1)
    {
        int poemCount_;
        std::cout<<"Enter poem count: ";
        is>>poemCount_;
        const auto book = std::make_shared<Poetry>(name_, author_, genre_, releaseYear_, availableCopies_, poemCount_);
        bookDb.addItem(book);
    }
    if (type==2)
    {
        int chapters_, pages_;
        std::cout<<"Enter chapters: ";
        is>>chapters_;
        std::cout<<"Enter pages: ";
        is>>pages_;
        const auto book = std::make_shared<Novel>(name_, author_, genre_, releaseYear_, availableCopies_, pages_, chapters_);
        bookDb.addItem(book);
    }

    for (const auto& i : bookDb )
        i->display(std::cout);

}
std::shared_ptr<Reader> Library::loginUser()
{
    std::cout<<"Enter your ID: ";
    int id;
    std::cin>>id;
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Invalid ID! \n";
        return nullptr;
    }
    auto user=readerDb.findId(id);
    if (user)
    {
        std::cout<<"Welcome"<<user->getFirstName()<<" "<<user->getLastName()<<"!\n";
        userMenu2(user);
        return user;
    }

    std::cout<<"No user found with ID: "<<id<<"\n";
    return nullptr;
}
void Library::userMenu()
{
    std::cout<<"=========================\n";
    std::cout<<"         USER MENU       \n";
    std::cout<<"=========================\n";
    std::cout<<"Do you already have a membership? (y/n)\n";
    std::string input;
    while (true)
    {
        std::cin>>input;
        if (input=="y" || input=="Y")
        {
            Reader* user=loginUser().get();
            if (user)
                userMenu2(std::shared_ptr<Reader>(user));
            else
                std::cout<<"Login failed. Please try again or create a membership.\n";
            break;
        }
        if (input=="n" || input=="N")
         {
             addReader(std::cin);
             break;
         }
        else
            std::cout<<"Invalid input! Please enter 'y' or 'n'. \n"; //fa chestia sa tot ceara input pana dai corect ca esti incapabil si nu poti din prima
    }
}

void Library::userMenu2(const std::shared_ptr<Reader>& user)
{
    while (true)
    {
        std::cout<<"=========================\n";
        std::cout<<"         USER MENU       \n";
        std::cout<<"=========================\n";
        std::cout<<"1. Display books\n";
        std::cout<<"2. Display borrowed books\n";
        std::cout<<"3. Borrow a book \n";
        std::cout<<"4. Return a book\n";
        std::cout<<"5. Exit\n";
        int choice=validateChoice("What do you want to do?", 1, 5);
        switch (choice)
        {
        case 1:
            bookDb.displayItems();
            break;
        case 2:
            user->displayBorrowedBooks();
            break;
        case 3:
                while (true)
                {
                    std::cout<<"Sort by: 1. None \n 2.Name\n 3.Realease Year\n";
                    const int sortChoice=validateChoice("Sort by: 1. None \n 2.Name\n 3.Realease Year\n" , 1, 3);
                    if (sortChoice==-1)
                        return;
                    if (sortChoice==1)
                        bookDb.displayItems();
                    else
                    { const SortBy by=(sortChoice==2)? SortBy::Name : SortBy::Year;
                        while (true)
                        {
                            std::cout<<"Press 'a' (for ascending), 'd' (for descending) or 'e' to exit.\n";
                            std::string input;
                            std::cin>>input;
                            if (input=="a" || input=="A")
                            {
                                std::vector<std::shared_ptr<Book>> sortedBooks=FilterBooks::filter(bookDb.getItems(), by, SortOrder::Ascending);
                                for (const auto& book : sortedBooks)
                                    book->display(std::cout);
                                break;
                            }
                            if (input=="d" || input=="D")
                            {
                                std::vector<std::shared_ptr<Book>> sortedBooks=FilterBooks::filter(bookDb.getItems(), by, SortOrder::Descending);
                                for (const auto& book : sortedBooks)
                                    book->display(std::cout);
                                break;
                            }
                            if (input=="e" || input=="E")
                            {
                                return;
                            }
                            else
                                std::cout<<"Invalid input! Please enter 'a', 'd' or 'e'. \n";
                        }
                    }

                if (const int bookId=validateChoice("Enter the ID of the book you want to borrow: ", 1, bookDb.size()); bookId!=-1)
                {
                    auto book=bookDb.findId(bookId);
                    user->borrowBook(std::make_shared<Book>(*book));
                }
                break;
            }
        case 4:
            {
                user->displayBorrowedBooks();
                int bookId=validateChoice("Enter ID of book to return: ",1, bookDb.size());
                if (bookId!=-1)
                {
                    auto book=bookDb.findId(bookId);
                    user->returnBook(*book);
                }
                break;
            }
        case 5:
            return;
        default:
            std::cout<<"Invalid input. Please choose a number between 1 and 5.\n";
            break;

        }
    }
}
void Library::start()
{
    std::cout<<"=========================\n";
    std::cout<<"Welcome to the library!\n";
    std::cout<<"=========================\n";
    std::cout<<"1. Login user\n";
    std::cout<<"2. Login admin\n";
    std::cout<<"3. Exit\n";
    int choice;
    std::cin>>choice;
    switch (choice)
    {
    case 1:
        userMenu();
        break;
    case 2:
        if (admin.login()==true)
        {
            while (true)
            {
                std::cout<<"1. Add book\n";
                std::cout<<"2. Remove book\n";
                std::cout<<"3. Display books\n";
                std::cout<<"4.Display readers\n";
                std::cout<<"5. Exit\n";
                std::cin>>choice;
                switch (choice)
                {
                case 1:
                    addBook(std::cin);
                    break;
                case 2:
                    removeBook();
                    break;
                case 3:
                    bookDb.displayItems();
                    break;
                case 4:
                    readerDb.displayItems();
                    break;
                case 5:
                    return;
                default:
                    std::cout<<"Invalid input! Please choose 1, 2, 3, 4 or 5\n";
                    break;

                }
            }
        }
        break;
    case 3:
        std::cout<<"Have a nice day!!\n";
        return;
    default: ;
    }
}



int main()
{  Library* library = &Library::getInstance();
    library->addData();
    library -> start();

    return 0;
}

