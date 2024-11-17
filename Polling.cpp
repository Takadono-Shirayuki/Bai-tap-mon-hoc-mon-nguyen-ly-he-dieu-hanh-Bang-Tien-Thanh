// Để chạy chương trình này, mở terminal và chạy lệnh sau:
// g++ -std=c++11 -pthread Polling.cpp -o Polling

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> sensorEvent(false);

void sensorPolling(int threshold) {
    int sensorValue = 0;
    while (!sensorEvent) {
        // Giả lập đọc giá trị cảm biến
        sensorValue = rand() % 75;

        std::cout << "Sensor Value: " << sensorValue << std::endl;

        // Kiểm tra nếu giá trị cảm biến đạt đến ngưỡng
        if (sensorValue >= threshold) {
            sensorEvent = true;
            std::cout << "Event Detected: Sensor Value reached " << sensorValue << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Chờ 500ms trước khi kiểm tra lại
    }
}

int main() {
    int threshold = 50; // Ngưỡng để kích hoạt sự kiện

    // Khởi động luồng polling cảm biến
    std::thread pollingThread(sensorPolling, threshold);

    // Chờ luồng polling kết thúc
    pollingThread.join();

    std::cout << "Polling finished." << std::endl;

    return 0;
}
