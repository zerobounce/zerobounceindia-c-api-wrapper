#include <stdio.h>

#include "ZeroBounce/ZBCreditsResponse.h"
#include "ZeroBounce/utils.h"

ZBCreditsResponse new_zb_credits_response() {
    ZBCreditsResponse response;

    response.credits = -1;

    return response;
}

char* zb_credits_response_to_string(ZBCreditsResponse* response) {
    const char *serialization = "ZBCreditsResponse{"
        "credits='%d'"
        "}";

    int size = snprintf(
        NULL, 0, serialization,
        response->credits
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
        response->credits
    );
    return buffer;
}

ZBCreditsResponse zb_credits_response_from_json(const json_object* j) {
    ZBCreditsResponse r = new_zb_credits_response();

    r.credits = atoi((char*)get_json_value(j, json_type_string, "Credits", (void*)"-1"));

    return r;
}
