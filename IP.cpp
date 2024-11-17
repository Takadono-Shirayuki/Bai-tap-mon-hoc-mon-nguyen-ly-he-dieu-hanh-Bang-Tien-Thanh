// Để chạy chương trình này, mở terminal và chạy lệnh sau:
// g++ -std=c++11 -pthread IP.cpp -o IP

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

std::atomic<bool> interruptFlag(false);
std::vector<int> deviceData = {1, 2, 3, 4, 5}; // Dữ liệu từ thiết bị
std::atomic<size_t> dataIndex(0);

void interruptHandler() {
    // Giả lập nhận ngắt khi có yêu cầu dữ liệu
    std::this_thread::sleep_for(std::chrono::seconds(1));
    interruptFlag = true;
    std::cout << "Interrupt: Data request received.\n" << std::endl;
}

void pollData() {
    while (!interruptFlag); // Chờ đến khi có ngắt

    while (dataIndex < deviceData.size()) {
        std::cout << "Polling data: " << deviceData[dataIndex++] << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Giả lập thời gian nhận dữ liệu
    }

    std::cout << "All data received." << std::endl;
}

int main() {
    std::thread interruptThread(interruptHandler);
    std::thread pollingThread(pollData);

    interruptThread.join();
    pollingThread.join();

    return 0;
}
