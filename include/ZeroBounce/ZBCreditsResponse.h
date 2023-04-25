#ifndef ZBCREDITSRESPONSE_H
#define ZBCREDITSRESPONSE_H

#include <json-c/json.h>

typedef struct {
    int credits;
} ZBCreditsResponse;

ZBCreditsResponse new_zb_credits_response();

char* zb_credits_response_to_string(ZBCreditsResponse* response);

ZBCreditsResponse zb_credits_response_from_json(const json_object* j);

int compare_zb_credits_response(const ZBCreditsResponse* response1, const ZBCreditsResponse* response2);

#endif