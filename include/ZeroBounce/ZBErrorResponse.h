#ifndef ZBERRORRESPONSE_H
#define ZBERRORRESPONSE_H

#include <json-c/json.h>

#include "utils.h"

typedef struct {
    int success;
    StringVector errors;
} ZBErrorResponse;

ZBErrorResponse new_zb_error_response();

char* zb_error_response_to_string(ZBErrorResponse* response);

ZBErrorResponse parse_error(const char* error);

#endif