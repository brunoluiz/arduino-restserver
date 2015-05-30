#include "RestServer.h"

RestServer::RestServer(EthernetServer& server): server_(server), routesIndex_(0), bufferIndex_(0) {
  // server_ = server;
}

void RestServer::run() {
  client_ = server_.available();
  if (client_) {
    JSON_START();
    check();
    bufferIndex_--;
    JSON_CLOSE();

    send(8, 0);

    client_.stop();

    reset();
  }
}

void RestServer::reset() {
  memset(&buffer_[0], 0, sizeof(buffer_));
  bufferIndex_ = 0;
}

void RestServer::addRoute(char * method, char * route, void (*f)(char * params) ) {
  // memcpy(routes_[routesIndex_].name, route, strlen(route)+1);
  routes_[routesIndex_].method   = method;
  routes_[routesIndex_].name     = route;
  routes_[routesIndex_].callback = f;
  
  DLOG( "Route added:" );
  DLOG( routes_[routesIndex_].name );
  routesIndex_++;
}

void RestServer::addToBuffer(char * value) {
  for (int i = 0; i < strlen(value); i++){
    buffer_[bufferIndex_+i] = value[i];  
  }
  bufferIndex_ = bufferIndex_ + strlen(value);
}

void RestServer::add(char* name, char * value) {
  char buffer_aux[255] = {0};
  uint16_t idx = 0;

  buffer_aux[idx++] = '"';
  for (int i = 0; i < strlen(name); i++){
    buffer_aux[idx++] = name[i];
  }
  buffer_aux[idx++] = '"';

  buffer_aux[idx++] = ':';
  buffer_aux[idx++] = '"';
  for (int i = 0; i < strlen(value); i++){
    buffer_aux[idx++] = value[i];  
  }
  buffer_aux[idx++] = '"';
  buffer_aux[idx++] = ',';

  addToBuffer(buffer_aux);
}

// Add to output buffer_
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(CORE_WILDFIRE)
void RestServer::add(char* name, String& value){
  for (int i = 0; i < value.length(); i++){
    buffer_[bufferIndex_+i] = value[i];  
  }
  bufferIndex_ = bufferIndex_ + value.length();
}
#endif

// Add to output buffer_
void RestServer::add(char* name, uint16_t value){
  char number[10];
  itoa(value,number,10);
  
  add(name, number);
}

// Add to output buffer_
void RestServer::add(char* name, int value){
  char number[10];
  itoa(value,number,10);
  
  add(name, number);
}

// Add to output buffer_ (Mega & ESP only)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(ESP8266) || defined(CORE_WILDFIRE)
void RestServer::add(char* name, float value){
  char number[10];
  dtostrf(value, 5, 2, number);
  
  add(name, number);
}
#endif

void RestServer::send(uint8_t chunkSize, uint8_t wait_time) {
  // Send the HTTP Header
  client_.println(HTTP_COMMON_HEADER);

  // Send all of it
  if (chunkSize == 0)
    client_.print(buffer_);

  // Send chunk by chunk #####################################

  // Max iteration
  uint8_t max = (int)(bufferIndex_/chunkSize) + 1;

  // Send data
  for (uint8_t i = 0; i < max; i++) {
    char buffer_aux[chunkSize+1];
    memcpy(buffer_aux, buffer_ + i*chunkSize, chunkSize);
    buffer_aux[chunkSize] = '\0';

    client_.print(buffer_aux);

    // Wait for client_ to get data
    delay(wait_time);
  }
}

void RestServer::check() {
  char route[CHECK_MAX_ROUTE_LENGTH] = {0};
  bool routePrepare = false;
  bool routeReady = false;
  uint8_t r = 0;

  char query[CHECK_MAX_QUERY_LENGTH] = {0};
  bool queryPrepare = false;
  bool queryReady = false;
  uint8_t q = 0;
  
  char method[CHECK_MAX_ROUTE_LENGTH] = {0};
  bool methodReady = false;
  uint8_t m = 0;

  bool currentLineIsBlank = true;
  char c;
  while ( client_.connected() && client_.available() ) {
    c = client_.read();
    // DLOGChar(c);

    // Start end of line process ////////////////
    // if you've gotten to the end of the line (received a newline
    // character) and the line is blank, the http request has ended,
    // so you can send a reply
    if (c == '\n' && currentLineIsBlank)
      break;
    if (c == '\n')
      currentLineIsBlank = true; // you're starting a new line
    else if (c != '\r')
      currentLineIsBlank = false; // you've gotten a character on the current line
    // End end of line process //////////////////

    // Start route catch process ////////////////
    if(c == '/' && !routePrepare)
      routePrepare = true;

    if((c == ' ' || c == '?') && routePrepare)
      routeReady = true;

    if(routePrepare && !routeReady)
      route[r++] = c;
    // End route catch process //////////////////

    // Start query catch process ////////////////
    if(c == ' ' && queryPrepare)
      queryReady = true;

    if(queryPrepare && !queryReady)
      query[q++] = c;

    if(c == '?' && !queryPrepare)
      queryPrepare = true;
    // End query catch process /////////////////

    // Start method catch process ///////////////
    if(c == ' ' && !methodReady)
      methodReady = true;

    if(!methodReady)
      method[m++] = c;
    // End method catch process /////////////////

  }

  for(int i = 0; i < routesIndex_; i++) {
      // Check if the routes names matches
      if(memcmp( route, routes_[i].name, sizeof(routes_[i].name) ) != 0)
        continue;

      // Check if the HTTP METHOD matters for this route
      if(memcmp( routes_[i].method, "*", sizeof(routes_[i].method) ) != 0) {
        // If it matters, check if the methods matches
        if(memcmp( method, routes_[i].method, sizeof(routes_[i].method) ) != 0)
          continue;
      }

      // Route callback (function)
      DLOG("Route callback");
      DLOG(route);
      routes_[i].callback(query);
  }

}