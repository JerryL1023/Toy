#include <iostream>
#include <string>

using namespace std;

class Singleton {
private:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(const Singleton& other) = default;
    Singleton& operator=(const Singleton& other) = default;

    static Singleton instance;

public:
    static Singleton& Getinstance(){
        return instance;
    }

    void print(){
        cout << "Success" << endl;
    }
};

Singleton Singleton::instance;

int main(){
    Singleton::Getinstance().print();
    return 0;
}