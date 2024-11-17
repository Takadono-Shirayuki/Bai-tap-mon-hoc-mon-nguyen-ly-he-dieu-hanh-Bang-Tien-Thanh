// Để chạy chương trình, mở terminal và gõ lệnh sau:
//   g++ -o AIO AIO.cpp -std=c++11 -pthread

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

// Hàm mô phỏng hoạt động đọc không đồng bộ
void asyncRead(int requestId, std::promise<std::string> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Giả lập thời gian đọc dữ liệu
    prom.set_value("Data read from request " + std::to_string(requestId)); // Gán kết quả vào promise
}

// Hàm mô phỏng hoạt động ghi không đồng bộ
void asyncWrite(int requestId, std::promise<std::string> prom) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Giả lập thời gian ghi dữ liệu
    prom.set_value("Data written for request " + std::to_string(requestId)); // Gán kết quả vào promise
}

int main() {
    // Tạo các đối tượng promise và future cho các yêu cầu I/O
    std::promise<std::string> readPromise, writePromise; // Tạo promise
    std::future<std::string> readFuture = readPromise.get_future(); // Tạo future từ promise
    std::future<std::string> writeFuture = writePromise.get_future(); // Tạo future từ promise

    // Khởi động các thao tác I/O không đồng bộ
    std::thread readThread(asyncRead, 1, std::move(readPromise)); // Chuyển promise vào luồng
    std::thread writeThread(asyncWrite, 2, std::move(writePromise)); // Chuyển promise vào luồng

    // Xử lý các công việc khác trong khi chờ kết quả I/O
    std::cout << "Doing other work while waiting for I/O operations to complete..." << std::endl; // Thông báo
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Giả lập công việc khác 
 
    // Chờ kết quả của các thao tác I/O
    std::string readResult = readFuture.get(); // Lấy kết quả từ future
    std::string writeResult = writeFuture.get(); // Lấy kết quả từ future

    // Hiển thị kết quả
    std::cout << readResult << std::endl; 
    std::cout << writeResult << std::endl;

    // Kết thúc các luồng
    readThread.join();
    writeThread.join();

    return 0;
}
