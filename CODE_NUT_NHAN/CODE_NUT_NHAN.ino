#include <ESP8266WiFi.h>
#include <espnow.h>

// Địa chỉ MAC của các ESP8266 nhận ///08:F9:E0:7D:B6:7F 
uint8_t broadcastAddress[] = {0x08, 0xF9, 0xE0, 0x7D, 0xB6, 0x7F}; // Địa chỉ MAC của ESP chủ

// Cấu trúc dữ liệu gửi
typedef struct struct_message {
  char ID[1]; // ID của ESP8266 gửi 
  int value; 
} struct_message;

// Tạo một biến kiểu struct_message
struct_message myData;
int old = 1,old1 = 1;
#define NUT1 D1
#define NUT2 D2
 char espID ='K';
 #define led D4
void setup() {
  Serial.begin(115200);
  pinMode(D1, INPUT_PULLUP); // Chân nút nhấn
  pinMode(D2, INPUT_PULLUP); // Chân nút nhấn
  pinMode(D4, OUTPUT);

  // Khởi động WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // Khởi tạo ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  // Thêm ESP8266 chủ làm peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
// Hàm gửi dữ liệu thành công
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery Success");
    
  } else {
    Serial.println("Delivery Fail");
    //sendData();
  }
}


void loop() {
  int val = digitalRead(D1);
  int val1 = digitalRead(D2); 
  if (val != old) { // Nếu nút nhấn
    delay(50);
    if(digitalRead(D1) == 0){
     espID = 'E'; // ID của ESP8266 gửi, thay đổi cho từng ESP (A, vB, C, D, E)
     old = val;
    sendData();
    delay(500);
  }
  }
  old = val;
///////////////////////////////////////////////
  if (val1 != old1) { // Nếu nút nhấn
    delay(50);
    if(digitalRead(D2) == 0){
     espID = '1'; // ID của ESP8266 gửi, thay đổi cho từng ESP (1,2,3,4,5)
    sendData();
    delay(500);
  }
  }
  old1 = val1; 
  int valv = analogRead(A0);
  float vol = valv/1023.0;
  vol = vol * 6.0;
  //Serial.println(vol);
  if(vol <= 3.3){
    digitalWrite(led, HIGH);
  }
  else digitalWrite(led, LOW);
}

void sendData() { 
  myData.ID[0] = espID;  
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  //Serial.print("Sent ID: ");
  //Serial.println(myData.ID); 
}
