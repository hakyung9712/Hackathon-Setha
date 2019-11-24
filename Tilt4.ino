#include <ESP8266WiFi.h>
const char* ssid = "SYNETWORK";
const char* password = "12345678";

WiFiServer server(80);
void setup() {
 Serial.begin(115200);
 delay(10);

 // Connect to WiFi network
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 // Start the server
 server.begin();
 Serial.println("Server started");
 // Print the IP address
 Serial.print("Use this URL to connect: ");
 Serial.print("http://");
 Serial.print(WiFi.localIP());
 Serial.println("/");
}
void loop() {
 // Check if a client has connected
 WiFiClient client = server.available();
 if (!client) {
 return;
 }
 // Wait until the client sends some data
 Serial.println("new client");
 while(!client.available()){
 delay(1);
 }
// Read the first line of the request
 String request = client.readStringUntil('\r');
 Serial.println(request);
 client.flush();
 delay(2000);

 int tiltlevel = analogRead(A0);
 // Return the response
 //client.println("HTTP/1.1 200 OK");
 //client.println("Content-Type: text/html");
 //client.println(""); // do not forget this one
 client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 1\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.print("Tilt level: ");
 client.print(tiltlevel);
 Serial.println(tiltlevel);
 client.println("</html>");
 delay(1);
 Serial.println("Client disonnected");
 Serial.println("");

}
