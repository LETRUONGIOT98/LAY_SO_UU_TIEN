#include <ESP8266WiFi.h>
#include <espnow.h>

// Cấu trúc dữ liệu nhận
typedef struct struct_message {
  char ID[1]; // ID của ESP8266 gửi
  int value; 
} struct_message;

// Tạo một biến kiểu struct_message
struct_message incomingData;
struct_message outgoingData;
// Các biến lưu giá trị X theo ID
int X = 0;
int A = 0, B = 0, C = 0, D = 0, E = 0;
int A1 = 0;
int BL = 0;
int C1 = 0;
int D11 = 0;
int E1 = 0; 

// Địa chỉ MAC của các ESP8266
uint8_t broadcastAddresses[][6] = {
    {0xC4, 0xD8, 0xD5, 0x14, 0x38, 0x1D}, // MAC A
    {0xC4, 0xD8, 0xD5, 0x14, 0x36, 0xDC}, // MAC B
    {0xC4, 0xD8, 0xD5, 0x02, 0x29, 0x1D}, // MAC C
    {0xEC, 0x64, 0xC9, 0xCC, 0xD7, 0x23}, // MAC D
    {0x48, 0x3F, 0xDA, 0x3C, 0xBE, 0x08}  // MAC E
};

void OnDataRecv(uint8_t *mac, uint8_t *data, uint8_t len) {
  if (len == sizeof(incomingData)) { // Kiểm tra kích thước dữ liệu nhận được
    memcpy(&incomingData, data, sizeof(incomingData));
    
    // Gán giá trị X cho biến tương ứng dựa trên ID
    if (incomingData.ID[0] == 'A') {
      X++;
      A = X;
      A1 = 0;
    } else if (incomingData.ID[0] == 'B') {
      X++;
      B = X;
      BL = 0;
    } else if (incomingData.ID[0] == 'C') {
      X++;
      C = X;
      C1 = 0;
    } else if (incomingData.ID[0] == 'D') {
      X++;
      D = X;
      D11 = 0;
    } else if (incomingData.ID[0] == 'E') {
      X++;
      E = X;
      E1 = 0;
    } else if (incomingData.ID[0] == '1') {
      A1 = A;
    } else if (incomingData.ID[0] == '2') {
      BL = B;
    } else if (incomingData.ID[0] == '3') {
      C1 = C;
    } else if (incomingData.ID[0] == '4') {
      D11 = D;
    } else if (incomingData.ID[0] == '5') {
      E1 = E;
    }
  } else {
    Serial.println("Received data size mismatch");
  }
}

void setup() {
  Serial.begin(115200);
  // Khởi động WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Khởi tạo ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } 
  
  // Đăng ký callback khi nhận được dữ liệu
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Thêm các ESP8266 vào danh sách peer
  for (int i = 0; i < 5; i++) {
    esp_now_add_peer(broadcastAddresses[i], ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  }
}

void loop() {
  // Tạo chuỗi dữ liệu để gửi
  String gui = String(A) + "," + String(B) + "," + String(C) + "," + String(D) + "," + String(E);
  String guilai = String(A1) + "," + String(BL) + "," + String(C1) + "," + String(D11) + "," + String(E1);
  String data = gui + "," + guilai;
  
  Serial.println(data); // Hiển thị dữ liệu ra Serial

  // Gửi các biến kiểu int tới các ESP8266
  for (int i = 0; i < 5; i++) {
    if(i == 0)outgoingData.value = A; // Gán giá trị ngẫu nhiên từ 0 đến 99
    if(i == 1)outgoingData.value = B; // Gán giá trị ngẫu nhiên từ 0 đến 99
    if(i == 2)outgoingData.value = C; // Gán giá trị ngẫu nhiên từ 0 đến 99
    if(i == 3)outgoingData.value = D; // Gán giá trị ngẫu nhiên từ 0 đến 99
    if(i == 4)outgoingData.value = E; // Gán giá trị ngẫu nhiên từ 0 đến 99
    esp_now_send(broadcastAddresses[i], (uint8_t *) &outgoingData, sizeof(outgoingData));
  }
  delay(1000); // Delay 1 giây giữa các lần gửi
}
