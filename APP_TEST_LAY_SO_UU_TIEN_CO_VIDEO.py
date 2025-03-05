import serial
import serial.tools.list_ports
import tkinter as tk
from tkinter import font
import cv2
from PIL import Image, ImageTk
import threading
import time
global video_playing
# Tìm tất cả các cổng COM hiện có và lọc cổng CH340
def find_ch340_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if 'CH340' in port.description:
            return port.device
    return None

# Tạo đối tượng Serial kết nối với cổng COM của CH340
def connect_to_serial_port(port, baud_rate=115200):
    try:
        ser = serial.Serial(port, baud_rate)
        return ser
    except serial.SerialException as e:
        print(f"Error connecting to {port}: {e}")
        return None

# Phát video trong cửa sổ Tkinter
def play_video():
    global video_playing
    cap = cv2.VideoCapture("video.mp4")  # Đường dẫn video

    while cap.isOpened() and video_playing:
        ret, frame = cap.read()
        if ret:
            # Chuyển đổi khung hình từ BGR sang RGB
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            # Chuyển đổi khung hình sang định dạng Tkinter
            img = Image.fromarray(frame)
            imgtk = ImageTk.PhotoImage(image=img)

            # Hiển thị khung hình lên nhãn video
            video_label.imgtk = imgtk
            video_label.config(image=imgtk)
        else:
            break

        # Dừng trong thời gian ngắn để hiển thị video theo thời gian thực
        time.sleep(0.03)

    cap.release()

# Dừng video
def stop_video():
    global video_playing
    video_playing = False

# Cập nhật giao diện
def update_display():
    global last_data_time
    current_time = time.time()

    if ser and ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        data = line.split(',')

        if len(data) == 10:  # Nhận được 10 giá trị
            # Hiển thị dữ liệu trong các ô
            label_A.config(text=data[0])
            label_B.config(text=data[1])
            label_C.config(text=data[2])
            label_D.config(text=data[3])
            label_E.config(text=data[4])

            # Hiển thị dữ liệu gọi lại trong các ô gọi lại
            recall_A.config(text=data[5])
            recall_B.config(text=data[6])
            recall_C.config(text=data[7])
            recall_D.config(text=data[8])
            recall_E.config(text=data[9])

            last_data_time = current_time  # Cập nhật thời gian nhận được dữ liệu
            stop_video()  # Dừng video nếu có dữ liệu

    # Kiểm tra nếu không có dữ liệu trong 30 giây
    if current_time - last_data_time > 30 and not video_playing:
        
        video_playing = True
        threading.Thread(target=play_video).start()  # Phát video trong luồng mới

    root.after(100, update_display)

# Tạo giao diện người dùng với Tkinter
root = tk.Tk()
root.title("Dữ liệu từ Cổng COM")

# Đặt kích thước của khung giao diện
frame_width = 1280
frame_height = 720
root.geometry(f"{frame_width}x{frame_height}")

# Tạo khung chính
frame = tk.Frame(root, bg="#f0f0f0")
frame.pack(fill=tk.BOTH, expand=True)

# Đặt kiểu chữ
label_font = font.Font(family="Arial", size=20, weight="bold")
label_font1 = font.Font(family="Arial", size=48, weight="bold")

# Hiển thị cổng COM đã kết nối
tk.Label(frame, text="ĐANG KẾT NỐI CỔNG:", font=label_font, bg="#f0f0f0").grid(row=0, column=0, columnspan=5, pady=10, sticky="nsew")

com_value_label = tk.Label(frame, text="No COM port", font=label_font, bg="#00fbff", relief="solid", width=30, height=2)
com_value_label.grid(row=1, column=0, columnspan=5, pady=5, sticky="nsew")

# Tạo các ô để hiển thị dữ liệu
header_labels = ["CỬA 1", "CỬA 2", "CỬA 3", "CỬA 4", "CỬA 5"]
for idx, text in enumerate(header_labels):
    tk.Label(frame, text=text, font=label_font, bg="#e0e0e0", relief="raised", width=12, height=3).grid(row=2, column=idx, padx=5, pady=5, sticky="nsew")

# Tạo các ô hiển thị dữ liệu
label_A = tk.Label(frame, text="0", font=label_font1, bg="#00ff00", width=12, height=3, borderwidth=2, relief="solid")
label_A.grid(row=3, column=0, padx=5, pady=5, sticky="nsew")

label_B = tk.Label(frame, text="0", font=label_font1, bg="#00ff00", width=12, height=3, borderwidth=2, relief="solid")
label_B.grid(row=3, column=1, padx=5, pady=5, sticky="nsew")

label_C = tk.Label(frame, text="0", font=label_font1, bg="#00ff00", width=12, height=3, borderwidth=2, relief="solid")
label_C.grid(row=3, column=2, padx=5, pady=5, sticky="nsew")

label_D = tk.Label(frame, text="0", font=label_font1, bg="#00ff00", width=12, height=3, borderwidth=2, relief="solid")
label_D.grid(row=3, column=3, padx=5, pady=5, sticky="nsew")

label_E = tk.Label(frame, text="0", font=label_font1, bg="#00ff00", width=12, height=3, borderwidth=2, relief="solid")
label_E.grid(row=3, column=4, padx=5, pady=5, sticky="nsew")

# Thêm các ô Gọi lại dưới mỗi cửa
recall_labels = ["Gọi lại Cửa 1", "Gọi lại Cửa 2", "Gọi lại Cửa 3", "Gọi lại Cửa 4", "Gọi lại Cửa 5"]
for idx, text in enumerate(recall_labels):
    tk.Label(frame, text=text, font=label_font, bg="#e0e0e0", relief="raised", width=12, height=3).grid(row=4, column=idx, padx=5, pady=5, sticky="nsew")

# Tạo các ô hiển thị dữ liệu Gọi lại
recall_A = tk.Label(frame, text="0", font=label_font1, bg="#ff0000", width=12, height=3, borderwidth=2, relief="solid")
recall_A.grid(row=5, column=0, padx=5, pady=5, sticky="nsew")

recall_B = tk.Label(frame, text="0", font=label_font1, bg="#ff0000", width=12, height=3, borderwidth=2, relief="solid")
recall_B.grid(row=5, column=1, padx=5, pady=5, sticky="nsew")

recall_C = tk.Label(frame, text="0", font=label_font1, bg="#ff0000", width=12, height=3, borderwidth=2, relief="solid")
recall_C.grid(row=5, column=2, padx=5, pady=5, sticky="nsew")

recall_D = tk.Label(frame, text="0", font=label_font1, bg="#ff0000", width=12, height=3, borderwidth=2, relief="solid")
recall_D.grid(row=5, column=3, padx=5, pady=5, sticky="nsew")

recall_E = tk.Label(frame, text="0", font=label_font1, bg="#ff0000", width=12, height=3, borderwidth=2, relief="solid")
recall_E.grid(row=5, column=4, padx=5, pady=5, sticky="nsew")

# Tạo nhãn để hiển thị video
video_label = tk.Label(frame)
video_label.grid(row=6, column=0, columnspan=5)

# Cấu hình hàng và cột để mở rộng đều
frame.grid_rowconfigure(2, weight=1)
frame.grid_rowconfigure(3, weight=1)
frame.grid_rowconfigure(4, weight=1)
frame.grid_rowconfigure(5, weight=1)
frame.grid_rowconfigure(6, weight=1)
for i in range(5):
    frame.grid_columnconfigure(i, weight=1)

# Tìm và kết nối với cổng COM của CH340
port = find_ch340_port()
ser = None
last_data_time = time.time()  # Thời gian lần cuối nhận được dữ liệu
video_playing = False  # Trạng thái video

if port:
    ser = connect_to_serial_port(port)
    if ser:
        com_value_label.config(text=f"{port}")
        print(f"Connected to {port}")
        update_display()
    else:
        com_value_label.config(text="Failed to connect")
        print("Failed to connect to the CH340 port.")
else:
    com_value_label.config(text="No CH340 port found")
    print("No CH340 port found.")

root.mainloop()
