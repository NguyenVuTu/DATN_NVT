
import socket

def get_ipv4_address():
    try:
        # Tạo một socket IPv4 và kết nối đến một địa chỉ tùy ý (ở đây là Google DNS)
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.connect(("8.8.8.8", 80))

        # Lấy địa chỉ IP từ socket
        ip_address = sock.getsockname()[0]
        
        # Đóng kết nối
        sock.close()

        return ip_address
    except socket.error as e:
        print("Error:", e)
