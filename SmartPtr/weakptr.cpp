#include <iostream>

using namespace std;

//还存在很大的问题，要是问到了就g
class ControlBlock {
public:

    ControlBlock() :_strongCnt(1),_weakCnt(1){}
    
    void addStrongref() { ++_strongCnt; }
    int releaseStrongref() { return --_strongCnt; }
    int get_strong_count() const { return _strongCnt; }
    

    void addWeakref() { ++_weakCnt; }
    int releaseWeakref() { --_weakCnt; }    

private:
    int _strongCnt;
    int _weakCnt;
};

template<typename T> class weakPtr;

template<typename T>
class sharePtr{
private:
    T* _ptr;
    ControlBlock* _controlBlock;

    friend class weakPtr<T>;

public:
    explicit sharePtr(T* ptr = nullptr) : _ptr(ptr){}

    ~sharePtr(){
        if(_controlBlock && _controlBlock->releaseStrongref() == 0){
            delete _ptr;
            //为什么这里会减少弱引用计数
            if(_controlBlock->releaseWeakref() == 0){
                delete _controlBlock;
            }
        }
    }

    sharePtr(const sharePtr& other){
        _ptr = other._ptr;
        _controlBlock = other._controlBlock;
        if(_controlBlock){
            _controlBlock->addStrongref();
        }
    }

    T& operartor= sharePtr(const sharePtr& other){

    }

    sharePtr(const sharePtr&& other) noexcept{

    }

    T& operartor= sharePtr(const sharePtr&& other) noexcept{

    }

    int use_count(){
        return _controlBlock ? _controlBlock->get_strong_count();
    }
};

template<typename T>
class weakPtr{
private:
    T* _ptr;
    ControlBlock* _controlBlock;

    friend class sharePtr<T>;

public:
    weakPtr():_ptr(nullptr),_controlBlock(nullptr){}

    ~weakPtr(){
        if(_controlBlock && _controlBlock->releaseWeakref() == 0){
            delete _controlBlock;
        }
    }

    weakPtr(const sharePtr<T>& sp){
        _ptr = sp._ptr;
        _controlBlock = sp._controlBlock;
        if(_controlBlock){
            _controlBlock->addWeakref();
        }
    }

    weakPtr(const weakPtr<T>& other){
        _ptr = other._ptr;
        _controlBlock = other._controlBlock;
        if(_controlBlock){
            _controlBlock->addWeakref();
        }
    }

    weakPtr& operator= weakPtr(const weakPtr<T>& other){

    }

    weakPtr(const weakPtr<T>&& other) noexcept{
        _ptr = other._ptr;
        _controlBlock = other._controlBlock;
        _ptr = nullptr;
        _controlBlock = nullptr;
    }

    weakPtr& operator= weakPtr(const weakPtr<T>&& other){

    }

    bool expired() const{
        return !_contrlBlock || _controlBlock->get_strong_count == 0;
    }

    sharePtr<T> lock const{
        return expired() ? sharePtr<T>(nullptr) : sharePtr<T>(*this);
    }
};