#pragma once

#include <iostream>

using namespace std;

class RefCount {
public:
    RefCount(int count) : _count(count) {}

    void addRef() { _count++; }
    void release() { _count--; }
    int getCount() { return _count; }

private:
    int _count;
};

template<typename T>
class sharePtr{
public:
    //1.构造和析构
    explicit sharePtr(T* ptr = nullptr) : _ptr(ptr) {
        if(_ptr){
            _cnt = new RefCount();
        }
        else{
            _cnt = nullptr;
        }
    }

    ~sharePtr(){
        release();
    }

    //2.所有权管理

    //复制构造函数
    sharePtr(const sharePtr& other){

    }

    //赋值运算符
    sharePtr& operator=(const sharePtr& other){

    }

    //移动构造函数
    sharePtr(cosnt sharePtr&& other){

    }

    //移动赋值运算符
    sharePtr&& operator=(const sharePtr& other){

    }

    //3.功能性函数
    T& operator* const { return *_ptr; }
    T* operator-> const { return _ptr; }
    T* get() const { return _ptr; }
    
private:
    void release_(){

    }

    T* _ptr;
    RefCount* _cnt;
};