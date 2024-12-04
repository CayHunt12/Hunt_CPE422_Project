#ifndef ANALOGIN_H
#define ANALOGIN_H

#include <string>
#include <fstream>
#include <sstream>

class AnalogIn {
private:
    int channel;  // Analog input channel (e.g., AIN0, AIN1)
    std::string filePath;  // Path to the corresponding AIN file in the sysfs

public:
    // Constructor: Initializes the analog input with a specific channel
    AnalogIn(int channel) : channel(channel) {
        std::ostringstream path;
        path << "/sys/bus/iio/devices/iio:device0/in_voltage" << channel << "_raw";
        filePath = path.str();
    }

    // Function to read the raw analog value
    int readAnalog() {
        std::ifstream file(filePath);
        int value = 0;
        if (file.is_open()) {
            file >> value;
            file.close();
        } else {
            throw std::runtime_error("Failed to open analog input file");
        }
        return value;
    }

    // Function to get a scaled analog value (for a 12-bit ADC)
    double getScaledValue(double maxVoltage = 1.8) {
        int rawValue = readAnalog();
        return (rawValue / 4095.0) * maxVoltage;  // Scale raw value to voltage
    }

    // Getter for the file path (for debugging purposes)
    std::string getFilePath() const {
        return filePath;
    }
};

#endif // ANALOGIN_H
