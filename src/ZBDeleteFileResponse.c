#include <stdio.h>

#include "ZeroBounce/ZBDeleteFileResponse.h"
#include "ZeroBounce/utils.h"

ZBDeleteFileResponse new_zb_delete_file_response() {
    ZBDeleteFileResponse response;

    response.success = false;
    response.message = "";
    response.file_name = "";
    response.file_id = "";

    return response;
}

char* zb_delete_file_response_to_string(ZBDeleteFileResponse* response) {
    const char *serialization = "ZBDeleteFileResponse{"
        "success=%d"
        ", message='%s'"
        ", file_name='%s'"
        ", file_id='%s'"
        "}";

    int size = snprintf(
        NULL, 0, serialization,
        response->success,
        response->message,
        response->file_name,
        response->file_id
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
        response->file_name,
        response->file_id
    );
    return buffer;
}

ZBDeleteFileResponse zb_delete_file_response_from_json(const json_object* j) {
    ZBDeleteFileResponse r = new_zb_delete_file_response();

    r.success = *(bool*)get_json_value(j, json_type_boolean, "success", &(bool){false});
    r.message = (char*)get_json_value(j, json_type_string, "message", (void*)"");
    r.file_name = (char*)get_json_value(j, json_type_string, "file_name", (void*)"");
    r.file_id = (char*)get_json_value(j, json_type_string, "file_id", (void*)"");

    return r;
}
