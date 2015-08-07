
/////////////////////////////
////// Envio de variable de ADC 0 a Emoncms 
/////// mas videos - more videos ......https://www.youtube.com/channel/UCv1D6zrC0ZL0PSgM6tdEpPg/videos
///////no olvides suscribirse

#include <SoftwareSerial.h>
 
#define DEBUG true
SoftwareSerial esp8266(10,11);      /////libreria software serial

 /// remplazar   apikey y direccion tcp ip de host emoncms
//#define APIKEY "6a36f95a048ff8ba64f5c0eb18204381" //  api key emoncms
//#define TARGET_IP "192.168.1.58"///         //local  direccion IP o Hosting  Emoncms


#define TARGET_PORT "80"          ///  puerto 80


#define ID "MAIRA"  //name of wireless access point to connect to   - Red de inhalambrica wifi 
#define PASS "3113884544"  //wifi password                          - Contraseña de Wifi 


                             
                             
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different
   
      ///inicializacion y configuracion de modulo ESP8266

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
  
      
//////////////////////////////////////////////////////////////////////////
   // read the input on analog pin 0:
  int sensorValue_batt = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage_Batt = (sensorValue_batt * (5.0 / 1023.0)*2);
  // print out the value you read:
    Serial.println("voltage_Batt");
  Serial.println(voltage_Batt);
  
  
     
          ///CONVERT  FLOAT TO CHAR  + CHAR TO STRING 
     
  
      char outstr1[15];
      dtostrf(voltage_Batt,4, 2, outstr1);   //float to char  4 numero de caracteres  3 cifras sin espacio
      String valor1 = outstr1;   // char to string
      
      
     /// encabezado de envio a emoncms       
    
    String webpage = "AT+CIPSTART=\"TCP\",\""; 
    webpage += TARGET_IP;
    webpage += "\",80\r\n";   
      
    sendData(webpage,1000,DEBUG);        


       ///////post con cadena + variables 
       
    String webpage1 = "GET /emoncms/input/post.json?json={Volt_adc0_arduino:"+ valor1 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n";
    webpage1+="Host: ";
    webpage1+=TARGET_IP;
    webpage1+="\r\n";
    webpage1+="User-Agent: test\r\n"    ;
    webpage1+="\r\n";   
    
    
       //// envia   comando AT + cantidad de caracteres   
      
     String cipsend = "AT+CIPSEND=";     
     cipsend+= webpage1.length();
     cipsend+="\r\n";     
      
     sendData(cipsend,1000,DEBUG);
     sendData(webpage1,1000,DEBUG);   
                
                
        //// comando AT de cierre de conexion            
     sendData("AT+CIPCLOSE=0\r\n",1500,DEBUG);
         
         
         
                  
         
              /// DELAY para reenvio de datos a emoncms
     delay(3000);  //3 seg


 
}
 
 
 //// rutina de depuracion via puerto serial
 
 
 
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
