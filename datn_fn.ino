//--------------------------Library------------------------------------
#include <string.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <WiFi.h>
//----------------------------define value----------------------------

#define room1 14
#define room2 33
#define light1 13
#define light2 4
#define fireSensor1 0
#define fireSensor2 26
#define buzz 32

//-----------------------------------Define address------------------------
#define keypad1 0x20
#define keypad2 0x22
LiquidCrystal_I2C lcd1(0x23, 16, 2);
LiquidCrystal_I2C lcd2(0x27, 16, 2);
//-----------------------------Wifi---------------------------------
const char *ssid = "Vutu";
const char *password = "1234512345";
//------------------------------MQTT setup----------------------------
const char *mqtt_server = "192.168.34.73";
WiFiClient espClient;
PubSubClient client(espClient);
//----------room1--------------------------------------------------------
String password1 = "";
bool buttonPressed1 = false;
long buttonTimer1 = 0;
bool stringAdded1 = false;
String pressedCharacter1 = "";
String grand_password1 = "";
bool lightStatus1 = false;
//----------room2------------------------------------------------------
String password2 = "";
bool buttonPressed2 = false;
long buttonTimer2 = 0;
bool stringAdded2 = false;
String pressedCharacter2 = "";
String grand_password2 = "";
bool lightStatus2 = false;
//------------------Dual Core-----------------------------------
TaskHandle_t Task1;
//-------------Setup-------------------------------------------------
void setup() {
  xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1",   /* Name of the task */
    10000,     /* Stack size in words */
    NULL,      /* Task input parameter */
    0,         /* Priority of the task */
    &Task1,    /* Task handle. */
    0);        /* Core where the task should run */
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);  //
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);  //1883 is the default port for MQTT server
  client.setCallback(callback);
  pinMode(room1, OUTPUT);
  pinMode(room2, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(fireSensor1, INPUT);
  pinMode(fireSensor2, INPUT);
  pinMode(buzz, OUTPUT);
  
  digitalWrite(room1, HIGH);
  digitalWrite(room2, HIGH);
  Wire.begin();
  lcd1.init();  // initialize the lcd
  lcd1.backlight();
  lcd2.init();  // initialize the lcd
  lcd2.backlight();
  lcd1.setCursor(0, 0);
  lcd1.print("Password:");
  lcd2.setCursor(0, 0);
  lcd2.print("Password:");
}
//-------------------------------Connect to MQTT broker-------------------------------
void connect_mqttServer() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //now attemt to connect to MQTT server
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32_client1"))  // Change the name of client here if multiple ESP32 are connected
    //attempt successful
    {
      Serial.println("connected");
      // Subscribe to topics here
      client.subscribe("rpi/broadcast1");
      client.subscribe("rpi/broadcast2");  //subscribe more topics here
    } else
    //attempt not successful
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}
//--------------------------------Receive data whenever it come------------------------
void callback(char *topic, byte *message, unsigned int length) {
  Serial.println("Message arrived on topic: ");
  Serial.print(String(topic));
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  if (String(topic) == "rpi/broadcast2")
    grand_password2 = messageTemp;
  else if (String(topic) == "rpi/broadcast1")
    grand_password1 = messageTemp;

  // Check if a message is received on the topic "rpi/broadcast"
  //Similarly add more if statements to check for other subscribed topics
}
//-------------------------Check password----------------------------------------------
void checkInput(bool *buttonPressed, bool *stringAdded, String *password, String *pressedCharacter, int status, String grand_password, int room, bool *lightStatus) 
{
  if (*buttonPressed == true)  //if a button was pressed...
  {
    if (*stringAdded == true)  //if a numberical button was pressed...
    {

      *password = *password + *pressedCharacter;  //concatenate
      Serial.println(*password);
      //Print the string on the LCD
      if (status == 1) {
        lcd1.setCursor(9, 0);
        lcd1.print(*password);
      } else {
        lcd2.setCursor(9, 0);
        lcd2.print(*password);
      }
      *stringAdded = false;  //don't let the code enter this part again
    }

    if (*pressedCharacter == "*")  //if we pressed the A on the keypad...
    {
      if (*password == grand_password || *password == "555555") {
        if (status == 1) {
          lcd1.setCursor(9, 0);
          lcd1.print("        ");
          *lightStatus = true;
        } else {
          lcd2.setCursor(9, 0);
          lcd2.print("        ");
          *lightStatus = true;
        }
        digitalWrite(room, LOW);
        delay(2000);
        digitalWrite(room, HIGH);
        *password = "";

      } else {
        *password = "";
        if (status == 1) {
          lcd1.setCursor(0, 1);
          lcd1.print("Wrong password");
          delay(1000);
          lcd1.setCursor(0, 1);
          lcd1.print("              ");
          lcd1.setCursor(9, 0);
          lcd1.print("        ");
        } else {
          lcd2.setCursor(0, 1);
          lcd2.print("Wrong password");
          delay(1000);
          lcd2.setCursor(0, 1);
          lcd2.print("              ");
          lcd2.setCursor(9, 0);
          lcd2.print("        ");
        }
      }
    } else if (*pressedCharacter == "#")  //D = delete
    {
      *password = "";
      if (status == 1) {
        lcd1.setCursor(9, 0);
        lcd1.print("        ");
      } else {
        lcd2.setCursor(9, 0);
        lcd2.print("        ");
      }
    }
  }
}
//------------------------Read Keypad-----------------------------------------------
void readKeyPad(uint8_t ID, bool *buttonPressed, long *buttonTimer, String *pressedCharacter, bool *stringAdded) {
  if (*buttonPressed == true) {
    if (millis() - *buttonTimer > 300) {
      //The button pressed is only set back to false after 300 ms, so we cannot press a button twice quickly
      *buttonTimer = millis();
      *buttonPressed = false;
    } else {
      //do nothing
    }
  } else {
    //B11101111
    Wire.beginTransmission(ID);  //00100000
    Wire.write(B11101111);
    //[P7]B11101111[P0] -> [P7]1110[P4] - activates first row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();
    Wire.requestFrom(ID, 1);
    switch (Wire.read()) {
      //11101110 - P0 pin went low after pressing the button -> 1 was pressed
      case 238:  //Button 1
        Serial.print("1");
        *pressedCharacter = "1";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      //11101101 - P1 pin went low after pressing the button -> 2 was pressed
      case 237:  //Button 2
        Serial.print("4");
        *pressedCharacter = "4";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      //11101011 - P2 pin went low after pressing the button -> 3 was pressed
      case 235:  //Button 3
        Serial.print("7");
        *pressedCharacter = "7";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      //11100111 - P3 pin went low after pressing the button -> A was pressed
      case 231:  //Button A
        Serial.println("*");
        *pressedCharacter = "*";
        *buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B11011111
    Wire.beginTransmission(ID);  //00100000
    Wire.write(B11011111);
    //[P7]B11011111[P0] -> [P7]1101[P4] - activates second row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(ID, 1);
    switch (Wire.read()) {
      //11011110 - P0 pin went low after pressing the button -> 2 was pressed
      case 222:  //Button 4
        Serial.print("2");
        *pressedCharacter = "2";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 221:  //Button 5
        Serial.print("5");
        *pressedCharacter = "5";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 219:  //Button 6
        Serial.print("8");
        *pressedCharacter = "8";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 215:  //Button B
        Serial.print("0");
        *pressedCharacter = "0";
        *buttonPressed = true;
        *stringAdded = true;
        break;
    }
    //-------------------------------------------
    //B10111111
    Wire.beginTransmission(ID);  //00100000
    Wire.write(B10111111);
    Wire.endTransmission();

    Wire.requestFrom(ID, 1);
    switch (Wire.read()) {
      case 190:  //Button 7
        Serial.print("3");
        *pressedCharacter = "3";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 189:  //Button 8
        Serial.print("6");
        *pressedCharacter = "6";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 187:  //Button 9
        Serial.print("9");
        *pressedCharacter = "9";
        *buttonPressed = true;
        *stringAdded = true;
        break;

      case 183:  //Button C
        Serial.println("#");
        *pressedCharacter = "#";
        *buttonPressed = true;
        break;
    }
    *buttonTimer = millis();
  }
}
//------------------Open Light Whenever Check-in Active And Fire Warning----------------------------
void open_light(String password, int light, bool *lightStatus) {
  if (password == "None" || password == "")
    *lightStatus = false;
  if (*lightStatus == true)
    digitalWrite(light, HIGH);
  else
    digitalWrite(light, LOW);
}

void fire_warning() {
  if (digitalRead(fireSensor1) == 0 || digitalRead(fireSensor2) == 0)
    analogWrite(buzz, 255);
  else
    analogWrite(buzz, 0);
}
//------------------Core1----------------------------
void Task1code(void *parameter) {
  for (;;) {
    readKeyPad(keypad1, &buttonPressed1, &buttonTimer1, &pressedCharacter1, &stringAdded1);
    readKeyPad(keypad2, &buttonPressed2, &buttonTimer2, &pressedCharacter2, &stringAdded2);
    checkInput(&buttonPressed1, &stringAdded1, &password1, &pressedCharacter1, 1, grand_password1, room1, &lightStatus1);
    checkInput(&buttonPressed2, &stringAdded2, &password2, &pressedCharacter2, 0, grand_password2, room2, &lightStatus2);
    open_light(grand_password1, light1, &lightStatus1);
    open_light(grand_password2, light2, &lightStatus2);
    fire_warning();
  }
}
//------------------Core2------------------------------
void loop() {
  if (!client.connected()) {
    connect_mqttServer();
  }
  client.loop();
}

