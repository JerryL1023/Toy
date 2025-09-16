#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class ThreadPool{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    bool stop;
    std::mutex mtx;
    std::condition_variable condition;

public:

    ThreadPool(int nums) : stop(false) {
        for(int i = 0;i < nums;i++){
            threads.emplace_back([this]{
                while(1){
                    std::unique_lock<std::mutex> lock(mtx);
                    condition.wait(lock,[this]{
                        return !tasks.empty() || stop;
                    });

                    if(stop && tasks.empty()) return;

                    std::function<void()> task = tasks.front();
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

        for(auto& t : threads){
            t.join();
        }

        condition.notify_all();
    }

    template<class F,class... Args>
    void enqueue(F&& f,Args&&... args){
        std::function<void()> task = 
            std::bind(std::forward<F>(f),std::forward<Args>(args)...);

        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace(std::move(task));
        }

        condition.notify_one();
    }
};


int main(){
    ThreadPool threadPool(3);

    std::mutex mtx;

    for(int i=0;i<10;i++){
        threadPool.enqueue([i,&mtx]{
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "Now Process task:" << i << std::endl;
        });
    }

    return 0;
}