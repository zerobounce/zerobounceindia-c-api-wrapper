#ifndef ZBFILESTATUSRESPONSE_H
#define ZBFILESTATUSRESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

typedef struct {
    bool success;
    char* message;
    char* error_reason;
    char* file_id;
    char* file_name;
    char* upload_date;
    char* file_status;
    char* complete_percentage;
    char* return_url;
} ZBFileStatusResponse;

ZBFileStatusResponse new_zb_file_status_response();

char* zb_file_status_response_to_string(ZBFileStatusResponse* response);

ZBFileStatusResponse zb_file_status_response_from_json(const json_object* j);

int compare_zb_file_status_response(const ZBFileStatusResponse* response1, const ZBFileStatusResponse* response2);

#endif