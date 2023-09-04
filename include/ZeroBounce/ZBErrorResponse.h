#ifndef ZBERRORRESPONSE_H
#define ZBERRORRESPONSE_H

#include <stdbool.h>

#include <json.h>

#include "utils.h"

/**
 * The struct used for when a request throws an error. This struct was introduced in order to provide
 * a standardized way of handling the error responses that a request can return.
 *
 * If the error JSON received from the server includes the words "error" or "message", then the
 * values of those keys will be added to the [errors] StringVector. If the error is not a JSON dictionary,
 * then JSON String will be added to the [errors] StringVector.
 *
 * If any type of messages are received, then they will be added according to the same rule above
 * after the errors found above.
 */
typedef struct {
    bool success;
    StringVector errors;
} ZBErrorResponse;

/**
 * @brief Function used to initialize a new ZBErrorResponse.
 *
 * @return ZBErrorResponse new instance
 */
ZBErrorResponse new_zb_error_response();

/**
 * @brief Function used to serialize a ZBErrorResponse.
 *
 * @param response ZBErrorResponse pointer
 * @return char* serialization
 */
char* zb_error_response_to_string(ZBErrorResponse* response);

/**
 * @brief Function used to create ZBErrorResponse from an error string.
 *
 * @param error error string
 * @return ZBErrorResponse new instance
 */
ZBErrorResponse parse_error(const char* error);

/**
 * @brief Function used to compare ZBErrorResponse instances.
 *
 * @param response1 ZBErrorResponse pointer
 * @param response2 ZBErrorResponse pointer
 * @return int 1 if the instances are equal, 0 otherwise
 */
int compare_zb_error_response(const ZBErrorResponse* response1, const ZBErrorResponse* response2);

#endif