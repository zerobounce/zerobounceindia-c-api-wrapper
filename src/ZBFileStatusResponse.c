#include <stdio.h>

#include "ZeroBounce/ZBFileStatusResponse.h"
#include "ZeroBounce/utils.h"

ZBFileStatusResponse new_zb_file_status_response() {
    ZBFileStatusResponse response;

    response.success = false;
    response.message = "";
    response.error_reason = "";
    response.file_id = "";
    response.file_name = "";
    response.upload_date = "";
    response.file_status = "";
    response.complete_percentage = "";
    response.return_url = "";

    return response;
}

char* zb_file_status_response_to_string(ZBFileStatusResponse* response) {
    const char *serialization = "ZBFileStatusResponse{"
        "success=%d"
        ", message='%s'"
        ", file_id='%s'"
        ", file_name='%s'"
        ", upload_date='%s'"
        ", file_status='%s'"
        ", complete_percentage='%s'"
        ", return_url='%s'"
        ", error_reason='%s'"
        "}";

    int size = snprintf(
        NULL, 0, serialization,
        response->success,
        response->message,
        response->error_reason,
        response->file_id,
        response->file_name,
        response->upload_date,
        response->file_status,
        response->complete_percentage,
        response->return_url
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
        response->error_reason,
        response->file_id,
        response->file_name,
        response->upload_date,
        response->file_status,
        response->complete_percentage,
        response->return_url
    );
    return buffer;
}

ZBFileStatusResponse zb_file_status_response_from_json(const json_object* j) {
    ZBFileStatusResponse r = new_zb_file_status_response();

    r.success = *(bool*)get_json_value(j, json_type_boolean, "success", &(bool){false});
    r.message = (char*)get_json_value(j, json_type_string, "message", (void*)"");
    r.error_reason = (char*)get_json_value(j, json_type_string, "error_reason", (void*)"");
    r.file_id = (char*)get_json_value(j, json_type_string, "file_id", (void*)"");
    r.file_name = (char*)get_json_value(j, json_type_string, "file_name", (void*)"");
    r.upload_date = (char*)get_json_value(j, json_type_string, "upload_date", (void*)"");
    r.file_status = (char*)get_json_value(j, json_type_string, "file_status", (void*)"");
    r.complete_percentage = (char*)get_json_value(j, json_type_string, "complete_percentage", (void*)"");
    r.return_url = (char*)get_json_value(j, json_type_string, "return_url", (void*)"");

    return r;
}
