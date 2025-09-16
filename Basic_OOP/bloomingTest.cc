#include <iostream>


class Distance{
private:
    int *data;

public:
    Distance():data(nullptr){}
    Distance(int *p):data(p){}
    Distance(int val):data(new int(val)){}

    ~Distance(){
        delete data;
        data = nullptr;
    }

    //如果拷贝构造函数不分配空间会导致double free
    Distance(const Distance& other){
        std::cout << "Copy construct" << std::endl;

        if(other.data){
            data  = new int(*other.data);
            //这里使用new之后也修改了内部的值，也分配了空间
        }
        else{
            data = nullptr;
        }
    }    

    //任何 = 重载都要确保不会自我赋值
    Distance& operator=(const Distance& other){
        if(this == &other){
            //要用地址进行比较
            return *this;
        }

        std::cout << "Copy assingment" << std::endl;
        delete data;
        if(other.data){
            data = new int(*other.data);
        }
        else{
            data = nullptr;
        }

        return *this;
    }

    Distance(Distance&& other) noexcept{
        std::cout << "Move construct" << std::endl;
        //这里是移动构造函数，data是未定义的状态，直接delete data;会出错
        data = other.data;
        other.data = nullptr;
    }

    Distance& operator=(Distance&& other) noexcept{
        std::cout << "Move assignment" << std::endl;
        if(&other != this){
            delete data;
            data = other.data;
            other.data = nullptr;
        }

        return *this;

    }

};

int main(){
    int *p1 = new int();
    //默认构造函数
    Distance dp(p1);

    //拷贝构造函数
    Distance dp1(dp);
    std::cout << "==================" << std::endl;

    Distance dp2(std::move(dp1));
    std::cout << "==================" << std::endl;

    //拷贝赋值运算符
    int *p2 = new int();
    Distance dp3(p2);
    dp3 = dp2;

    std::cout << "==================" << std::endl;

    //移动赋值运算符
    int *p3 = new int();
    Distance dp4(p3);
    dp4 = std::move(dp3);

    std::cout << "==================" << std::endl;

    return 0;
}