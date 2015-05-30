#ifndef log_h
#define log_h

#ifndef N_LOG
#define LOG_SETUP() Serial.begin(38400);
#define LOG(var) Serial.println(var);
#define LOGChar(var) Serial.print(var);
#else
#define LOG_SETUP() ;
#define LOG(var) ;
#define LOGChar(var) ;
#endif

#ifndef N_DEBUG || N_LOG
#define DLOG(var) Serial.println(var);
#define DLOGChar(var) Serial.print(var);
#else
#define DLOG(var) ;
#define DLOGChar(var) Serial.print(var);
#endif

#endif