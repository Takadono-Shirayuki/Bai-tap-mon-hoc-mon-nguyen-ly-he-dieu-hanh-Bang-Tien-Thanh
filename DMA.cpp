// Để chạy chương trình này, mở terminal và chạy lệnh sau:
// g++ -std=c++11 DMA.cpp -o DMA

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

class DMAController {
public:
    void transferData(std::vector<int>& source, std::vector<int>& destination, size_t size) {
        std::cout << "DMA Transfer Started..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Giả lập thời gian truyền dữ liệu
        
        for (size_t i = 0; i < size; ++i) {
            destination[i] = source[i];
        }

        std::cout << "DMA Transfer Completed." << std::endl;
    }
};

class PeripheralDevice {
public:
    std::vector<int> data;

    PeripheralDevice(size_t size) : data(size) {
        // Khởi tạo dữ liệu giả lập
        for (size_t i = 0; i < size; ++i) {
            data[i] = i + 1;
        }
    }

    void showData() const {
        for (int value : data) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    const size_t dataSize = 10;
    PeripheralDevice sourceDevice(dataSize);
    PeripheralDevice destinationDevice(dataSize);

    DMAController dmaController;

    std::cout << "Source Device Data: ";
    sourceDevice.showData();

    std::cout << "Destination Device Data Before DMA: ";
    destinationDevice.showData();

    // Thực hiện truyền dữ liệu bằng DMA
    dmaController.transferData(sourceDevice.data, destinationDevice.data, dataSize);

    std::cout << "Destination Device Data After DMA: ";
    destinationDevice.showData();

    return 0;
}
