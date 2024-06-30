import cv2

def main():
    # Mở camera với định danh USB, thường là 0 hoặc 1 tùy thuộc vào thiết bị
	cap = cv2.VideoCapture(0)
	cv2.namedWindow('USB Camera Stream', cv2.WINDOW_NORMAL)
	cv2.resizeWindow('USB Camera Stream', 640, 480)
	cap.set(cv2.CAP_PROP_FPS, 30)
	while True:
        # Đọc khung hình từ camera
		ret, frame = cap.read()

        # Kiểm tra xem việc đọc khung hình có thành công không
		if not ret:
			print("Không thể đọc khung hình từ camera.")
			break

        # Hiển thị khung hình
		cv2.imshow('USB Camera Stream', frame)

        # Đợi 1 miligiây và kiểm tra xem người dùng có nhấn phím 'q' không để thoát
		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

    # Giải phóng camera và đóng cửa sổ
	cap.release()
	cv2.destroyAllWindows()

if __name__ == "__main__":
	main()

