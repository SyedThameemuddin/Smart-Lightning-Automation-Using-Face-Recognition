import cv2
import requests
import face_recognition
from cvzone.FaceDetectionModule import FaceDetector

# ESP32 IP address and endpoint for controlling the LED
ESP32_IP = 'http://172.31.99.11'  # Ensure this IP is correct

# Load the known face images and encode them
known_images = {
    "Thameem": "C:\\Users\\Syed Thameemuddin\\Pictures\\Thameem1.png",
    # "Najju": "C:\\Users\\Syed Thameemuddin\\Pictures\\Najju.jpg",
    # "Almas": "C:\\Users\\Syed Thameemuddin\\Pictures\\Almas.jpg"
}

# Encode the known faces
known_encodings = {}
for name, image_path in known_images.items():
    image = face_recognition.load_image_file(image_path)
    encoding = face_recognition.face_encodings(image)[0]
    known_encodings[name] = encoding

# Initialize the face detector using cvzone
detector = FaceDetector()

face_detection_enabled = False  # Global variable to keep track of face detection status

def send_led_command(state):
    try:
        response = requests.get(f'{ESP32_IP}/led?state={state}')
        print(f"Sent LED command: {state}, Response: {response.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"Error sending command: {e}")

def check_face_detection_status():
    global face_detection_enabled
    try:
        response = requests.get(f'{ESP32_IP}/status')
        if response.status_code == 200:
            status = response.text
            if "Face Detection is Enabled" in status:
                face_detection_enabled = True
            else:
                face_detection_enabled = False
            print(f"Face Detection Status: {face_detection_enabled}")
        else:
            print("Failed to get face detection status from ESP32")
    except requests.exceptions.RequestException as e:
        print(f"Error checking status: {e}")

def main():
    cap = cv2.VideoCapture(0)  # Use the appropriate camera index or URL

    while True:
        check_face_detection_status()

        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        frame = cv2.flip(frame, 1)

        if face_detection_enabled:
            frame, faces = detector.findFaces(frame)
            if faces:
                # Convert the frame to RGB for face_recognition
                rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                
                # Detect faces in the frame
                face_locations = face_recognition.face_locations(rgb_frame)
                face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)

                for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
                    # Compare the detected face with the known faces
                    matches = face_recognition.compare_faces(list(known_encodings.values()), face_encoding)
                    name = "Unknown"  # Default name if no match is found

                    if any(matches):  # If the face matches any known face
                        matched_idx = matches.index(True)
                        name = list(known_encodings.keys())[matched_idx]

                    # Draw a rectangle around the face
                    cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), thickness=2)
                    # Display the name above the rectangle
                    cv2.putText(frame, name, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (255, 255, 255), 2)

                send_led_command(1)  # Turn LED on
            else:
                send_led_command(0)  # Turn LED off
        else:
            print("Face detection is disabled, skipping face detection")

        cv2.imshow('Face Detection', frame)

        if cv2.waitKey(1) & 0xFF == 27:  # ESC key is represented by the ASCII value 27
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

