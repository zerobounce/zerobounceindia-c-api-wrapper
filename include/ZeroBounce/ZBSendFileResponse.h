#ifndef ZBSENDFILERESPONSE_H
#define ZBSENDFILERESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

#include "utils.h"

typedef struct {
    bool success;
    StringVector message;
    char* file_name;
    char* file_id;
} ZBSendFileResponse;

ZBSendFileResponse new_zb_send_file_response();

char* zb_send_file_response_to_string(ZBSendFileResponse* response);

ZBSendFileResponse zb_send_file_response_from_json(const json_object* j);

#endif