// #define N_LOG
#define N_DEBUG

#include <RestServer.h>
#include <UIPEthernet.h>
#include <Log.h>

EthernetServer server(80);
RestServer rest(server); // You need to pass the EthernetServer reference to the RestServer

/* *************************************************************** */
/* Relay Methods and Settings                                      */
/* *************************************************************** */

// Relay Pin is at Digital port 7
// Relay States have inverted logic, so to return data I used the "!"
#define RELAY_ON    LOW
#define RELAY_OFF  HIGH
int relayPin = 7;
int relayState = RELAY_OFF;

void setRelay(char * params = "") {
  if( strncmp( params, "active=1", 8 ) == 0) {
    relayState = RELAY_ON;
    DLOG("on");
    digitalWrite(relayPin, relayState);
  }
  else if( strncmp( params, "active=0", 8 ) == 0) {
    relayState = RELAY_OFF;
    DLOG("off");
    digitalWrite(relayPin, relayState);
  }

  rest.addData("active", !relayState);
}

void getRelay(char * params = "") {
  rest.addData("active", !relayState);
}
/* *************************************************************** */

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

  // Add routes to the REST Server
  rest.addRoute(POST, "/relayPin", setRelay);
  rest.addRoute(GET,  "/relayPin", getRelay);

  // Setup relayPin and other sensors
  pinMode(relayPin, OUTPUT); // Set Pin7 as output 
  digitalWrite(relayPin, RELAY_OFF);    //Turn off relayPin 
}

void loop() {
  // Run the RestServer
  rest.run();
}