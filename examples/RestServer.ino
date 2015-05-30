// #define N_LOG
// #define N_DEBUG

#include <RestServer.h>
#include <UIPEthernet.h>
#include <Log.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);
RestServer rest(server);

void digital(char * params = "") {
  rest.add("hello", "world");
  rest.add("lorem", "ipsum");
}

void analog(char * params = "") {
  rest.add("random-text", "Lorem ipsum");
  rest.add("test", (int)1);
  rest.add("lorem", "ipsum");
}

void setup() {
  // Open serial communications and wait for port to open:
  LOG_SETUP();
  LOG("Init...");
  
  // Start Ethernet lib
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(10,0,0,2);
  Ethernet.begin(mac, ip);
  
  // // Start Ethernet server
  server.begin();
  LOG("Server is at ");
  LOG( Ethernet.localIP() );

  // // Add routes to the REST Server
  rest.addRoute("*", "/digital", digital);
  // rest.addRoute("GET", "/analog", analog);
}

void loop() {
  rest.run();
}