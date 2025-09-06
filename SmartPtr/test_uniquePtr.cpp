#include <utility> // for std::move
#include <string>
#include <iostream>

#include "uniquePtr.h"

using namespace std;
// 假设上面的 uniquePtr 类定义在这里，或者你 #include "uniquePtr.h"

void separator(const std::string& title) {
    cout << "\n--- " << title << " ---\n";
}

// 一个接收 uniquePtr 的函数，演示所有权转移
void takeOwnership(uniquePtr<ObjectT> ptr) {
    cout << "Inside takeOwnership function." << endl;
    if (ptr) {
        ptr->print();
    }
} // 函数结束，ptr 离开作用域，它所管理的对象被销毁

int main() {
    separator("1. Basic Scope Test");
    {
        uniquePtr<ObjectT> p1(new ObjectT(10));
        cout << "p1 created." << endl;
        if (p1) {
            p1->print();
            (*p1).m_val = 11;
            p1->print();
        }
    } // p1 在此离开作用域，ObjectT(11) 应该被销毁

    separator("2. Move Constructor Test");
    uniquePtr<ObjectT> p2(new ObjectT(20));
    cout << "Before move, p2 holds: " << p2.get() << endl;
    
    // 使用 std::move 触发移动构造函数
    uniquePtr<ObjectT> p3 = std::move(p2);
    
    cout << "After move, p2 holds: " << p2.get() << endl; // 应该为 0 (nullptr)
    cout << "After move, p3 holds: " << p3.get() << endl;
    if (p3) {
        p3->print();
    }
    
    separator("3. Move Assignment Operator Test");
    uniquePtr<ObjectT> p4(new ObjectT(40));
    // 使用 std::move 触发移动赋值运算符
    p3 = std::move(p4); // 在这里, p3 原来管理的对象 ObjectT(20) 应该被销毁
    
    cout << "After assignment, p4 holds: " << p4.get() << endl;
    cout << "After assignment, p3 holds: " << p3.get() << endl;
    if (p3) {
        p3->print();
    }
    
    separator("4. Function Ownership Transfer Test");
    uniquePtr<ObjectT> p5(new ObjectT(50));
    cout << "Before calling function, p5 holds: " << p5.get() << endl;
    takeOwnership(std::move(p5)); // 所有权转移给函数
    cout << "After calling function, p5 holds: " << p5.get() << endl;

    separator("5. Copy Test (should not compile)");
    // uniquePtr<ObjectT> p6(new ObjectT(60));
    // uniquePtr<ObjectT> p7 = p6; // 如果取消这行注释，会编译错误
    // takeOwnership(p6);         // 如果取消这行注释，也会编译错误

    separator("End of Main");
    // p3 在此离开作用_域，它管理的 ObjectT(40) 应该被销毁
    return 0;
}