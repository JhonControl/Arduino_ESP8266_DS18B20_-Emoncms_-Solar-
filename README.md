 Arduino_ESP8266_DS18B20_-Emoncms_-Solar-
 
Monitoreo Solar Emoncms

Monitorear variable a campo abierto y visualizar variables en Emoncms implementando  Arduino ESP8266, 
se adecuan minipaneles solares y una bateria de 6VDC.

1 Lectura de Sensor  DS18B20
2 Se realiza la lectura de 3 entradas analogas, dado que las variables monitoreadas sobre pasan el nivel de voltaje permitido por los ADC
3 Conversion de ADC a variables (Voltaje y corriente)
4 Conversion de flotantes a String para envio de JSON a Emoncms
  Ejemplo :GET /emoncms/input/post.json?json={Voltaje_Panel_arduino:"+ valor2 +"}&apikey="+APIKEY+"  HTTP/1.0\r\n"
5 Se implementa un Delay para el envio ciclico de datos a Emoncms
