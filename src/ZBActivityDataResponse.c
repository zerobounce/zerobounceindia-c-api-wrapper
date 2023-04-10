#include "ZeroBounce/ZBActivityDataResponse.h"

struct ZBActivityDataResponse new_zb_activity_data_response() {
    struct ZBActivityDataResponse response;

    response.found = 0;
    response.activeInDays = -1;

    return response;
}

char* zb_activity_data_response_to_string(struct ZBActivityDataResponse response) {
    const char* serialization = "ZBActivityDataResponse{found=%d, activeInDays='%s'}";
    int size = snprintf(NULL, 0, serialization, response.found, response.activeInDays);

    if (size < 0) {
        return NULL;
    }

    char* buffer = (char*) malloc(size + 1);
    if (!buffer) {
        return NULL;
    }

    snprintf(buffer, size + 1, serialization, response.found, response.activeInDays);
    return buffer;
}

struct ZBActivityDataResponse zb_activity_data_response_from_json(const json_object* j) {
    struct ZBActivityDataResponse r;

    r.found = *(int*)get_json_value(j, "found", (void*)0, json_type_int);
    r.activeInDays = atoi((char*)get_json_value(j, "active_in_days", (void*)"-1", json_type_string));

    return r;
}
