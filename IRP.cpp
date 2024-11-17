// Để chạy chương trình, mở terminal và gõ lệnh sau:
//   g++ -o IRP IRP.cpp -std=c++11 -pthread

#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

// Định nghĩa cấu trúc IRP
struct IRP {
    int requestId;
    std::string operation; // "read" hoặc "write"
    std::string data;
};

// Hàng đợi quản lý các IRP
std::queue<IRP> irpQueue; // Hàng đợi IRP
std::mutex queueMutex; // Mutex bảo vệ hàng đợi
std::condition_variable cv; // Điều kiện để thông báo khi có IRP mới

// Biến giả lập việc xử lý IRP
bool finish = false;

void processIrp() {
    while (!finish) {
        std::unique_lock<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
        cv.wait(lock, [] { return !irpQueue.empty(); }); // Chờ cho đến khi có IRP mới

        // Lấy IRP từ hàng đợi
        IRP irp = irpQueue.front(); // Lấy IRP đầu tiên
        irpQueue.pop(); // Xóa IRP đầu tiên

        lock.unlock(); // Mở khóa mutex

        // Xử lý IRP
        if (irp.operation == "read") {
            std::cout << "Processing READ IRP #" << irp.requestId << " with data: " << irp.data << std::endl;
        } else if (irp.operation == "write") {
            std::cout << "Processing WRITE IRP #" << irp.requestId << " with data: " << irp.data << std::endl;
        }

        // Giả lập thời gian xử lý
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Xử lý mất 1 giây
    }
}

void addIrpToQueue(int requestId, const std::string& operation, const std::string& data) {
    std::lock_guard<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
    irpQueue.push(IRP{requestId, operation, data}); // Thêm IRP vào hàng đợi
    cv.notify_one(); // Thông báo có IRP mới
}

int main() {
    std::thread processor(processIrp);

    // Thêm các IRP vào hàng đợi
    addIrpToQueue(1, "read", "Data to read");
    addIrpToQueue(2, "write", "Data to write");
    addIrpToQueue(3, "read", "Another data to read");

    // Giả lập thời gian chờ
    std::this_thread::sleep_for(std::chrono::seconds(5)); // Chờ 5 giây
    std::lock_guard<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
    finish = true; // Đánh dấu kết thúc
    cv.notify_one(); // Thông báo kết thúc

    std::cout << "All IRPs have been processed" << std::endl;
    // Đợi cho các IRP được xử lý
    processor.join(); // Chờ cho tiến trình xử lý IRP kết thúc
    return 0;
}
