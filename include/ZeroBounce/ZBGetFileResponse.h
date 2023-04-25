#ifndef ZBGETFILERESPONSE_H
#define ZBGETFILERESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

typedef struct {
    bool success;
    char* message;
    char* local_file_path;
} ZBGetFileResponse;

ZBGetFileResponse new_zb_get_file_response();

char* zb_get_file_response_to_string(ZBGetFileResponse* response);

ZBGetFileResponse zb_get_file_response_from_json(const json_object* j);

int compare_zb_get_file_response(const ZBGetFileResponse* response1, const ZBGetFileResponse* response2);

#endif