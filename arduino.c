#include <SPI.h>
#include <Ethernet.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

byte mac[] = { 0x54, 0xEE, 0x75, 0xC2, 0xBA, 0x21 };
float flat,flon;
char server[] = "192.168.0.10";
IPAddress ip(192,168,0,177); 
EthernetClient client; 
SoftwareSerial SIM900(7, 8);
int vibr_pin =3; 
TinyGPS gps;



void setup(){
   pinMode(vibr_pin, INPUT); // Set vibr_Pin input for measurment
   Serial.begin(9600);
   SIM900.begin(9600); // Start GSM
   
   if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      Ethernet.begin(mac, ip);
   }
   delay(1000);
}



void loop(){
   int val = 1;
   bool crash = false;
   // Read vibr_pin for toggle (Crash)
   val = digitalRead(vibr_pin);
   delay(100);
   // New GPS data trigger
   bool newData = false;
   // Trigger Crash
   if(val == 1)
      crash = true;
   else
      crash = false;
   // When crash occurs, send GPS data over GSM
   if(crash){
   // For one second we parse GPS data and report some key values
     for (unsigned long start = millis(); millis() - start < 1000;){
      while (Serial.available()){
         char c = Serial.read();
         if (gps.encode(c))
           newData = true;
          }
    }
  
   // Sending GPS data
   if (newData){
   unsigned long age;
   gps.f_get_position(&flat, &flon, &age);
   
   SIM900.print("AT+CMGF=1\r");
   delay(300);


   
   SIM900.print("Emergency! Crash detected\r\n");
   delay(200);
   SIM900.println("Mr Raghav");
   delay(200);
   // recipient's mobile number with country code
   SIM900.println("AT + CMGS = \"+91 9000000000\"\r\n");
   delay(300);
   SIM900.print("Latitude = ");
   SIM900.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6
  );
   SIM900.print(", Longitude = ");
   SIM900.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6
  );

   SIM900.println(flat,6);
   SIM900.println(flon,6);
  
   delay(200);

  // End AT command with a ^Z, ASCII code 26
   SIM900.println((char)26);
   delay(200);
   SIM900.println();


   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    Serial.print("GET /testcode/dht.php?name=");
    client.print("GET /testcode/dht.php?name=");     //YOUR URL
    Serial.println("Mr Raghav");
    client.print("Mr Raghav");

    Serial.print("&mobile=");
    client.print("&mobile=");     //YOUR URL
    long int number = 9000000000;
    Serial.println(number);
    client.print(number);

    
    Serial.print("&lat=");
    client.print("&lat=");     //YOUR URL
    Serial.println(flat,6);
    client.print(flat,6);
    client.print("&lon=");
    Serial.println("&lon=");
    client.print(flon);
    Serial.println(flon);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 192.168.0.10");
    client.println("Connection: close");
    client.println();
  }
  
}}}