#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  int id;
  float buzzer_value;
} struct_message;

struct_message myData;

struct_message board1;
struct_message board2;

struct_message boardsStruct[2] = {board1, board2};

// callback function that will be executed when data is received
void gelen(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  //  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  memcpy(&myData, incomingData, sizeof(myData));
  boardsStruct[myData.id - 1].id = myData.id;
  boardsStruct[myData.id - 1].buzzer_value = myData.buzzer_value;
}

String message;

void setup() {

  Serial.begin(115200);
  Serial.println("basladim");
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(gelen);

  delay(2000);

}

void loop() {
  Serial.print("Board 1= ");
  Serial.print(boardsStruct[0].buzzer_value);
  Serial.print("  Board 2= ");
  Serial.println(boardsStruct[1].buzzer_value);

}
