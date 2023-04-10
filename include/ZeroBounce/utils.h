#ifndef UTILS_H
#define UTILS_H

#include <json-c/json.h>

void* get_json_value(json_object* obj, json_type type, const char* key, void* default_value) {
    json_object* value_obj = NULL;
    if (json_object_object_get_ex(obj, key, &value_obj)) {
        if (json_object_is_type(value_obj, type)) {
            switch (type) {
                case json_type_int:
                    return (void*)json_object_get_int(value_obj);
                case json_type_string:
                    return (void*)json_object_get_string(value_obj);
                default:
                    return NULL;
            }
        }
    }
    return default_value;
}

#endif