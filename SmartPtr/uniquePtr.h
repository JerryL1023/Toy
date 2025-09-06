#include <iostream>

using namespace std;

struct ObjectT{
    int m_val;

    ObjectT(int val) : m_val(val) {
        cout << "Object(" << m_val << ") Constructed." << endl; 
    }

    ~ObjectT(){
        cout << "Object(" << m_val << ") Destroyed." << endl; 
    }

    void print(){
        cout << "Value:" << m_val << endl;
    }
};

template<typename T>
class uniquePtr{
public:
    //构造函数为什么要加explicit
    explicit uniquePtr(T *ptr = nullptr) : m_ptr(ptr){}

    ~uniquePtr(){
        delete m_ptr;
    }

    //禁止拷贝构造和拷贝赋值
    uniquePtr(const uniquePtr&) = delete;
    uniquePtr& operator=(const uniquePtr&) = delete;

    //移动构造函数
    uniquePtr(uniquePtr&& other) noexcept {
        cout << "Move Constructor Called!" << endl;

        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }

    //移动赋值运算符 ?
    uniquePtr& operator=(uniquePtr&& other) noexcept {
        cout << "Move Assignment Operator Called" << endl;

        if(this != &other){
           delete m_ptr;
           m_ptr = other.m_ptr;
           other.m_ptr = nullptr; 
        }

        return *this;
    }

    //3.指针行为模拟
    //重载*运算符
    T& operator*() const { return *m_ptr; }

    //重载->
    T* operator->() const { return m_ptr; }

    //
    operator bool() const { return m_ptr != nullptr; }

    T* get() { return m_ptr; }

private:
    T* m_ptr;
};
