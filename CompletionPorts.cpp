#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>

// Định Nghĩa Cấu Trúc Kết Quả I/O
struct IOResult {
    int requestId;
    std::string result;
};

// Hàng Đợi Completion Ports và Các Đối Tượng Đồng Bộ Hóa
std::queue<IOResult> completionQueue; // Hàng đợi kết quả I/O
std::mutex queueMutex; // Mutex bảo vệ hàng đợi
std::condition_variable cv; // Điều kiện để thông báo khi có kết quả I/O mới

// Biến giả lập việc xử lý kết quả I/O
bool finish = false;

// Hàm Xử Lý Yêu Cầu I/O Không Đồng Bộ
void asyncOperation(int requestId, std::promise<void> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Giả lập thời gian xử lý

    {
        std::lock_guard<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
        completionQueue.push({requestId, "Operation completed"}); // Thêm kết quả I/O vào hàng đợi
    }

    cv.notify_one(); // Thông báo có kết quả I/O mới
    prom.set_value(); // Đánh dấu yêu cầu đã hoàn thành
}

// Hàm Hoàn Thành Yêu Cầu
void completionPort() {
    while (!finish) {
        std::unique_lock<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
        cv.wait(lock, [] { return !completionQueue.empty() || finish; }); // Chờ cho đến khi có kết quả I/O mới

        // Kiểm tra nếu đã kết thúc và hàng đợi trống
        if (finish) {
            break; // Thoát khỏi vòng lặp
        }

        // Lấy kết quả I/O từ hàng đợi
        IOResult result = completionQueue.front(); // Lấy kết quả đầu tiên
        completionQueue.pop(); // Xóa kết quả đầu tiên

        lock.unlock(); // Mở khóa mutex

        // Xử lý kết quả I/O
        std::cout << "Request #" << result.requestId << ": " << result.result << std::endl;
    }
}

int main() {
    // Khởi động luồng completion port
    std::thread completionThread(completionPort);

    // Tạo các đối tượng promise và future cho các yêu cầu I/O
    std::promise<void> prom1, prom2; // Tạo promise
    std::future<void> fut1 = prom1.get_future(); // Tạo future từ promise
    std::future<void> fut2 = prom2.get_future(); // Tạo future từ promise

    // Khởi động các thao tác I/O không đồng bộ
    std::thread ioThread1(asyncOperation, 1, std::move(prom1)); // Chuyển promise vào luồng
    std::thread ioThread2(asyncOperation, 2, std::move(prom2)); // Chuyển promise vào luồng

    // Giả lập công việc khác
    std::cout << "Doing other work while waiting for I/O operations to complete..." << std::endl; // Thông báo

    // Chờ kết quả của các thao tác I/O không đồng bộ
    fut1.get(); // Chờ kết quả từ future
    fut2.get(); // Chờ kết quả từ future

    // Kết thúc các luồng
    ioThread1.join(); // Chờ cho luồng kết thúc
    ioThread2.join(); // Chờ cho luồng kết thúc

    // Kết thúc luồng completion port
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Chờ 1 giây 
    std::lock_guard<std::mutex> lock(queueMutex); // Lock mutex để thao tác với hàng đợi
    finish = true; // Đánh dấu kết thúc
    cv.notify_one(); // Thông báo kết thúc
    std::cout << "All I/O operations have been processed" << std::endl;

    // Đợi cho luồng completion port kết thúc
    completionThread.join(); // Chờ cho luồng kết thúc

    return 0;
}
