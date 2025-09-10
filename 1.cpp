#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;


class MenuItem {
protected:
    int id;
    string name;
    double price;
public:
    MenuItem(int i=0, string n="", double p=0.0) : id(i), name(n), price(p) {}
    virtual void display() {
        cout << setw(2) << id << "  " << setw(15) << name << "  Rs." << price << endl;
    }
    virtual double getPrice() { return price; }
    string getName() { return name; }
    int getId() { return id; }
};


class Snack : public MenuItem {
    string type; 
public:
    Snack(int i=0, string n="", double p=0.0, string t="Veg") : MenuItem(i,n,p), type(t) {}
    void display() override {
        cout << setw(2) << id << "  " << setw(15) << name << " (" << type << ")  Rs." << price << endl;
    }
};


class Combo : public MenuItem {
    vector<MenuItem*> items;
    double discountPercent;
public:
    Combo(int i, string n, double p, double disc) : MenuItem(i,n,p), discountPercent(disc) {}
    void addItem(MenuItem* m) { items.push_back(m); }

    void display() override {
        cout << setw(2) << id << "  " << setw(15) << name << " [Combo] Rs." << price;
        cout << " (Extra " << discountPercent << "% discount for eligible customers)" << endl;
    }

    double getPrice() override {
        return price; 
    }

    double getDiscountPercent() { return discountPercent; }
};


class Customer {
    string name;
    int age;
    bool isStudent;
public:
    Customer(string n, int a, bool s) : name(n), age(a), isStudent(s) {}

    double applyDiscount(double total, double comboDiscount) {
        double discount = 0.0;
        if(isStudent) discount = max(discount, comboDiscount); 
        if(age < 12) discount = max(discount, 15.0); 
        if(age > 60) discount = max(discount, 20.0);
        return total - (total * discount / 100.0);
    }
};


void saveBill(string custName, double total) {
    ofstream fout("bill.txt", ios::app);
    if(fout) {
        fout << "Customer: " << custName << " | Total Bill: Rs." << total << endl;
    }
    fout.close();
}


int main() {
 
    Snack s1(1,"Samosa",20,"Veg");
    Snack s2(2,"Sandwich",50,"Veg");
    Snack s3(3,"Burger",80,"Veg");
    Snack s4(4,"Cold Drink",40,"Veg");
    Snack s5(5,"Chips",30,"Veg");

  
    Combo c1(6,"Burger+Fries+Coke",150,10); 
    Combo c2(7,"Samosa+Tea",50,15);         
    Combo c3(8,"Sandwich+ColdCoffee",120,20); 
    vector<MenuItem*> menu = {&s1,&s2,&s3,&s4,&s5,&c1,&c2,&c3};
 cout << "\t⭐️ HATKE SNAKS ⭐️ " << endl;
 
    cout << "===== snaks menu =====" << endl;
    for(auto item : menu) {
        item->display();
    }

    
    string cname;
    int age;
    char student;
    cout << "\nEnter customer name: ";
    cin >> cname;
    cout << "Enter age: ";
    cin >> age;
    cout << "Is student? (y/n): ";
    cin >> student;
    bool isStu = (student=='y' || student=='Y');
    Customer cust(cname,age,isStu);

    // Order selection
    int choice;
    double total=0;
    vector<MenuItem*> ordered;
    cout << "\nEnter item IDs to order (0 to finish): ";
    while(true){
        cin >> choice;
        if(choice==0) break;
        bool found=false;
        for(auto item : menu){
            if(item->getId()==choice){
                ordered.push_back(item);
                total += item->getPrice();
                found=true;
                break;
            }
        }
        if(!found) cout << "Invalid ID!" << endl;
    }

  
    double finalTotal=total;
    for(auto item : ordered){
        Combo* combo=dynamic_cast<Combo*>(item);
        if(combo){
            finalTotal = cust.applyDiscount(finalTotal, combo->getDiscountPercent());
        }
        else{
            finalTotal = cust.applyDiscount(finalTotal, 0.0);
        }
    }

    cout << "\n===== BILL =====" << endl;
    for(auto item : ordered){
        cout << item->getName() << " - Rs." << item->getPrice() << endl;
    }
    cout << "Total: Rs." << total << endl;
    cout << "After Discounts: Rs." << finalTotal << endl;

    // Save bill to file
    saveBill(cname, finalTotal);

    return 0;
}
