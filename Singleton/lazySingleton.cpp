#include <iostream>
#include <string>

#include <mutex>
// Lazy mode
// thread problem still exits
class Singleton{
private:
    class Deletor{
        public:
            Deletor() = default;
            ~Deletor(){
                if(instance){
                    delete instance;
                    Singleton::instance = nullptr;
                }
            }
    };

    static Singleton* instance;
    static Deletor deletor;
    static std::mutex m;

public:
    Singleton() = default;
    ~Singleton() = default;

    Singleton(Singleton& other) = delete;
    Singleton& operator=(Singleton& other) = delete;

    //加锁方案
    static Singleton* Getinstance(){
        if(!instance){
            std::lock_guard<std::mutex> lock(m);
            if(!instance)
                instance = new Singleton();
        }
        return instance;
    }

    void print(std::string str){
        std::cout << str << std::endl;
    }
};

Singleton* Singleton::instance = nullptr;
Singleton::Deletor Singleton::deletor;
std::mutex Singleton::m;

int main(){
    Singleton::Getinstance()->print("I love u");

    return 0;
}