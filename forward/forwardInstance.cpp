#include <iostream>
#include <string>

class CData{
public:
    CData() = delete;
    ~CData(){
        std::cout << "~CData()" << std::endl;
    }
    CData(const char*ch):data(ch){
        std::cout << "CData(const char*ch)" << std::endl;
    }

    CData(const std::string& str){
        std::cout << "CData(const std::string& str)" << std::endl;
    }

    CData(std::string&& str) : data(str){
        std::cout << "CData(const std::string&& str)" << std::endl;
    }

private:
    std::string data;
};

template<typename T>
CData* creator(T&& t){
    return new CData(t);
    //return new CData(std::forward<T>(t));
}

int main(){
    std::string str1 = "hello";
    std::string str2 = "world";

    CData* p1 = creator(str1);
    CData* p2 = creator(str1 + str2);

    return 0;
}