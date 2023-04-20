#include <stdio.h>

#include "ZeroBounce/ZBGetFileResponse.h"
#include "ZeroBounce/utils.h"

ZBGetFileResponse new_zb_get_file_response() {
    ZBGetFileResponse response;

    response.success = false;
    response.message = "";
    response.local_file_path = "";

    return response;
}

char* zb_get_file_response_to_string(ZBGetFileResponse* response) {
    const char *serialization = "ZBGetFileResponse{"
        "success=%d"
        ", message='%s'"
        ", local_file_path='%s'"
        "}";

    int size = snprintf(
        NULL, 0, serialization,
        response->success,
        response->message,
        response->local_file_path
    );

    if (size < 0) {
        return NULL;
    }

    char* buffer = (char*) malloc(size + 1);
    if (!buffer) {
        return NULL;
    }

    snprintf(
        buffer, size + 1, serialization,
        response->success,
        response->message,
        response->local_file_path
    );
    return buffer;
}

ZBGetFileResponse zb_get_file_response_from_json(const json_object* j) {
    ZBGetFileResponse r = new_zb_get_file_response();

    r.success = *(bool*)get_json_value(j, json_type_boolean, "success", &(bool){false});
    r.message = *(char**)get_json_value(j, json_type_string, "message", &(char*){""});

    return r;
}
