#include <ESP8266WiFi.h>

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Create a server on port 80
WiFiServer server(80);

void setup() {
  // Start Serial for communication with Arduino Uno
  Serial.begin(9600);  // Baud rate must match the Arduino's

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Process the request and send the corresponding serial command to Arduino
    if (request.indexOf("/red/on") != -1) {
      Serial.println("red/on");  // Send command with line break
      Serial.println("Sending 'red/on' command to Arduino");
    } else if (request.indexOf("/red/off") != -1) {
      Serial.println("red/off");
      Serial.println("Sending 'red/off' command to Arduino");
    }

    if (request.indexOf("/yellow/on") != -1) {
      Serial.println("yellow/on");
      Serial.println("Sending 'yellow/on' command to Arduino");
    } else if (request.indexOf("/yellow/off") != -1) {
      Serial.println("yellow/off");
      Serial.println("Sending 'yellow/off' command to Arduino");
    }

    if (request.indexOf("/green/on") != -1) {
      Serial.println("green/on");
      Serial.println("Sending 'green/on' command to Arduino");
    } else if (request.indexOf("/green/off") != -1) {
      Serial.println("green/off");
      Serial.println("Sending 'green/off' command to Arduino");
    }

    if (request.indexOf("/blue/on") != -1) {
      Serial.println("blue/on");
      Serial.println("Sending 'blue/on' command to Arduino");
    } else if (request.indexOf("/blue/off") != -1) {
      Serial.println("blue/off");
      Serial.println("Sending 'blue/off' command to Arduino");
    }

    if (request.indexOf("/white/on") != -1) {
      Serial.println("white/on");
      Serial.println("Sending 'white/on' command to Arduino");
    } else if (request.indexOf("/white/off") != -1) {
      Serial.println("white/off");
      Serial.println("Sending 'white/off' command to Arduino");
    }

    // Send HTTP response back to the client
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<html><body>LED Control Successful</body></html>");
    client.stop();  // Close the connection to the client
  }
}
