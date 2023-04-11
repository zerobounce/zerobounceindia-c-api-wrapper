#include <stdio.h>

#include "ZeroBounce/ZBActivityDataResponse.h"
#include "ZeroBounce/utils.h"

ZBActivityDataResponse new_zb_activity_data_response() {
    ZBActivityDataResponse response;

    response.found = false;
    response.active_in_days = -1;

    return response;
}

char* zb_activity_data_response_to_string(ZBActivityDataResponse* response) {
    const char* serialization = "ZBActivityDataResponse{found=%d, active_in_days='%d'}";
    int size = snprintf(NULL, 0, serialization, response->found, response->active_in_days);

    if (size < 0) {
        return NULL;
    }

    char* buffer = (char*) malloc(size + 1);
    if (!buffer) {
        return NULL;
    }

    snprintf(buffer, size + 1, serialization, response->found, response->active_in_days);
    return buffer;
}

ZBActivityDataResponse zb_activity_data_response_from_json(const json_object* j) {
    ZBActivityDataResponse r = new_zb_activity_data_response();

    r.found = *(bool*)get_json_value(j, json_type_boolean, "found", (void*)false);
    r.active_in_days = atoi((char*)get_json_value(j, json_type_string, "active_in_days", (void*)"-1"));

    return r;
}
