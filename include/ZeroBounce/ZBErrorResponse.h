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

int compare_zb_error_response(const ZBErrorResponse* response1, const ZBErrorResponse* response2);

#endif