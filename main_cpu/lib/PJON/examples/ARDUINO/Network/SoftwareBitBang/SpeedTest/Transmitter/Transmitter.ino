
// Uncomment to use the mode you prefer (default SWBB_MODE 1)
// #define SWBB_MODE 1 // 1.95kB/s - 15625Bd
// #define SWBB_MODE 2 // 2.21kB/s - 17696Bd
// #define SWBB_MODE 3 // 2.94kB/s - 23529Bd
// #define SWBB_MODE 4 // 3.40kB/s - 27210Bd

#include <PJONSoftwareBitBang.h>

// Bus id definition
uint8_t bus_id[] = {0, 0, 0, 1};

// PJON object
PJONSoftwareBitBang bus(bus_id, 45);

uint8_t content[] = "01234567890123456789";

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection lost with device id ");
    Serial.println(bus.packets[data].content[0], DEC);
  }
};

void setup() {
  bus.strategy.set_pin(12);
  bus.set_error(error_handler);
  bus.begin();
  bus.send(44, content, 20);

  Serial.begin(115200);
};

void loop() {
  if(!bus.update()) // If all packets are delivered, send another
    bus.send(44, content, 20);
};
