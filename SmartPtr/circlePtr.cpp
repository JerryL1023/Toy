#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Son;

class Father {
public:
    string _name;
    shared_ptr<Son> _son;

    Father(const string& name):_name(name){
        cout << "Father " << _name  << ": Constructed" << endl;
    }

    ~Father() noexcept{
        cout << "Father " << _name  << ": Destroyed" << endl;
    }
};

class Son{
public:
    string _name;
    shared_ptr<Father> _father;

    Son(const string& name):_name(name){
        cout << "Son " << _name  << ": Constructed" << endl;
    }

    ~Son() noexcept{
        cout << "Son " << _name  << ": Destroyed" << endl;
    }
};

void test(){
    cout << "-----------------------Entering Scope----------------------------------" << endl;
    {
        shared_ptr<Father> p_father  = make_shared<Father>("papa");
        shared_ptr<Son> p_son = make_shared<Son>("Sonny");

        cout << "Father ref count:" << p_father.use_count() << endl;
        cout << "Son ref count:" << p_son.use_count() << endl;

        p_father->_son = p_son;
        p_son->_father = p_father;

        cout << "Father ref count:" << p_father.use_count() << endl;
        cout << "Son ref count:" << p_son.use_count() << endl;
    }
    cout << "-----------------------Ending Scope----------------------------------" << endl;
}

int main(){
    test();
    {
        shared_ptr<Son> son_ = make_shared<Son>("Normal Son");
        cout << "Normal Son ref count:" << son_.use_count() << endl;
    }


    return 0;
}