#ifndef ZBDELETEFILERESPONSE_H
#define ZBDELETEFILERESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

typedef struct { 
    bool success ;
    char* message;
    char* file_name;
    char* file_id;
} ZBDeleteFileResponse;

ZBDeleteFileResponse new_zb_delete_file_response();

char* zb_delete_file_response_to_string(ZBDeleteFileResponse* response);

ZBDeleteFileResponse zb_delete_file_response_from_json(const json_object* j);

int compare_zb_delete_file_response(const ZBDeleteFileResponse* response1, const ZBDeleteFileResponse* response2);

#endif