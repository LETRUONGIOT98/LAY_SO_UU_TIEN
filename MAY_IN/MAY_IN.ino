/*
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

// Khai báo các chân RX, TX sử dụng SoftwareSerial cho Arduino Uno
#define TX_PIN 6
#define RX_PIN 5

SoftwareSerial mySerial(RX_PIN, TX_PIN);  // RX, TX
Adafruit_Thermal printer(&mySerial);

void setup() {
  mySerial.begin(9600);  // Khởi tạo giao tiếp với máy in
  printer.begin();  // Khởi tạo máy in
  
  // In văn bản với kích thước bình thường
  printer.setSize('S');  // Kích thước chữ nhỏ (S), bình thường (M), lớn (L)
  printer.println("Văn bản kích thước bình thường");
  
  // In văn bản với kích thước lớn
  printer.setSize('L');  // Kích thước lớn
  printer.println("Văn bản kích thước lớn");
  
  // In đậm
  printer.boldOn();
  printer.println("Văn bản in đậm");
  printer.boldOff();
  
  printer.feed(2);  // Đẩy giấy ra 2 dòng
}

void loop() {
  // Không thực hiện gì trong loop
}
*/
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

// Khai báo các chân RX, TX sử dụng SoftwareSerial cho Arduino Uno
#define TX_PIN 6
#define RX_PIN 5

SoftwareSerial mySerial(RX_PIN, TX_PIN);  // RX, TX
Adafruit_Thermal printer(&mySerial);

void setup() {
  mySerial.begin(9600);  // Khởi tạo giao tiếp với máy in
  printer.begin();  // Khởi tạo máy in

  // In với các kích thước khác nhau từ 1 đến 8
  for (int i = 1; i <= 8; i++) {
    printWithSize(i, i, "Kích thước in: ", i);
  }

  // Trở về kích thước mặc định
  setPrintSize(1, 1);
  printer.println("In văn bản với kích thước bình thường.");
  printer.feed(3);  // Đẩy giấy ra 3 dòng để kết thúc
}

void loop() {
  // Không thực hiện gì trong loop
}

// Hàm thay đổi kích thước chữ
void setPrintSize(int width, int height) {
  if (width < 1 || width > 8) width = 1;  // Giới hạn hệ số từ 1 đến 8
  if (height < 1 || height > 8) height = 1;
  
  int size = ((width - 1) << 4) | (height - 1);  // Chuyển đổi hệ số width và height
  printer.write(0x1D);  // Gửi lệnh ESC
  printer.write(0x21);  // Chọn chế độ in
  printer.write(size);  // Thiết lập hệ số kích thước
}

// Hàm in văn bản với kích thước chữ cụ thể
void printWithSize(int width, int height, const char* text, int value) {
  setPrintSize(width, height);  // Thiết lập kích thước chữ
  printer.print(text);  // In văn bản
  printer.println(value);  // In giá trị
  printer.feed(1);  // Đẩy giấy ra 1 dòng
}
