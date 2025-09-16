#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>


class ThreadPool {
public:
    ThreadPool(int numsThreads) : stop(false){
        for(int i=0;i<numsThreads;i++){
            threads.emplace_back([this]{
                while(1){
                    std::unique_lock<std::mutex> lock(mtx);
                    condition.wait(lock,[this]{
                        return !tasks.empty() || stop;
                    });

                    if(stop && tasks.empty()){
                        return;
                    }

                    std::function<void()> task(std::move(tasks.front()));
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }

    ~ThreadPool(){
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }

        condition.notify_all();
        for(auto& t : threads){
            t.join();
        }
    }

    template<class F,class... Args>
    void enqueue(F &&f,Args&&... args){
        std::function<void()> task = 
            std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace(std::move(task));
        }

        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable condition;

    bool stop;
};

int main(){
    ThreadPool pool(4);
    std::mutex mtx;

    for(int i=0;i<10;i++){
        pool.enqueue([i,&mtx](){
            {
            std::lock_guard<std::mutex> lock(mtx);
            
            std:: cout << "========================" << std::endl;
            std::cout << "task:" << i << " is runing " << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "task: " << i << " is done " << std::endl;               
            std:: cout << "========================" << std::endl;

            }
        });
    }

    return 0;
}


/*
几个疑问：

1.为什么加入的thread中要用while循环，这里构造函数中的thread中加入的函数是立即执行还是等收到任务之后执行
2.为什么总是要判断stop，整体写完我发现除了析构函数，没有修改stop的地方
3.不论是lock_gurad还是unique_lock都需要在{}中执行吗
4.lambda语法的注意要点有哪些
5.条件变量的使用方法
6.函数对象的使用方法
7.详细解释一下enqueue代码及其原理，当传入函数进来是哪一个部分对应哪一个部分
8.线程池什么时候会被析构，stop只有在线程池整体被析构的时候才会进行，测试代码中，给任务的执行时间，其实只有从第一个任务添加开始到最后一个任务添加结束？因为添加动作结束之后貌似并不会等待，
线程池会马上析构，然后将stop标志置位true，
9.如何按顺序输出123 99次
10.如何把线程池改造为单例模式
11.解锁的时机为什么是取到任务之后
*/

