
# Smart Lighting Automation

## Project Overview

**Smart Lighting Automation** is an advanced IoT-based system that intelligently controls lighting using an ESP32 microcontroller. This project is designed to optimize energy efficiency and improve user convenience by automating lighting control based on real-time inputs such as motion and facial recognition. It also allows manual control through a Wi-Fi-based web interface.

## Features

- **Motion Detection**: Automatically turns the LED on or off when motion is detected, ensuring efficient use of lights.
- **Manual Wi-Fi Control**: Control the lighting remotely through a web interface from any phone or PC.
- **Enable/Disable Features**: Users can enable or disable motion detection and manual control for a more customized experience.
- **Facial Recognition**: When face detection is enabled, the system records video, detects faces, and turns the LED on when a face is recognized. The LED turns off when no face is detected.
- **Worker Identification**: The system prints the names of recognized workers, providing useful identification in office spaces.

## How It Works

1. **Motion Detection**: The system uses an IR sensor to detect motion and control the LED accordingly.
2. **Manual Control**: Users can manually turn the LED on or off via a web interface connected to the ESP32 through Wi-Fi.
3. **Face Detection & Recognition**: When enabled, the camera captures video and uses facial recognition to control the LED. The system identifies workers and displays their names, or "Unknown" if the person is not recognized.
4. **Video Recording**: The system records and stores video data locally whenever face detection is enabled, providing a record of activity.

## Project Use Case

This project is particularly useful in office environments where it automates lighting based on presence and identity. When a person enters a room or cabin, the system turns the lights on and identifies them if their face is recognized. If the room is empty, the lights automatically turn off, saving energy. The added feature of face detection and recording makes the system ideal for areas where monitoring is needed.

## Technology Stack

- **ESP32 Microcontroller**: The brain of the project, controlling sensors, LED, and web interface.
- **IR Sensor**: Detects motion to automate lighting.
- **OpenCV with Python**: Used for facial detection and recognition.
- **Face Recognition Library**: For recognizing and identifying faces.
- **Web Interface**: Built to control the LED and system features remotely via Wi-Fi.
- **Buzzer**: Provides audible feedback when the LED is activated.

## Upcoming Features

- **Telegram Bot/Smartphone App**: Future iterations will include remote control through a Telegram bot or smartphone app for greater flexibility.

## Installation & Setup

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/smart-lighting-automation.git
    ```

2. Follow the steps to set up your ESP32 microcontroller and sensors. Refer to the `circuit_diagram.png` file in the repository for hardware connections.

3. Install the required Python libraries for facial recognition:
    ```bash
    pip install opencv-python face_recognition requests
    ```

4. Upload the ESP32 code to your microcontroller and set the correct IP address in the Python code.

5. Run the Python script for face recognition and manual control.

6. Access the web interface via the IP address of your ESP32 to manually control the system.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please follow these steps:

1. **Fork the Repository:**
   - Click the "Fork" button at the top right of the repository page to create your copy of the repository.

2. **Clone Your Fork:**
   - Clone your forked repository to your local machine using:
     ```bash
     git clone https://github.com/SyedThameemuddin/Smart-Lighting-Automation.git
     ```

3. **Create a New Branch:**
   - Create a new branch for your feature or fix:
     ```bash
     git checkout -b feature/my-feature
     ```

4. **Make Your Changes:**
   - Make your changes in the codebase and commit them:
     ```bash
     git commit -m "Add my feature"
     ```

5. **Push Your Changes:**
   - Push your changes to your fork:
     ```bash
     git push origin feature/my-feature
     ```

6. **Open a Pull Request:**
   - Go to the original repository and click on "Pull Requests."
   - Click on "New Pull Request" and select your branch to compare with the main branch of the original repository.
   - Provide a description of your changes and submit the pull request.

### Guidelines
- Please ensure your code adheres to the existing style and conventions.
- Include tests for any new functionality where applicable.
- Update documentation as needed.

Thank you for considering contributing to this project!


## License

This project is licensed under the MIT License. See the [License](LICENSE.txt)
 file for more details.
