#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>


std::mutex mtx;
std::condition_variable condition;
/*
设置一个轮次变量控制顺序，当与当前线程id相符时才进行输出，用好条件变量即可
*/
int currentRound = 1;
void orderPrint(int threadId,int num){
    for(int i=0;i<100;i++){
        std::unique_lock<std::mutex> lock(mtx);
        condition.wait(lock,[threadId,num]{
            return threadId == currentRound;
        });

        std::cout << num;

        if(num == 3){
            std::cout << std::endl;
            currentRound = 1;
        }
        else{
            currentRound++;
        }

        condition.notify_all();
    }
}

int main(){
    std::thread t1(orderPrint,1,1);
    std::thread t2(orderPrint,2,2);
    std::thread t3(orderPrint,3,3);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}