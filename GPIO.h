// GPIO.h - A simple GPIO class for BeagleBone Black
#ifndef GPIO_H
#define GPIO_H

#include <string>
#include <iostream>

class GPIO {
public:
    enum Direction { INPUT, OUTPUT };
    
    GPIO(const std::string& pin, Direction dir);
    ~GPIO();

    void setDirection(Direction dir);
    bool read();
    void write(bool value);

private:
    std::string pin;
    Direction direction;
    bool value;
};

#endif // GPIO_H
