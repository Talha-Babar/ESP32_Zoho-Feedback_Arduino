
#include <HTTPClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

WebServer server(80);

String ssid = "";
String password = "";
//const char* apiKey = "https://www.zohoapis.com/crm/v2/functions/Customer_Feedback/actions/execute?auth_type=apikey&zapikey=1003.375bfdf09cba6ab0de8cd66a62085a7c.66ce1b40c27fa54e8b23f271fbde0af9";
const char* apiKey = "https://www.zohoapis.eu/crm/v2/functions/Sample/actions/execute?auth_type=apikey&zapikey=1003.bbb4158972effad1a500d591772c7d75.39665992c94420629e62c4b19f1da321";

const int debounceDelay = 500; // Adjust this value as needed
unsigned long lastDebounceTime = 0;

const char* htmlPage = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Wi-Fi Setup</title>
</head>
<body>
<h2>ESP32 Wi-Fi Setup</h2>
<form action="/get">
SSID: <input type="text" name="ssid"><br><br>
Password: <input type="password" name="pass"><br><br>
<input type="submit" value="Connect">
</form>
</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleFormSubmit() {
  ssid = server.arg("ssid");
  password = server.arg("pass");

  // Attempt to connect to the new Wi-Fi network
  WiFi.begin(ssid.c_str(), password.c_str());

  // Wait a bit for the connection to establish
  int retries = 30;
  while (WiFi.status() != WL_CONNECTED && retries--) {
    delay(1000);
    Serial.print(".");
  }

  // Check if connected
  if(WiFi.status() == WL_CONNECTED) {
    server.send(200, "text/plain", "Connected. IP Address: " + WiFi.localIP().toString());
  } else {
    server.send(200, "text/plain", "Connection failed.");
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(2, INPUT); // GPIO 4 for Button 1
  pinMode(4, INPUT); // GPIO 5 for Button 2
  pinMode(5, INPUT); // GPIO 6 for Button 3

  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
  }

  // Set up the access point
  WiFi.softAP("ESP32-Setup", "");

  // Register web server handlers
  server.on("/", handleRoot);
  server.on("/get", handleFormSubmit);

  // Start the server
  server.begin();

}

void loop() {
  // Check each button for button press with debounce
  debounceButton(2, "1"); // Button 1
  debounceButton(4, "2"); // Button 2
  debounceButton(5, "3"); // Button 3

    server.handleClient();
   
}

void debounceButton(int buttonPin, String data) {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Check for button press with debounce
  if ((buttonState == HIGH) && ((millis() - lastDebounceTime) > debounceDelay)) {
    // Button is pressed
    Serial.print("Button ");
    Serial.print(buttonPin);
    Serial.println(" is pressed");

    // Send data via HTTP
    sendData(data);

    lastDebounceTime = millis();
  }
}

void sendData(String data) {
  String jsonData = "{\"State\":\"" + data + "}";
  String url = String(apiKey) + "&State=" + data;

  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error: " + http.errorToString(httpResponseCode));
  }

  http.end();
}
