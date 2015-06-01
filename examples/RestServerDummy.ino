// #define N_LOG
// #define N_DEBUG

#include <RestServer.h>
#include <UIPEthernet.h>
#include <Log.h>

EthernetServer server(80);
RestServer rest(server);  // You need to pass the EthernetServer reference to the RestServer

void digital(char * params = "") {
  rest.addData("hello", "world");
  rest.addData("lorem", "ipsum");
}

void analog(char * params = "") {
  rest.addData("random-text", "Lorem ipsum");
  rest.addData("test", (int)1);
  rest.addData("lorem", "ipsum");
}

void setup() {
  // Open serial communications and wait for port to open:
  LOG_SETUP();
  LOG("Init...");
  
  // Start Ethernet lib
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  IPAddress ip(10,0,0,2);
  Ethernet.begin(mac, ip);
  
  // Start Ethernet server
  server.begin();
  LOG("Server is at ");
  LOG( Ethernet.localIP() );

  // Add routes to the REST Server
  rest.addRoute(ANY, "/digital", digital);
  rest.addRoute(GET, "/analog", analog);
}

void loop() {
  // Run the RestServer
  rest.run();
}