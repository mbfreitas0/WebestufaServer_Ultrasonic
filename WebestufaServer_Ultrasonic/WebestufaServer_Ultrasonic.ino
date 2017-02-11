
//Programa : Arduino Ethernet Shield W5100 e HC-SR04
//Código Fonte : FILIPEFLOP http://blog.filipeflop.com/arduino/tutorial-ethernet-shield-w5100.html
//Adaptação para uso com o Sensor DHT11: Marcelo | Webestufa
//Baseado no programa exemplo de David A. Mellis e Tom Igoe

#include <Ultrasonic.h>
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>


//Define os parametros para o sensor ultrasonico HC-SR04
#define PINO_TRIGGER  6 //Porta ligada ao pino Trigger do sensor
#define PINO_ECHO     7 //Porta ligada ao pino Echo do sensor
//Inicializa o sensor ultrasonico
Ultrasonic ultrasonic(PINO_TRIGGER, PINO_ECHO);


//Definição dos Parametros para o Sensor DHT11
#define DHTPIN A0 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);


//Definicoes de IP, mascara de rede e gateway
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,99);          //Define o endereco IP
IPAddress gateway(192,168,0,1);      //Define o gateway
IPAddress subnet(255, 255, 255, 0); //Define a máscara de rede
//Inicializa o servidor web na porta 80
EthernetServer server(80);


void setup()
{
  //Inicializa a interface de rede
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  dht.begin();

}


  void loop() {
  
  float cmMsec;
  long microsec = ultrasonic.timing();
  //Le e armazena as informacoes do sensor ultrasonico
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
      int h = dht.readHumidity(); // Variável para receber a umidade
      int t = dht.readTemperature();// Variável para receber a temperatura
  
  //Aguarda conexao do browser  
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == 'n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2"); //Recarrega a pagina a cada 2seg
          client.println();
          client.println("<!DOCTYPE HTML>");
          //INICIO DO CODIGO HTML

          client.println("<HTML>");
          client.println("<head><Webestufa</title></head>");
          client.println("<BODY>");
          client.println("<br><br><center><a href=\"http://www.webestufa.com.br\"> <img align=center width=350px heigh=150px  src=https://lh3.googleusercontent.com/-GGIuiX7uaG4/V5frN70j5cI/AAAAAAAAAh8/h7rBqeX_d_UEqC_EDjHDqFUOU_OcX6sLgCCo/s128/logowebestufa.gif></center></br></br>");
          client.println("<H1><center>Arduino + Ethernet Shield + DHT11 + Sensor Ultrassonico HC-SR04</center></H1>");
          client.println("<br />");
          client.println("<br />");
          //Mostra o estado da porta digital 3
          client.println("<h2><center>");
          int porta_digital = digitalRead(3);
          //Mostra as informacoes lidas pelo sensor ultrasonico
          client.print("<br><br><center>Sensor Ultrasonico :");
          client.print(cmMsec);
          client.print(" cm");
          client.println("</center></h2>");
          client.println("<br>");
          client.println("<h2><center>");
          client.println("Temperatura:");
          client.println(t);
          client.println(" *C");
          client.println(" - ");
          client.println("Umidade:");
          client.println(h);
          client.println("%");
          client.println("</center></h2>");
          client.println("<br>");

          break;
        }
        if (c == 'n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != 'r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();

    
    }
}

