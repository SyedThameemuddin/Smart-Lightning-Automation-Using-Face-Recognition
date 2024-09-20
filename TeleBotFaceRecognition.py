import cv2
import requests
import face_recognition
from cvzone.FaceDetectionModule import FaceDetector

# ESP32 IP address
ESP32_IP = 'http://172.31.99.11'  # Ensure this IP is correct

# Load the known face images and encode them
known_images = {
    "Thameem": "C:\\Users\\Syed Thameemuddin\\Pictures\\Thameem1.png",
    # Add more known faces as needed
}

# Encode the known faces
known_encodings = {}
for name, image_path in known_images.items():
    image = face_recognition.load_image_file(image_path)
    encoding = face_recognition.face_encodings(image)[0]
    known_encodings[name] = encoding

detector = FaceDetector()

def check_for_faces(frame):
    faces, _ = detector.findFaces(frame)
    for face in faces:
        x, y, w, h = face['bbox']
        face_image = frame[y:y+h, x:x+w]
        face_encoding = face_recognition.face_encodings(face_image)
        if face_encoding:
            matches = face_recognition.compare_faces(list(known_encodings.values()), face_encoding[0])
            if True in matches:
                return True
    return False

def send_command(command):
    url = f"{ESP32_IP}/telegram?command={command}"
    response = requests.get(url)
    print(response.text)

def main():
    cap = cv2.VideoCapture(0)  # Use the appropriate camera index or URL

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Check for faces in the frame
        if check_for_faces(frame):
            send_command("led_on")  # Turn LED on if a face is detected
        else:
            send_command("led_off")  # Turn LED off if no face is detected

        cv2.imshow("Webcam", frame)

        key = cv2.waitKey(1) & 0xFF
        if key == ord('f'):  # Press 'f' to enable face detection
            send_command("enable_face")
        elif key == ord('d'):  # Press 'd' to disable face detection
            send_command("disable_face")
        elif key == ord('m'):  # Press 'm' to enable motion detection
            send_command("enable_ir")
        elif key == ord('n'):  # Press 'n' to disable motion detection
            send_command("disable_ir")
        elif key == ord('b'):  # Press 'b' to enable both
            send_command("enable_both")
        elif key == ord('c'):  # Press 'c' to disable both
            send_command("disable_both")
        elif key == ord('q'):  # Press 'q' to quit
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
