


// Load Wi-Fi library
#include <WiFi.h>
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//LED define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
String ledHistory = R"=====(
    [
        {"time": "10:00", "state": "Bật"},
        {"time": "10:05", "state": "Tắt"},
        {"time": "10:10", "state": "Bật"},
        {"time": "10:15", "state": "Tắt"},
        {"time": "10:20", "state": "Bật"}
    ]
)=====";

// Replace with your network credentials
const char* ssid = "VIETTEL";
const char* password = "3ngay0tam";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";


// Assign output variables to GPIO pins
const int output26 = 2;

int flag = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void setup() {
  Serial.begin(115200);
  
  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);

  // Set outputs to LOW
  digitalWrite(output26, LOW);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("BUILT_IN LED on");
              output26State = "on";
              digitalWrite(output26, HIGH);
             
             
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("BUILT_IN LED off");
              output26State = "off";
              digitalWrite(output26, LOW);
              
               
               } 
               
            // Display the HTML web page
            // client.println("<!DOCTYPE html><html>");
            // client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            // client.println("<link rel=\"icon\" href=\"data:,\">");
            // // CSS to style the on/off buttons 
            // // Feel free to change the background-color and font-size attributes to fit your preferences
            // client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            // client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            // client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            // client.println(".button2 {background-color: #555555;}</style></head>");
            
            // // Web Page Heading
            // client.println("<body><h1>ESP32 Web Server</h1>");
            
            // // Display current state, and ON/OFF buttons for BUILT_IN LED
            // client.println("<p>BUILT_IN LED - State " + output26State + "</p>");
            // // If the output26State is off, it displays the ON button       
            // if (output26State=="off") {
            //   client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            // } else {
            //   client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            // } 
               
            // // Display current state, and ON/OFF buttons for GPIO 27  
           
            // client.println("</body></html>");
            
            // // The HTTP response ends with another blank line
            // client.println();
            client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS to style the switch
client.println("<style>");
client.println("html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".switch { position: relative; display: inline-block; width: 60px; height: 34px;}");
client.println(".switch input { opacity: 0; width: 0; height: 0;}");
client.println(".slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; transition: .4s;}");
client.println(".slider:before { position: absolute; content: ''; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; transition: .4s;}");
client.println("input:checked + .slider { background-color: #4CAF50;}");
client.println("input:checked + .slider:before { transform: translateX(26px);}");
client.println(".slider.round { border-radius: 34px;}");
client.println(".slider.round:before { border-radius: 50%;}");
client.println("</style></head>");

client.println("<body><h1>ESP32 Web Server</h1>");

// Display current state, and switch for BUILT_IN LED
client.println("<p>BUILT_IN LED - State " + output26State + "</p>");
client.println("<label class=\"switch\">");
if (output26State == "off") {
  client.println("<input type=\"checkbox\" onclick=\"location.href='/26/on'\">");
} else {
  client.println("<input type=\"checkbox\" checked onclick=\"location.href='/26/off'\">");
}
client.println("<span class=\"slider round\"></span>");
client.println("</label>");

client.println("</body></html>");
client.println();

          
            // Break out of the while loop
            
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
