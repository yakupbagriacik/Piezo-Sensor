#include <ESP8266WiFi.h>
#include <espnow.h>


uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xB2, 0xD1, 0xC9};

typedef struct struct_message {
  int id;
  float buzzer_value;
} struct_message;

struct_message myData;

float buzzer;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}

void loop() {
  delay(10);
  buzzer = analogRead(A0);
  myData.id = 1;
  myData.buzzer_value = buzzer;
  // Send message via ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

}
