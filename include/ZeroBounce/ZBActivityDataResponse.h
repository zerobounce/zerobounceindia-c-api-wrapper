#ifndef ZBACTIVITYDATARESPONSE_H
#define ZBACTIVITYDATARESPONSE_H

#include <json-c/json.h>

struct ZBActivityDataResponse {
    int found;
    int activeInDays;
};

struct ZBActivityDataResponse new_zb_activity_data_response();

char* zb_activity_data_response_to_string(struct ZBActivityDataResponse response);

struct ZBActivityDataResponse zb_activity_data_response_from_json(const json_object* j);

#endif