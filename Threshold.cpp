#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <paho-mqtt/MQTTClient.h>
#include "LED.h"

#define ADDRESS    "tcp://io.adafruit.com"
 #define CLIENTID   "Beagle1"
 #define TOPIC      "CayHunt12/feeds/weather.temperature"
 #define AUTHMETHOD "CayHunt12"
 #define AUTHTOKEN  "aio_KuOb45KdXFxgQEWMxE0dedqi8VGd"
 #define QOS        1
 #define TIMEOUT    10000L

void on_message(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    LED* usr3 = static_cast<LED*>(context);
    std::string payload((char*)message->payload, message->payloadlen);

    try {
        int blinkCount = std::stoi(payload);
        std::cout << "Blink Count Received: " << blinkCount << std::endl;
        if (blinkCount > 0) {
            usr3->blink(blinkCount, 500); // 500 ms delay
        }
    } catch (const std::exception& e) {
        std::cerr << "Invalid message: " << payload << " - " << e.what() << std::endl;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
}

int main() {
    LED usr3(3);

    MQTTClient client;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    MQTTClient_setCallbacks(client, &usr3, nullptr, on_message, nullptr);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = AIO_USERNAME;
    conn_opts.password = AIO_KEY;

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        std::cerr << "Failed to connect to Adafruit IO" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected to Adafruit IO. Subscribing to feed..." << std::endl;
    MQTTClient_subscribe(client, AIO_FEED, QOS);

    while (true) {
        // Keep the loop alive for MQTT message processing.
        usleep(100000); // 100 ms
    }

    MQTTClient_disconnect(client, TIMEOUT);
    MQTTClient_destroy(&client);

    return 0;
}
