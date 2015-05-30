#ifndef restsettings_h
#define restsettings_h


#ifndef ROUTES_TOTAL
#define ROUTES_TOTAL 5
#endif

#ifndef ROUTES_LENGHT
#define ROUTES_LENGHT 25
#endif

#ifndef CHECK_MAX_ROUTE_LENGTH
#define CHECK_MAX_ROUTE_LENGTH 45
#endif

#ifndef CHECK_MAX_METHOD_LENGTH
#define CHECK_MAX_METHOD_LENGTH 6
#endif

#ifndef CHECK_MAX_QUERY_LENGTH
#define CHECK_MAX_QUERY_LENGTH 30
#endif

#ifndef HTTP_COMMON_HEADER
#define HTTP_COMMON_HEADER "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: POST, GET, PUT, OPTIONS\r\nContent-Type: application/json\r\nConnection: close\r\n"
#endif HTTP_COMMON_HEADER

#ifndef OUTPUT_BUFFER_SIZE
#define OUTPUT_BUFFER_SIZE 250
#endif OUTPUT_BUFFER_SIZE

#define JSON_START() addToBuffer("{ ");
#define JSON_CLOSE() addToBuffer(" }");


#endif