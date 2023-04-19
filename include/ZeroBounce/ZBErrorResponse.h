#ifndef ZBERRORRESPONSE_H
#define ZBERRORRESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

#include "utils.h"

typedef struct {
    bool success;
    StringVector errors;
} ZBErrorResponse;

ZBErrorResponse new_zb_error_response();

char* zb_error_response_to_string(ZBErrorResponse* response);

ZBErrorResponse parse_error(const char* error);

#endif