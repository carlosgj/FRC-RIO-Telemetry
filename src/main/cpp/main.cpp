#include <iostream>
#include <thread>
#include <chrono>

int main(){
    while(true){
        std::cout << "I'm running!\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}