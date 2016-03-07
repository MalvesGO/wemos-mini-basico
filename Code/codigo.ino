#include <ESP8266WiFi.h>

const char* ssid = "SSID";
const char* password = "password";



int ledPin = D4; // GPIO2
int ledPin2 = D3; // GPIO2

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);

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
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  int value2 = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  if (request.indexOf("/LED2=ON") != -1)  {
    digitalWrite(ledPin2, HIGH);
    value2 = HIGH;
  }
  if (request.indexOf("/LED2=OFF") != -1)  {
    digitalWrite(ledPin2, LOW);
    value2 = LOW;
  }

  // Set ledPin according to the request
  //digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<meta charset ='utf-8'/>");
  client.println("<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>");


  client.println(F("<link href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css' rel='stylesheet'></link>"));


  client.println("<div class='jumbotron'>");
  client.println("<h1 class='text-center'>Automação Residencial</h1>");
  client.println("<h2 class='text-center'>ESP8266</h2>");
  client.println("</div>");

  client.println("<div class='col-md-6'>");
  client.println("<div class='alert alert-info text-center' role='alert'>Lampada 1</div>");
  client.println("<a class='btn btn-danger btn-lg center-block'  href=\"/LED=ON\"> Desligar </a>");
  client.println("<br>");
  client.println("<a class='btn btn-success btn-lg center-block' href=\"/LED=OFF\"> Ligar </a>");
  client.println("</div>");

  client.println("<div class='col-md-6'>");
  client.println("<div class='alert alert-info text-center' role='alert'>Lampada 2</div>");
  client.println("<a class='btn btn-danger btn-lg center-block'  href=\"/LED2=ON\"> Desligar </a>");
  client.println("<br>");
  client.println("<a class='btn btn-success btn-lg center-block' href=\"/LED2=OFF\"> Ligar </a>");
  client.println("</div>");

  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
