#include <iostream>
#include <memory>
#include <utility>

template<typename R,typename... Args>
struct ICallable {
    virtual R invoke(Args&& ... args) = 0;
    virtual ~ICallable() {}
};

template<typename T,typename R,typename... Args>
class ICallableImpl : public ICallable<R,Args ...> {
private:
    T callable;

public:
    ICallableImpl(T&& c) : callable(std::move(c)) {

    }

    R invoke(Args&&... args) override {
        return callable(std::forward<Args>(args)...);
    }
};

template<typename Signature>
class MyFunction;

template<typename R,typename... Args>
class MyFunction<R(Args...)>{
    std::unique_ptr<ICallable<R,Args...>> funcPtr;

public:
    template<typename T>
    MyFunction(T&& callable){
        funcPtr = std::make_unique<ICallableImpl><T, R,Args...>>(
            std::forward<T>(callable);
        )
    }

    R operator() (Args... args) const {
        reutnr funcPtr->invoke(std::forward<Args>(args)...);
    }
};


void func(){
    std::cout << "Hello World" << std::endl;
}


int main(){
    MyFunction<void()> f = func();
    f();
    return 0;
}