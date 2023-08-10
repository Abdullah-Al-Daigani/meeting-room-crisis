#include <header.h>

void callback(char *topic, byte *payload, unsigned int length) // the call back function which gets called with incoming MQTT messages
{
  // prints the incoming message topic to serial for debugging
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  StaticJsonDocument<256> payload_JSON;           // make a new JSON document to put the data into
  deserializeJson(payload_JSON, payload, length); // convert the character array  to a JSON document

  // int BracketRange[BracketCount];
  // bool ReservationStatus[BracketCount];
  Serial.println();

  for (int i = 0; i < BracketCount; i++)
  {
    BracketRange[i] = payload_JSON["BracketRange"][i];
    ReservationStatus[i] = payload_JSON["ReservationStatus"][i];
    if (ReservationStatus[i])
    {
      Serial.println(BracketRange[i]);
    }

    // Serial.println(BracketRange[i]);
    // Serial.println(ReservationStatus[i]);
  }

  String CurrentReserveName = payload_JSON["CurrentReserveName"];
  String CurrentReservePeriod = payload_JSON["CurrentReservePeriod"];
  // Serial.println(CurrentReserveName);

  content = payload_JSON["content"]; // stores the content parameter from the JSON document to the address *content
  // Serial.println(content);           // prints the content to serial for debugging
  message = content; // store the content of the telegram message in the variable
  // displayName();
}

void setup() // the setup function runs only once after startup
{
  delay(100);         // a delay to make sure the generation is complete before doing anything else
  Serial.begin(9600); // start the serial interface to report back through the USB for debugging
  while (!Serial)     // wait until serial is active before continuing
    ;

  Serial.print("Connecting to ");       // send to serial that WiFi connection is being attempted
  Serial.println(ssid);                 // show ssid of the network you're trying to connect to
  WiFi.begin(ssid, password);           // initiate the WiFi connection
  while (WiFi.status() != WL_CONNECTED) // wait until WiFi is connected
    ;

  Serial.println("WiFi connected"); // prints that the WiFi is connected to serial for debugging
  Serial.println("IP address: ");   //
  Serial.println(WiFi.localIP());   // prints the current IP address to serial for debugging
                                    //

  client.setServer(mqtt_server, 1883); // set the IP address and port of the MQTT broker
  client.setCallback(callback);        // set the callback function

  SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI); // start the SPI protocol
  display.init();                          // initiate the display
  display.setRotation(0);                  // set the rotation to portrait

  displayQRcode(); // display the QR code on the display
  displayName();   // display the name on the display

  delay(500); // wait to make sure the display is displaying correctly
}

void loop() // the loop function runs continuously
{

  if (!client.connected()) // make sure the MQTT client is active
  {                        //
    reconnect();           // reconnect to the MQTT client
  }
  client.loop(); // required for the MQTT client to function correctly
}

void reconnect() // reconnects to the MQTT client
{

  while (!client.connected()) // check whether the client is connected
  {
    Serial.print("Attempting MQTT connection..."); // prints to serial for debugging
    if (client.connect("ESP32-1"))                 // Attempt to connect
    {                                              //
      Serial.println("connected");                 // prints to serial for debugging
      client.publish("outTopic", "hello world");   // once connected, publish an announcement
      client.subscribe("Meeter");                  // resubscribe
    }
    else
    {
      Serial.print("failed, rc=");               // prints to serial for debugging
      Serial.print(client.state());              // prints the error to serial for debugging
      Serial.println(" try again in 5 seconds"); // prints to serial for debugging
      delay(5000);                               // Wait 5 seconds before retrying
    }
  }
}

void displayQRcode(void) // display the QR code on the screen
{
  QRCode qrcode;                                                               //
  uint8_t qrcodeData[qrcode_getBufferSize(3)];                                 // Create the QR code
  qrcode_initText(&qrcode, qrcodeData, 3, 0, "https://t.me/GarageMeetingBot"); //
  display.fillScreen(GxEPD_WHITE);
  for (uint8_t y = 0; y < qrcode.size; y++)
  {
    for (uint8_t x = 0; x < qrcode.size; x++)
    {
      if (qrcode_getModule(&qrcode, x, y))
      {
        display.fillRect(4 + (4 * x), 4 + (4 * y), 4, 4, GxEPD_BLACK);
      }
    }
  }
}

void displaySC1(String name, String period)
{
}

void displayName(void) // display the name and location of the locker on the screen
{
  displayQRcode();
  display.setCursor(10, 140);
  display.setTextSize(2);
  display.setTextColor(GxEPD_BLACK);
  display.print(message);
  display.update();
}