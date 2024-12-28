# ESP32 Wi-Fi Setup and Button Feedback to API

This project demonstrates using an ESP32 to set up Wi-Fi credentials through a web interface and send button feedback to a specified API endpoint. The system includes a debounce mechanism for button presses to ensure reliable data transmission.

---

## Features

- **Wi-Fi Configuration**: Configure Wi-Fi credentials via a hosted web interface.
- **Button Press Feedback**: Send feedback to an API when buttons are pressed.
- **Debounce Mechanism**: Prevent multiple detections for a single button press.
- **SPIFFS**: Used for internal storage.

---

## How It Works

1. **Wi-Fi Setup**:

   - The ESP32 starts as a Wi-Fi access point named `ESP32-Setup`.
   - A hosted webpage allows users to input Wi-Fi credentials.

2. **Button Feedback**:

   - Monitors three buttons connected to GPIO pins.
   - Sends button feedback as a JSON payload to the configured API endpoint.

3. **Debounce Mechanism**:

   - Ensures only one signal is sent per button press.

4. **API Integration**:
   - Button states are sent as JSON to the specified API URL.
