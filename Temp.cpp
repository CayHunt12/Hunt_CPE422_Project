#include <iostream>
#include <mqtt/async_client.h>  // Paho MQTT library
#include "AnalogIn.h"

 #define ADDRESS    "tcp://io.adafruit.com"
 #define CLIENTID   "Beagle1"
 #define TOPIC      "CayHunt12/feeds/weather.temperature"
 #define AUTHMETHOD "CayHunt12"
 #define AUTHTOKEN  "aio_KuOb45KdXFxgQEWMxE0dedqi8VGd"
 #define QOS        1
 #define TIMEOUT    10000L

int main() {
    AnalogIn temp_sensor(0);  // Assume temperature sensor on ADC0
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);
    mqtt::connect_options connOpts;
    connOpts.set_user_name(AIO_USERNAME);
    connOpts.set_password(AIO_KEY);
    
    try {
        std::cout << "Connecting to Adafruit IO..." << std::endl;
        client.connect(connOpts)->wait();
        std::cout << "Connected!" << std::endl;

        float temp_value = temp_sensor.read();  // Read raw ADC value
        float temperature = (temp_value / 4095.0) * 100.0;  // Example conversion to °C
        
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, std::to_string(temperature));
        pubmsg->set_qos(1);
        client.publish(pubmsg)->wait();
        
        std::cout << "Published Temperature: " << temperature << " °C" << std::endl;

        client.disconnect()->wait();
        std::cout << "Disconnected from AIO." << std::endl;
    } catch (const mqtt::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
