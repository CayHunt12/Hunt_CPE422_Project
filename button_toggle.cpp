#include <iostream>
#include <unistd.h>
#include "GPIO.h"
#include "MQTTClient.h"

#define BUTTON_PIN "P9_12"  // Define the pin where the pushbutton is connected

#define IO_FEED "window_sensor"  // The feed name on Adafruit IO
#define BROKER "mqtt://io.adafruit.com"
#define CLIENTID "BBB_Window_Sensor_Client"
#define USERNAME "your_adafruit_io_username"
#define PASSWORD "your_adafruit_io_key"

GPIO button(BUTTON_PIN, GPIO::INPUT);   // Create GPIO object for pushbutton

MQTTClient client;

void messageReceived(MQTTClient* client, char* topic, char* message) {
    std::cout << "Received message: " << message << " on topic: " << topic << std::endl;
}

void setup_mqtt() {
    MQTTClient_create(&client, BROKER, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, messageReceived, NULL);
    MQTTClient_connect(client, USERNAME, PASSWORD);
}

void send_to_adafruit_io(const std::string& message) {
    MQTTClient_publish(client, IO_FEED, message.size(), (const void*)message.c_str(), 0, 0, NULL);
}

void loop() {
    bool prev_button_state = false;

    while (true) {
        bool button_state = button.read();

        // Check if the button state has changed
        if (button_state != prev_button_state) {
            prev_button_state = button_state;

            if (button_state) {
                // Simulate window opened (button pressed)
                send_to_adafruit_io("Window opened");
            } else {
                // Simulate window closed (button released)
                send_to_adafruit_io("Window closed");
            }
        }

        usleep(100000);  // Delay for 100ms to debounce
    }
}

int main() {
    std::cout << "Setting up MQTT connection..." << std::endl;
    setup_mqtt();

    std::cout << "Monitoring button press..." << std::endl;
    loop();

    return 0;
}
