#include <iostream>

using namespace std;

struct ObjectT{
    int m_val;

    ObjectT(int val) : m_val(val) {}
    ~ObjectT(){}

    void print(){
        cout << "Value:" << m_val << endl;
    }
};

template<typename T>
class uniquePtr{
public:
    //1.构造函数和析构函数
    explicit uniquePtr(T *ptr = nullptr) : m_ptr(ptr){}

    ~uniquePtr(){
        delete m_ptr;
    }

    //2.特殊操作限定
    //禁止拷贝构造和拷贝赋值
    uniquePtr(const uniquePtr&) = delete;
    uniquePtr& operator=(const uniquePtr&) = delete;

    //移动构造函数
    uniquePtr(uniquePtr&& other) noexcept {
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }

    //移动赋值运算符 
    uniquePtr& operator=(uniquePtr&& other) noexcept {
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

    operator bool() const { return m_ptr != nullptr; }

    T* get() { return m_ptr; }

private:
    T* m_ptr;
};
