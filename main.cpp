#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Drink;
class Menu;
class Order;
class Customer;
class Admin;

class User
{
    std::string name;

public:
    User(std::string name) : name(name) {}
    virtual ~User() {}
    virtual void login() = 0;

};

class Admin : public User
{
    const int password = 1234;
    static int passwordTries;


public:
    Admin(std::string name) : User(name) {}
    static void checkPasswordTries();
    void sortbyName();
    void sortbyValue();
    void login() override;
    void seeStats();
};

int Admin::passwordTries = 0;

class Customer : public User
{
    int money;

public:
    Drink* takeOrder();
    void login() override;


};


class Drink
{
    std::string name;
    double price;
    std::string size;


public:
    virtual std::string description() const = 0;
    virtual double cost() const = 0;
};

template <typename T>
    class CoffeBased: public Drink
    {
        protected:
        T gramsCoffe;
        std::string syrop;
        bool milk;
        bool extraShot;
        bool sugar;
        public:
       virtual void customize() = 0;


    };

class Espresso : public CoffeBased<int>
{

};

class Cappuccino : public CoffeBased<double>
{

};



class Menu
{
    std::vector<std::shared_ptr<Drink*> > drinks;
};

class Order
{   static int id;
    static double totalValue;

    std::vector<std::shared_ptr<Drink*> > drinks;
    std::vector<std::shared_ptr<Customer*> > customers;
public:

};


int main()
{
    return 0;
}