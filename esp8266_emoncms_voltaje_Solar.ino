
#include <OneWire.h>
#include <SoftwareSerial.h>
 
#define DEBUG true
SoftwareSerial esp8266(10,11);



//#define APIKEY "6a36f95a048ff8ba64f5c0eb18204381" //local
///#define TARGET_IP "192.168.1.66"///         //local


#define TARGET_PORT "80"     
#define TARGET_NAME "jhon"    


#define ID "MAIRA"  //name of wireless access point to connect to
#define PASS "3113884544"  //wifi password


OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)
float celsius, fahrenheit,celsius_web;
int Tem_int =0;
                             
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
   
   

    sendData("AT+RST\r\n",1000,DEBUG); // 
     
    sendData("AT+CWMODE=1\r\n",1000,DEBUG); // 
    
      String cmd="AT+CWJAP=\"";  
      cmd+=ID;
      cmd+="\",\"";
      cmd+=PASS;
      cmd+="\"";      
     sendData( cmd+"\r\n",1000,DEBUG); // 
    
    
     sendData("AT+CIPMUX=0\r\n",1000,DEBUG); // 
    
}
 
void loop()
{ 

  /////////////////////////////////////////////////////////////////////////
  
  
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
  //  Serial.println("No more addresses.");
   /// Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  

  for( i = 0; i < 8; i++) {  
    addr[i];

  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
//  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:

      type_s = 1;
      break;
    case 0x28:

      type_s = 0;
      break;
    case 0x22:
    //  Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
    //  Serial.println("Device is not a DS18x20 family device.");
      return;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);      
  
  delay(1000);    

//  delay(1000);    
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         

  for ( i = 0; i < 9; i++) {       
    data[i] = ds.read();
  }
  OneWire::crc8(data, 8); 
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; 
    if (data[7] == 0x10) {      
      raw = (raw & 0xFFF0) + 12 - data[6];    }
  } else {
    byte cfg = (data[4] & 0x60);

    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    
  }
  celsius = (float)raw / 16.0;
  celsius_web = celsius;
  
  ////////////////////////////////////////////////
  
      Serial.print("temperatura actual =");  
     Serial.println(celsius_web); 
     
     
          ///CONVERT  FLOAT TO CHAR  + CHAR TO STRING 
     
  
        char outstr[15];
      dtostrf(celsius_web,5, 2, outstr);   //float to char
      String valor = outstr;   // char to string
//////////////////////////////////////////////////////////////////////////
   // read the input on analog pin 0:
  int sensorValue_batt = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage_Batt = (sensorValue_batt * (5.0 / 1023.0)*2);
  // print out the value you read:
    Serial.println("voltage_Batt");
  Serial.println(voltage_Batt);
  
  
   int sensorValue_panel = analogRead(A1);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage_Panel = (sensorValue_panel * (5.0 / 1023.0)*2);
  // print out the value you read:
    Serial.println("voltage_Panel");
  Serial.println(voltage_Panel);
  
  
     int sensorValue_current = analogRead(A2);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
float Volt_Panel = sensorValue_current * (5.0 / 1023.0);
  // print out the value you read:
      Serial.println("current_Panel");
  
  float current_Panel=   (Volt_Panel/5.0);
 Serial.println(current_Panel);

     
          ///CONVERT  FLOAT TO CHAR  + CHAR TO STRING 
     
  
        char outstr1[15];
      dtostrf(voltage_Batt,4, 2, outstr1);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor1 = outstr1;   // char to string
      
      
              char outstr2[15];
      dtostrf(voltage_Panel,4, 2, outstr2);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor2 = outstr2;   // char to string
      
      
              char outstr3[15];
      dtostrf(current_Panel,4, 2, outstr3);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor3 = outstr3;   // char to string

 
    

    
    
    String webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        

   // String webpage1 = "GET /emoncms/input/post.json?json={temperatura_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    String webpage1 = "GET /emoncms/input/post.json?json={Voltaje_Batt_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
      
     String cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
                
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         
         
         //////////////////////////////////////////////////////////////////////////////////////////////////////
         
         
   webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        

   // String webpage1 = "GET /emoncms/input/post.json?json={temperatura_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
     webpage1 = "GET /emoncms/input/post.json?json={Voltaje_Panel_arduino:"+ valor2 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
      
      cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
                
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         
         ////////////////////////////////////////////////////////////////////////////////////////
         
         webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        

   // String webpage1 = "GET /emoncms/input/post.json?json={temperatura_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
     webpage1 = "GET /emoncms/input/post.json?json={Corriente_panel_arduino:"+ valor3 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
      
      cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
                
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         
         ////////////////////////////////////////////////////////////////////////  
         
      webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        

   // String webpage1 = "GET /emoncms/input/post.json?json={temperatura_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
     webpage1 = "GET /emoncms/input/post.json?json={Temperatura_arduino:"+ valor +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
      
      cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
                
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         
         /////////////////////////////////////////////////////////////////// 
         
         
         
         
         
         
         
         
         
         
         
         
     
     delay(70000);  //5min


///delay(3000);  //5min

 
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
