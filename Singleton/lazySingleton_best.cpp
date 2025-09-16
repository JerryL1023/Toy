#include <iostream>
#include <string>

using namespace std;

class Singleton{
private:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(const Singleton& other) = default;
    Singleton& operator=(const Singleton& other) = default;

public:
    static Singleton& Getinstance(){
        static Singleton instance;
        return instance;
    }

    void print(){
        cout << "success" << endl;
    }
};

int main(){
    Singleton::Getinstance().print();

    return 0;
}