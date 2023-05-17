{\rtf1\ansi\ansicpg1252\cocoartf2709
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fmodern\fcharset0 Courier;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;\red255\green255\blue255;}
{\*\expandedcolortbl;;\cssrgb\c0\c1\c1;\cssrgb\c100000\c100000\c99985\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww21860\viewh20160\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs26 \cf2 \cb3 ## Zero Bounce C API Wrapper\
Este envoltorio de API contiene funciones para interactuar f\'e1cilmente con la API de ZeroBounce.\
Puedes encontrar m\'e1s informaci\'f3n sobre ZeroBounce en la [documentaci\'f3n oficial](https://www.zerobounce.net/docs/).\
\
## INSTALACI\'d3N\
Antes de instalar el envoltorio, aseg\'farate de tener instalado `CMake` en tu sistema. Puedes descargarlo desde [aqu\'ed](https://cmake.org/download/).\
\
Puedes instalarlo utilizando CMake con `FetchContent`. Agrega lo siguiente a tu archivo `CMakeLists.txt`:\
```cmake\
include(FetchContent)\
FetchContent_Declare(\
    ZeroBounce\
    GIT_REPOSITORY https://github.com/zerobounce/zerobounce-c-api-wrapper.git\
    GIT_TAG <DESIRED_TAG>\
)\
FetchContent_MakeAvailable(ZeroBounce)\
```\
\
Esto generar\'e1 el objetivo `ZeroBounce` que puedes enlazar de la forma t\'edpica:\
\
```cmake\
target_link_libraries(nombre_de_tu_objetivo PRIVATE ZeroBounce)\
```\
\
Si est\'e1s utilizando Visual Studio Code, puedes construir la biblioteca utilizando las extensiones `CMake` y `Cmake Tools`. Despu\'e9s de instalar las extensiones, simplemente abre los comandos con `CTRL + SHIFT + P` y usa `CMake: Configure` seguido de `CMake: Build`.\
\
Alternativamente, tambi\'e9n puedes utilizar los comandos de la CLI en el directorio ra\'edz de tu proyecto.\
\
#### LINUX\
\
```bash\
mkdir build\
cmake -S . -B build -G "Unix Makefiles"\
cmake --build build\
```\
\
#### WINDOWS\
\
Aseg\'farate de tener `MinGW` instalado en tu sistema. Puede instalarse siguiendo los pasos [aqu\'ed](https://code.visualstudio.com/docs/cpp/config-mingw).\
\
```bash\
mkdir build\
cmake -S . -B build -G "MinGW Makefiles"\
cmake --build build\
```\
\
Ten en cuenta que es posible que la biblioteca requiera algunos archivos .dll (que se encuentran en el directorio `build`) para funcionar correctamente en tu proyecto.\
\
## USO\
Incluye la biblioteca en tu archivo:\
```c\
#include <ZeroBounce/ZeroBounce.h>\
``` \
\
Inicializa el envoltorio con tu clave de API:\
```c\
ZeroBounce* zb = zero_bounce_get_instance();\
zero_bounce_initialize(zb, "<TU_CLAVE_DE_API>");\
```\
## Ejemplos\
Luego puedes usar cualquiera de las funciones de envoltura, por ejemplo:\
\
##### Verifica cu\'e1ntos cr\'e9ditos te quedan en tu cuenta\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_credits(ZBCreditsResponse response) \{\
    printf("%s\\n\\n", zb_credits_response_to_string(&response));\
\}\
\
get_credits(zb, on_success_credits, on_error);\
```\
\
##### Verifica el uso de la API para un per\'edodo de tiempo determinado\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_api_usage(ZBGetApiUsageResponse response) \{\
    printf("%s\\n\\n", zb_get_api_usage_response_to_string(&response));\
\}\
\
struct tm start_date;\
start_date.tm_year = 118;\
start_date.tm_mon = 0;\
start_date.tm_mday = 1;\
\
struct tm end_date;\
end_date.tm_year = 123;\
end_date.tm_mon = 11;\
end_date.tm_mday = 12;\
\
get_api_usage(zb, start_date, end_date, on_success_api_usage, on_error);\
```\
\
##### Valida una direcci\'f3n de correo electr\'f3nico\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_validate(ZBValidateResponse response) \{\
    printf("%s\\n\\n", zb_validate_response_to_string(&response));\
\}\
\
char* email = "valid@example.com";   // La direcci\'f3n de correo electr\'f3nico que deseas validar\
char* ip_address = "127.0.0.1";     // La direcci\'f3n IP desde la cual se registr\'f3 el correo electr\'f3nico (opcional)\
\
validate_email(zb, email, ip_address, on_success_validate, on_error);\
```\
\
##### Valida un lote de correos electr\'f3nicos\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_validate_batch(ZBValidateBatchResponse response) \{\
    printf("%s\\n\\n", zb_validate_batch_response_to_string(&response));\
\}\
\
EmailToValidateVector vector = email_to_validate_vector_init();\
\
ZBEmailToValidate email_1 = \{"valid@example.com", "1.1.1.1"\};\
email_to_validate_vector_append(&vector, email_1);\
\
ZBEmailToValidate email_2 = \{"invalid@example.com", ""\};\
email_to_validate_vector_append(&vector, email_2);\
\
validate_email_batch(zb, vector, on_success_validate_batch, on_error);\
\
email_to_validate_vector_free(&vector);\
```\
\
##### Utiliza el punto de conexi\'f3n de la API de actividad para obtener informaci\'f3n sobre el compromiso general de los suscriptores con los correos electr\'f3nicos\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_activity_data(ZBActivityDataResponse response) \{\
    printf("%s\\n\\n", zb_activity_data_response_to_string(&response));\
\}\
\
char* email = "valid@example.com";    // Direcci\'f3n de correo electr\'f3nico del suscriptor\
\
get_activity_data(zb, email, on_success_activity_data, on_error);\
```\
\
##### La API sendfile permite al usuario enviar un archivo para validar correos electr\'f3nicos a granel\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\
\
\}\
\
void on_success_send_file(ZBSendFileResponse response) \{\
    printf("%s\\n\\n", zb_send_file_response_to_string(&response));\
\}\
\
char* file_path = "<RUTA_DEL_ARCHIVO>"; 			// Ruta del archivo CSV o TXT\
int email_address_column = 3; 			// \'cdndice de la columna "email" en el archivo. El \'edndice comienza en 1\
SendFileOptions options = new_send_file_options();	// Opciones adicionales\
\
options.returnUrl = "https://dominio.com/llamado/despues/procesamiento/solicitud";\
options.firstNameColumn = 4;		// \'cdndice de la columna "nombre" en el archivo\
options.lastNameColumn = 5;		// \'cdndice de la columna "apellido" en el archivo\
options.genderColumn = 6;		// \'cdndice de la columna "g\'e9nero" en el archivo\
options.ipAddressColumn = 7;		// \'cdndice de la columna "direcci\'f3n IP" en el archivo\
options.hasHeaderRow = true;		// Si esto es `true`, la primera fila se considera encabezados de tabla\
options.removeDuplicate = true;	// Si esto es `true`, los correos electr\'f3nicos duplicados se eliminar\'e1n\
\
send_file(zb, file_path, email_address_column, options, on_success_send_file, on_error);\
```\
\
##### La API getfile permite a los usuarios obtener el archivo de resultados de validaci\'f3n para el archivo enviado utilizando la API sendfile\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_get_file(ZBGetFileResponse response) \{\
    printf("%s\\n\\n", zb_get_file_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";				// El ID de archivo devuelto al llamar a la API sendfile\
char* local_download_path = "<RUTA_DESCARGA_ARCHIVO>"; 	// La ubicaci\'f3n donde se guardar\'e1 el archivo descargado\
\
get_file(zb, file_id, local_download_path, on_success_get_file, on_error);\
```\
\
##### Verifica el estado de un archivo cargado a trav\'e9s del m\'e9todo "sendFile"\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_file_status(ZBFileStatusResponse response) \{\
    printf("%s\\n\\n", zb_file_status_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";		// El ID de archivo devuelto al llamar a la API sendfile\
\
file_status(zb, file_id, on_success_file_status, on_error);\
```\
\
##### Elimina el archivo que se envi\'f3 mediante la API sendfile. El archivo solo se puede eliminar cuando su estado es _`Complete`_\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_delete_file(ZBDeleteFileResponse response) \{\
    printf("%s\\n\\n", zb_delete_file_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";		// El ID de archivo devuelto al llamar a la API sendfile\
\
delete_file(zb, file_id, on_success_delete_file, on_error);\
```\
\
##### La API scoringSendfile permite al usuario enviar un archivo para validar correos electr\'f3nicos a granel\
\
\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_send_file(ZBSendFileResponse response) \{\
    printf("%s\\n\\n", zb_send_file_response_to_string(&response));\
\}\
\
char* file_path = "<RUTA_DEL_ARCHIVO>";			// Ruta del archivo CSV o TXT\
int email_address_column = 3; 			// \'cdndice de la columna "email" en el archivo. El \'edndice comienza en 1\
SendFileOptions options = new_send_file_options();	// Opciones adicionales\
\
options.returnUrl = "https://dominio.com/llamado/despues/procesamiento/solicitud";\
options.hasHeaderRow = true;            // Si esto es `true`, la primera fila se considera encabezados de tabla\
\
scoring_send_file(zb, file_path, email_address_column, options, on_success_send_file, on_error);\
```\
\
##### La API scoringGetFile permite a los usuarios obtener el archivo de resultados de validaci\'f3n para el archivo enviado utilizando la API scoringSendfile\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_get_file(ZBGetFileResponse response) \{\
    printf("%s\\n\\n", zb_get_file_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";                       	// El ID de archivo devuelto al llamar a la API scoringSendfile\
char* local_download_path = "<RUTA_DESCARGA_ARCHIVO>";  	// La ubicaci\'f3n donde se guardar\'e1 el archivo descargado\
\
scoring_get_file(zb, file_id, local_download_path, on_success_get_file, on_error);\
```\
\
##### Verifica el estado de un archivo cargado a trav\'e9s del m\'e9todo "scoringSendFile"\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_file_status(ZBFileStatusResponse response) \{\
    printf("%s\\n\\n", zb_file_status_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";                    // El ID de archivo devuelto al llamar a la API scoringSendfile\
\
scoring_file_status(zb, file_id, on_success_file_status, on_error);\
```\
\
##### Elimina el archivo que se envi\'f3 mediante la API scoring scoringSendfile. El archivo solo se puede eliminar cuando su estado es _`Complete`_\
```c\
void on_error(ZBErrorResponse error_response) \{\
    printf("%s\\n\\n", zb_error_response_to_string(&error_response));\
\}\
\
void on_success_delete_file(ZBDeleteFileResponse response) \{\
    printf("%s\\n\\n", zb_delete_file_response_to_string(&response));\
\}\
\
char* file_id = "<ID_DEL_ARCHIVO>";                       // El ID de archivo devuelto al llamar a la API scoringSendfile\
\
scoring_delete_file(zb, file_id, on_success_delete_file, on_error);\
```\
\
## DESARROLLO\
\
Se recomienda realizar el desarrollo en Linux.\
\
Despu\'e9s de verificar el repositorio, compila la biblioteca y luego ejecuta las pruebas.\
\
```bash\
./build/bin/ZeroBounceTests\
```\
\
Deber\'edas ver una salida como esta:\
\
```bash\
24 pruebas 0 fallos 0 ignoradas\
OK\
```\
}