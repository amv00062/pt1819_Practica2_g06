#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define HELO "HELO"  // 

#define QUIT  "QUIT" // Finalizacion de la conexion de aplicacion


// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "220"
#define OKDATA "354"
#define ER  "5"
#define OKW "250"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_WELC 0
#define S_HELO 1
#define S_MAIL 2
#define S_RCPT 3
#define S_DATA 4
#define S_MENS 5
#define S_FINM 6
#define S_QUIT 7
#define S_EXIT 8

//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT  25



// #define mod "mod"
#define MF "mail from:"
#define RCPT "rcpt to:"
#define DATA "data"
#define FMENS "."
