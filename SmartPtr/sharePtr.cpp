#include <iostream>
#include <utility>

using namespace std;

class RefCount {
public:
    RefCount(int count = 1) : _count(count) {}

    void addRef() { ++_count; }
    int release() { return --_count; }
    int getCount() { return _count; }

private:
    int _count;
};

template<typename T>
class sharePtr{
public:
    //1.构造和析构
    explicit sharePtr(T* ptr = nullptr) : _ptr(ptr) {
        //初始化计数器
        if(_ptr){
            _cnt = new RefCount();
        }
        else{
            _cnt = nullptr;
        }
    }

    ~sharePtr(){
        release_();
    }

    //2.所有权管理
    //复制构造函数
    sharePtr(const sharePtr& other){
        _ptr = other._ptr;
        _cnt = other._cnt;

        //如果计数器不为空就增加
        if(_cnt){
            _cnt->addRef();
        }   
    }

    //赋值运算符
    sharePtr& operator=(const sharePtr& other){
        if(this == &other){
            return *this;
        }
        release_();
        
        _ptr = other._ptr;
        _cnt = other._cnt;
        if(_cnt){
            _cnt->addRef();
        }
    }

    //移动构造函数
    sharePtr(sharePtr&& other) noexcept{
        _ptr = other._ptr;
        _cnt = other._cnt;
        other._ptr = nullptr;
        other._cnt = nullptr;
    }

    //移动赋值运算符
    sharePtr& operator=(sharePtr&& other) noexcept{
        if(this == &other){
            return *this;
        }
        release_();

        _ptr = other._ptr;
        _cnt = other._cnt;
        other._ptr = nullptr;
        other._cnt = nullptr;

        return *this;
    }

    //3.功能性函数
    T& operator*() const { return *_ptr; }
    T* operator->() const { return _ptr; }
    T* get() const { return _ptr; }
    
private:
    void release_(){
        if(_cnt && _cnt->release() == 0){
            delete _cnt;
            delete _ptr;
        }

        _cnt = nullptr;
        _ptr = nullptr;
    }

    T* _ptr;
    RefCount* _cnt;
};