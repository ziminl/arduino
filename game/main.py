import cv2
import numpy as np
import serial
import time

arduino = serial.Serial('COM3', 9600)  # Replace 'COM3' with your Arduino port
time.sleep(2)  # Give some time for the connection to establish

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to grab frame")
        break

    height, width, _ = frame.shape
    center_x, center_y = width // 2, height // 2
    roi_size = 50
    roi = frame[center_y - roi_size:center_y + roi_size, center_x - roi_size:center_x + roi_size]

    hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
    ## https://www.w3schools.com/colors/colors_rgb.asp
    lower_yellow = np.array([20, 100, 100])
    upper_yellow = np.array([30, 255, 255])

    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
    yellow_detected = cv2.countNonZero(mask) > (roi_size * roi_size * 0.5)

    if yellow_detected:
        arduino.write(b'C')

    cv2.rectangle(frame, (center_x - roi_size, center_y - roi_size), (center_x + roi_size, center_y + roi_size), (0, 255, 0), 2)
    cv2.imshow('Frame', frame)
    cv2.imshow('Mask', mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
arduino.close()
