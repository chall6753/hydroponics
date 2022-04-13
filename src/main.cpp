// preprocessor
#include <Arduino.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>    //include arduinos i2c library
#include <sequencer3.h> //imports a 3 function sequencer
#include <Ezo_i2c_util.h> //brings in common print statements
#include <Adafruit_HTU21DF.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "RTClib.h"
#include <SPI.h>

Ezo_board EC = Ezo_board(60, "EC");      //create an EC circuit object who's address is 60 and name is "EC"
Ezo_board RTD = Ezo_board(102, "RTD");    //create an RTD circuit object who's address is 102 and name is "RTD"
Ezo_board PH = Ezo_board(50, "PH");
Ezo_board PMP_UP = Ezo_board(1, "PMP_UP");                   //create a pump circuit object, whose address is 50 and name is "PMP_UP". This pump dispenses pH up solution.
Ezo_board PMP_DOWN = Ezo_board(2, "PMP_DOWN");
Ezo_board PMP_A = Ezo_board(3, "PMP_A"); //nutrient A pump
Ezo_board PMP_B = Ezo_board(4, "PMP_B"); //nutrient B pump

RTC_DS3231 rtc;
Adafruit_HTU21DF htu = Adafruit_HTU21DF();
void step1(); //forward declarations of functions to use them in the sequencer before defining them
void step2();
void step3();
// Replace with your network credentials
const char* ssid     = "dogghouse";
const char* password = "Jester91!";
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://charlies-farm-database.herokuapp.com/src/post-esp-data.php";
const char* serverName1 = "https://charlies-farm-database.herokuapp.com/src/GetData.php";
float water_temp;
float pH;
float ec;
int ph_up_pump;
int ph_down_pump;
int pmp_a;
int pmp_b;
Sequencer3 Seq( &step1, 815, //calls the steps in sequence with time in between them
                &step2, 815, 
                &step3, 2000); 

void setup() {      
  Wire.begin();                           //start the I2C
  Serial.begin(115200);                     //start the serial communication to the computer
  Seq.reset();
  htu.begin();                            //initialize the sequencer
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(12,OUTPUT);//pump to arduino
  pinMode(14,OUTPUT);//light to arduino
  pinMode(27,OUTPUT);//heater to arduino
  pinMode(26,OUTPUT);//fan to arduino
}

void loop() {
  Seq.run();                              //run the sequncer to do the polling
}

void step1() {
  //code to check relay database and control relays
  HTTPClient http;
  String payload;
  String getData;
  http.begin(serverName1);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // Send HTTP GET request
        getData = "id";
          int httpGetResponseCode = http.POST(getData);
           
        if (httpGetResponseCode>0) {
            Serial.print("HTTP Get Response code: ");
            Serial.println(httpGetResponseCode);
            payload = http.getString();
            Serial.println(payload);
          }
          else {
            Serial.print("Error code: ");
            Serial.println(httpGetResponseCode);
          }
          // Free resources
          http.end();
   
  
  String pumpStatus = payload.substring(11,12);
  String lightStatus = payload.substring(23,24);
  String fanStatus = payload.substring(35,36);
  String brightLight = payload.substring(47,48);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  DateTime now = rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print('hour');
    int y = now.hour();
    if(y > 7 && y < 22){
      Serial.println("turn lights on");
    }
    Serial.print(y);


  if(pumpStatus == String(1)){
    digitalWrite(12,HIGH); 
  }
  else{digitalWrite(12,LOW);
  }
  if(y > 6 && y < 22){
    digitalWrite(14,HIGH); 
  }
  else{digitalWrite(14,LOW);
  }
  // if(lightStatus == String(1)){
  //   digitalWrite(14,HIGH); 
  // }
  // else{digitalWrite(14,LOW);
  // }
  if(fanStatus == String(1)){
    digitalWrite(27,HIGH); 
  }
  else{digitalWrite(27,LOW);
  }
  if(y > 6 && y < 22){
    digitalWrite(26,HIGH); 
  }
  else{digitalWrite(26,LOW);
  }
  // if(brightLight == String(1)){
  //   digitalWrite(26,HIGH); 
  // }
  // else{digitalWrite(26,LOW);
  // }

  //send a read command. we use this command instead of RTD.send_cmd("R"); 
  //to let the library know to parse the reading

  RTD.send_read_cmd();
  PH.send_read_cmd();
    float temp = htu.readTemperature();
    float rel_hum = htu.readHumidity();
    Serial.print("Temp: "); Serial.print(temp); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(rel_hum); Serial.println(" \%");
}

void step2() {
  receive_and_print_reading(RTD);             //get the reading from the RTD circuit
  receive_and_print_reading(PH);
  if ((RTD.get_error() == Ezo_board::SUCCESS) && (RTD.get_last_received_reading() > -1000.0)) {        //if the temperature reading has been received and it is valid
    water_temp = RTD.get_last_received_reading();
    EC.send_read_with_temp_comp(RTD.get_last_received_reading());                               //send readings from temp sensor to EC sensor
  } else {                                                                                      //if the temperature reading is invalid
    EC.send_read_with_temp_comp(25.0);                                                          //send default temp = 25 deg C to EC sensor
  }
  Serial.print(" ");
  pH = PH.get_last_received_reading();
  if (PH.get_error() == Ezo_board::SUCCESS){
    if (PH.get_last_received_reading() <= 5.5) {                            //test condition against pH reading
      Serial.println("PH LEVEL LOW,PMP_UP = ON");
      PMP_UP.send_cmd_with_num("d,", 1);                  //if condition is true, send command to turn on pump (called PMP_UP) and dispense pH up solution, in amounts of 0.5ml. Pump turns clockwise.
      ph_up_pump=1;
    }
    else {
      PMP_UP.send_cmd("x"); 
      ph_up_pump=0;                                //if condition is false, send command to turn off pump (called PMP_UP)
    }
  
    if (PH.get_last_received_reading() >= 6.3) {                          //test condition against pH reading
      Serial.println("PH LEVEL HIGH,PMP_DOWN = ON");
      PMP_DOWN.send_cmd_with_num("d,", 1);               //if condition is true, send command to turn on pump (called PMP_DOWN) and dispense pH down solution, in amounts of 0.5ml. Pump turns clockwise.
      ph_down_pump=1;
    }
    else {
      PMP_DOWN.send_cmd("x"); 
      ph_down_pump=0;                              //if condition is false, send command to turn off pump (called PMP_DOWN)
    }
  }
   
}

void step3() {
  receive_and_print_reading(EC);               //get the reading from the EC circuit and print it
  Serial.println();
  ec = EC.get_last_received_reading();
// pumps for nutrients
if (ec < 1200){
  PMP_A.send_cmd_with_num("d,", 1);
  PMP_B.send_cmd_with_num("d,", 1);          
  pmp_a =1; //lets database know when pumps were turned on
  pmp_b =1;
}else{
  pmp_a=0;
  pmp_b=0;
}
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    
    // Prepare your HTTP POST request data
    
    float temp = htu.readTemperature();
    float rel_hum = htu.readHumidity();
    String temp_str = String(temp);
    String rel_hum_str = String(rel_hum);
    String water_temp_str = String(water_temp);
    String pH_str = String(pH);
    String ec_str = String(ec);
    String ph_up_pump_str = String(ph_up_pump);
    String ph_down_pump_str = String(ph_down_pump);
    String pmp_a_str = String(pmp_a);
    String pmp_b_str = String(pmp_b);
    String httpRequestData = "temp_air="+temp_str+"&humidity="+rel_hum_str+"&temp_water="+water_temp_str+"&pH="+pH_str+"&ec="+ec_str+"&ph_up_pump="+ph_up_pump_str+"ph_down_pump="+ph_down_pump_str+"&pmp_a="+pmp_a_str+"&pmp_b="+pmp_b_str;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    

 // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
  if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  
  delay(300000);
}
