#include "wifi_func.h"

int reset = 0;

int status = WL_IDLE_STATUS;
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;      // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password

WiFiClient wifiClient;
ThingsBoard tb(wifiClient);






void printext(float temperature,float humidity, float internal_temp) {
  TelnetStream.print("La temperatura è: ");
  TelnetStream.print(temperature);
  TelnetStream.println("°C");
  TelnetStream.print("L'umidità è: ");
  TelnetStream.print(humidity);
  TelnetStream.println("%");
  TelnetStream.print("La temperatura interna è: ");
  TelnetStream.print(internal_temp);
  TelnetStream.println("°C");

}

void printwifistatus(){
  if (WiFi.status() == WL_CONNECTED) {
    TelnetStream.print("SSID: ");
    TelnetStream.println(WiFi.SSID());
  
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    TelnetStream.print("IP Address: ");
    TelnetStream.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    TelnetStream.print("signal strength (RSSI):");
    TelnetStream.print(rssi);
    TelnetStream.println(" dBm");
  }
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    TelnetStream.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    TelnetStream.print("Attempting to connect to SSID: ");
    TelnetStream.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  } 
}

void thingsb_aquarium_send(float external_temp,float external_hum,float internal_temp){
  
  if ( !tb.connected() ) {
    reconnect();
  }
  TelnetStream.print("Sending to ThingsBoard node ...");
  
  tb.sendTelemetryFloat("ext_temp", external_temp);
  tb.sendTelemetryFloat("ext_hum", external_hum);
  tb.sendTelemetryFloat("temp_aquarium", internal_temp);
  TelnetStream.println( "[DONE]" );


}



void reconnect(){

  // Attempt to connect (clientId, username, password)
  while ( !tb.connected() ) {
      TelnetStream.print("Connecting to ThingsBoard node ...");
  
    if (tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      TelnetStream.println( "[DONE]" );
    } else {
      TelnetStream.print( "[FAILED]" );
      TelnetStream.println( " : retrying in 5 seconds" );
      // Wait 5 seconds before retrying
      TelnetStream.println(reset);
      delay( 5000 );
      if (reset == 4){
        resetcustom();
      }  
      reset++; 
    }
  }
}

//reset function
void resetcustom(){
  Serial.println("resetting");
  NVIC_SystemReset(); 
}


void webserver_aquarium(WiFiServer server){
  Serial.println("server");
  wifiClient = server.available();

  if (wifiClient) {
    Serial.println("Nuovo client");

    boolean currentLineIsBlank = true;
    while (wifiClient.connected()) {
      if (wifiClient.available()) {
        char c = wifiClient.read();
        Serial.write(c);
    

        if (c == '\n' && currentLineIsBlank) {
  
          wifiClient.println("HTTP/1.1 200 OK");
          wifiClient.println("Content-Type: text/html");
          wifiClient.println("Connection: close");
          wifiClient.println("Refresh: 5");
          wifiClient.println();
          wifiClient.println("<!DOCTYPE HTML>");
          wifiClient.println("<html>");
    
          //client.print("<h1>" + msg + "</h1>");
          wifiClient.print("<h1>Claudio</h1>");
          
          wifiClient.println("</html>");
          break;
        }
      }
    }
  }
  //wifiClient.stop();
  Serial.println("Client disconnesso");
}

