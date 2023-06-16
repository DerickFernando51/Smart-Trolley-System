#include <WiFi.h>
#include <WiFiMulti.h>
#include <HX711.h>
#include "HX711.h"
#include <CuteBuzzerSounds.h> //buzzer
#include<LiquidCrystal_I2C.h> //LCD 16*2
#include <Arduino.h>
//#include <Firebase_ESP_Client.h>
#include<FirebaseESP32.h>
#include <Wire.h>
#include "time.h"
 
 
//#include<Wire.h>
int i=0; //firebase scanned barcodes variable
int j=0; //firebase removed barcodes variable
int remobuttonState=0;//state is changed to 1 in ISR1
int endbuttonState=0;//state is changed to 1 in ISR2

//FIREBASE VARIABLES/
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID " "
#define WIFI_PASSWORD  " "

//* 2. Define the API Key */
#define API_KEY " "

//* 3. Define the RTDB URL */
#define DATABASE_URL " " //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

//* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL " "
#define USER_PASSWORD " "



// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
String itemPath = "/ItemDesc";
 

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 180000;
 

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


//BARCODE/
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define GREEN_LED 2  // green led/yellow
#define WIFI_LED 4  // WiFi led
#define LIGHT1 12 //LED 1 in the trolley bucket
#define LIGHT2 13 //LED 2 in the trolley bucket
#define BUZZER_PIN 18 //~ with red led
#define REMO_BUTTON_PIN 27 // button for remove items
#define END_BUTTON_PIN 5 //button for End purchasing 

const uint32_t connectTimeoutMs = 10000;
String Code;  //barcodereader
const int LOADCELL_DOUT_PIN = 23;
const int LOADCELL_SCK_PIN = 19;

WiFiMulti wifiMulti;
HX711 scale;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

int count = 0;

int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

double total = 0;
double weight = 0;
 
char input[13];
int count_prod = 0;

void ISR1();
void ISR2();


void setup()
{
  attachInterrupt(digitalPinToInterrupt(REMO_BUTTON_PIN), ISR1, CHANGE);//External interrupt triggered when remove button is pressed
  attachInterrupt(digitalPinToInterrupt(END_BUTTON_PIN), ISR2, CHANGE);//External interrupt triggered when remove button is pressed
  cute.init(BUZZER_PIN);  //buzzer
  pinMode(REMO_BUTTON_PIN, INPUT_PULLUP);  // config GIOP21 as input pin and enable the internal pull-up resistor for remove items
  pinMode(END_BUTTON_PIN, INPUT_PULLUP);  //End purchasing
  pinMode(WIFI_LED, OUTPUT);  //LED WIFI
  pinMode(GREEN_LED, OUTPUT);  // LED Green
  pinMode(LIGHT1, OUTPUT); // LED 1 in the trolley bucket
  pinMode(LIGHT2, OUTPUT); // LED 2 in the trolley bucket

  Serial.begin(115200);
  delay(10);
  Serial2.begin(9600);
  Serial2.setTimeout(100);

  WiFi.mode(WIFI_STA);
  lcd.init();

  wifiMulti.addAP(" ", " "); //Add wifi network name and password
 

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(" no networks ");
    lcd.setCursor(7, 1);
    lcd.print(" found ");
  }
  else {

    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
    Serial.println("Connecting Wifi...");
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Connecting Wifi.");

    if (wifiMulti.run() == WL_CONNECTED) {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.print(WiFi.localIP());
      Serial.print('\t');
      Serial.print(WiFi.SSID());
      digitalWrite(WIFI_LED, HIGH); // WIFI LED ON

      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print(" WiFi Connected ");
      lcd.setCursor(0, 1);
      lcd.print(WiFi.localIP());
      delay(2000);
      lcd.clear();
      digitalWrite(WIFI_LED, LOW);  //WIFI light off
      lcd.noBacklight();
      delay(100);

      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print(" AUTOMATIC BILL ");
      lcd.setCursor(0, 1);
      lcd.print(" SHOPPING CART ");

      //~ Starting oad cell calibration ~
      Serial.println(" ");
      Serial.println("HX711 Demo");
      Serial.println("Initializing the scale");
      scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
      Serial.println("Before setting up the scale:");
      Serial.print("read: \t\t");
      Serial.println(scale.read());      // print a raw reading from the ADC

      Serial.print("read average: \t\t");
      Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

      Serial.print("get value: \t\t");
      Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

      Serial.print("get units: \t\t");
      Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
      // by the SCALE parameter (not set yet)

      scale.set_scale(5.509);// this value is obtained by calibrating the scale with known weights; 
      scale.tare();// reset the scale to 0

      Serial.println("After setting up the scale:");

      Serial.print("read: \t\t");
      Serial.println(scale.read()); // print a raw reading from the ADC

      Serial.print("read average: \t\t");
      Serial.println(scale.read_average(20));  // print the average of 20 readings from the ADC

      Serial.print("get value: \t\t");
      Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

      Serial.print("get units: \t\t");
      Serial.println(scale.get_units(5), 1);   // print the average of 5 readings from the ADC minus tare weight, divided  by the SCALE parameter set with set_scale

      Serial.println("Readings:");

      lcd.clear();

      Serial.println("Welcome to SLT SMART TROLLEY");
      delay(500);

      lcd.setCursor(0, 0);
      lcd.print(" WELCOME TO SLT ");
      lcd.setCursor(0, 1);
      lcd.print(" SMART TROLLEY ");
      delay(4000);
      lcd.clear();
      digitalWrite(LIGHT1, HIGH); //LED 1 on
      digitalWrite(LIGHT2, HIGH); //LED 2 on

      lcd.setCursor(0, 0);
      lcd.print("TotalAmount :RS.");
      lcd.setCursor(5, 1);
      lcd.print(total);
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TotalWeight(g):");
      lcd.setCursor(5, 1);
      lcd.print(weight);
      delay(1500);
      lcd.clear();

      Serial.println("You Can Add Items To Trolley");
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print(" You Can Add");
      lcd.setCursor(0, 1);
      lcd.print("Items To Trolley");

      

    }
  }
//FIREBASE
  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings/";
  
}


void loop()
{
  remobuttonState=0;
  endbuttonState=0;

  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected!");
    delay(500);
    cute.play(S_OHOOH);
    delay(2000);
    lcd.setCursor(3, 0);
    lcd.print(" WiFi Not ");
    lcd.setCursor(3, 1);
    lcd.print(" Connect ");
    delay(1000);
    lcd.clear();
    wifiMulti.run(connectTimeoutMs) == WL_CONNECTED;
    Serial.println(" ");
    Serial.println("WiFi connected: ");
    //digitalWrite(WIFI_LED, HIGH); // WIFI LED ON
    Serial.print(WiFi.SSID());
    Serial.print(" ");
    Serial.println(WiFi.RSSI());
    delay(2000);
  }
  // digitalWrite(WIFI_LED, LOW);  //WIFI light off

  //WiFiClient client = server.available();   // Listen for incoming clients

  // barcodereader
  while (Serial2.available() > 0) {
    lcd.clear();
    Code = Serial2.readString();
    Serial.println(Code);

      
  // Send scanned barcodes to firebase database
      databasePath = "/Smart_Cart_Wishlist/";
      parentPath= databasePath + String(i); 
      i=i+1; //Increment scanned barcodes index variable
      json.set(itemPath.c_str(), String(Code));
      Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
   
  Serial.println("Put the Item on the Trolley");
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print(" Put the Item ");
    lcd.setCursor(0, 1);
    lcd.print("on the Trolley");
    digitalWrite(GREEN_LED, HIGH); // LED GREEN ON
    cute.play(S_CONNECTION);
    delay(5000);
    lcd.clear();
    //Loadcell();

    
    
    Serial.println("Press remove btn if u want to remove");
    lcd.setCursor(0, 0);
    lcd.print("Press remove");
    lcd.setCursor(0, 1);
    lcd.print("button to remove");
    delay(5000);
     
    Remove_Item();
    End_purchasing();
    
    Serial.println("You can add more items to trolley");
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("You Can Add More");
    lcd.setCursor(0, 1);
    lcd.print("Items To Trolley");
    delay(5000);
     

    Remove_Item();
    End_purchasing();

  
  }
 
}


//LOAD CELL FUNCTION/
void Loadcell() {

  Serial.println("Don't Touch Trolley");
  delay(500);
  lcd.setCursor(1, 0);
  lcd.print(" Don't Touch ");
  lcd.setCursor(1, 1);
  lcd.print(" the Trolley ");
  cute.play(S_FART2);
  delay(2000);
  lcd.clear();

  Serial.println("Start Measuring Weight");
  delay(500);
  lcd.setCursor(1, 0);
  lcd.print("Start Measuring ");
  lcd.setCursor(5, 1);
  lcd.print(" Weight ");
  delay(1500);
  lcd.clear();

  Serial.print("one reading:\t");
  Serial.print(scale.get_units(5), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(20), 5);
  lcd.setCursor(1, 0);                        //display weight
  lcd.print(" Item Weight ");
  lcd.setCursor(5, 1);
  lcd.print(scale.get_units(20), 5);
  delay(3000);
  lcd.clear();

  scale.power_down();             // put the ADC in sleep mode
  delay(5000);
  Serial.println("END Measuring Weight");
  delay(300);
  lcd.setCursor(1, 0);
  lcd.print(" END Measuring ");
  lcd.setCursor(5, 1);
  lcd.print(" Weight ");
  digitalWrite(GREEN_LED, LOW); // LED Green OFF
  cute.play( S_MODE3);// S_OHOOH
  delay(2000);
  lcd.clear();
  scale.power_up();

}


 

void Remove_Item(){
  while(remobuttonState==1){
  Serial.println("Scan the item to Remove");
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Scan the item  ");
  lcd.setCursor(1, 1);
  lcd.print("to Remove");

   while (Serial2.available()== 0) {}
    lcd.clear();
    Code = Serial2.readString();
    Serial.println(Code);
  
  //SEND REMOVED ITEMS TO FIREBASE//   
      databasePath = "/Removed_Items/";
      parentPath= databasePath + String(j);
       j=j+1;//Increment firebase barcode no. variable
      json.set(itemPath.c_str(), String(Code));
       Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    

    //Remove ITEM FROM SCANNED ITEMS FIREBASE//   
      databasePath = "/Smart_Cart_Wishlist/";
      i=i-1;//Increment firebase barcode no. variable
      parentPath= databasePath + String(i);

       json.remove(itemPath.c_str());
       Serial.printf("Set json... %s\n", Firebase.RTDB.deleteNode(&fbdo,parentPath.c_str()) ? "ok" : fbdo.errorReason().c_str());
   


    digitalWrite(GREEN_LED, HIGH); // LED GREEN ON
    cute.play(S_CONNECTION);
    delay(5000);
    lcd.clear();
   // Loadcell();
    
    remobuttonState=0;
  }

}

void End_purchasing() {
  while(endbuttonState==1){
  Loadcell();
  Serial.println("Total Amount : RS. ");
  Serial.print(total);
  Serial.print("");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TotalAmount: Rs.");
  lcd.setCursor(4, 1);
  lcd.print(total);
  delay(4000);

  Serial.println("Total Weight : ");
  Serial.print(weight);
  Serial.print("");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TotalWeight(g): ");
  lcd.setCursor(4, 1);
  lcd.print(weight);
  delay(4000);

  Serial.println("Thank You !");
  Serial.println("Come Again...");
  lcd.setCursor(0, 0);
  lcd.print(" Thank You !!!! ");
  lcd.setCursor(0, 1);
  lcd.print(" Come Again ... ");
  delay(5000);

  lcd.clear();
  delay(1000);
  lcd.noBacklight();

  digitalWrite(LIGHT1, LOW); //LED 1 off
  digitalWrite(LIGHT2, LOW); //LED 2 off
  }

}
void ISR1() {   
   remobuttonState = 1;   
}

void ISR2() {   
   endbuttonState = 1;   
}

///~ END Process~~~//

 
