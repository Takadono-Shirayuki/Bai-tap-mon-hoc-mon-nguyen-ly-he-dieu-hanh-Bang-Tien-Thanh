// Để chạy chương trình này, hãy sử dụng lệnh sau:
// g++ interrupt.cpp -o interrupt -std=c++11 -pthread 
// ./interrupt

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> interruptFlag(false);

void interruptHandler() {
    while (!interruptFlag) {
        // Giả lập kiểm tra ngắt mỗi 100ms
        // Thực tế, CPU kiểm tra ngắt sau mỗi lệnh 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Interrupt: Counter reached the limit." << std::endl;
}

void counterTask(int limit) {
    std::cout << "Counter Task Started..." << std::endl;
    int counter = 0;
    while (counter < limit) {
        std::cout << "Counter: " << counter << std::endl;
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    // Kích hoạt ngắt
    interruptFlag = true;
}

int main() {
    int limit = 10;

    // Khởi động luồng xử lý ngắt
    std::thread interruptThread(interruptHandler);

    // Khởi động nhiệm vụ bộ đếm
    counterTask(limit);

    // Chờ luồng ngắt kết thúc
    interruptThread.join();

    return 0;
}
