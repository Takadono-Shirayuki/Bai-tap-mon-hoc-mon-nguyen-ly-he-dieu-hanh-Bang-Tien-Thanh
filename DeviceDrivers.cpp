#include <iostream>
#include <string>

// Lớp cơ sở cho tất cả các thiết bị
class Device {
public:
    virtual void initialize() = 0;
    virtual void readData() = 0;
    virtual void writeData(const std::string& data) = 0;
    virtual ~Device() {}
};

// Lớp đại diện cho ổ đĩa
class Disk : public Device {
public:
    void initialize() override {
        std::cout << "Initializing Disk Device" << std::endl;
    }

    void readData() override {
        std::cout << "Reading data from Disk" << std::endl;
    }

    void writeData(const std::string& data) override {
        std::cout << "Writing data to Disk: " << data << std::endl;
    }
};

// Lớp đại diện cho máy in
class Printer : public Device {
public:
    void initialize() override {
        std::cout << "Initializing Printer Device" << std::endl;
    }

    void readData() override {
        std::cout << "Reading data from Printer (Not applicable)" << std::endl;
    }

    void writeData(const std::string& data) override {
        std::cout << "Printing: " << data << std::endl;
    }
};

// Lớp đại diện cho trình điều khiển thiết bị
class DeviceDriver {
private:
    Device* device;

public:
    DeviceDriver(Device* dev) : device(dev) {}

    void initializeDevice() {
        device->initialize();
    }

    void performRead() {
        device->readData();
    }

    void performWrite(const std::string& data) {
        device->writeData(data);
    }
};

int main() {
    Disk myDisk;
    Printer myPrinter;

    DeviceDriver diskDriver(&myDisk);
    DeviceDriver printerDriver(&myPrinter);

    // Khởi tạo và sử dụng ổ đĩa
    std::cout << "Using Disk Device" << std::endl;
    diskDriver.initializeDevice();
    diskDriver.performRead();
    diskDriver.performWrite("Sample data for Disk");

    // Khởi tạo và sử dụng máy in
    std::cout << "\nUsing Printer Device" << std::endl;
    printerDriver.initializeDevice();
    printerDriver.performRead(); // Máy in không hỗ trợ đọc dữ liệu
    printerDriver.performWrite("Sample data for Printer");

    return 0;
}
