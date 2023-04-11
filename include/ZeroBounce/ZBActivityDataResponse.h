#ifndef ZBACTIVITYDATARESPONSE_H
#define ZBACTIVITYDATARESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

typedef struct {
    bool found;
    int active_in_days;
} ZBActivityDataResponse;

ZBActivityDataResponse new_zb_activity_data_response();

char* zb_activity_data_response_to_string(ZBActivityDataResponse* response);

ZBActivityDataResponse zb_activity_data_response_from_json(const json_object* j);

#endif