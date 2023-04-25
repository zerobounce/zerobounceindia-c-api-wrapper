#ifndef UTILS_H
#define UTILS_H

#include <json-c/json.h>

typedef struct {
    size_t size;
    char** data;
} StringVector;

StringVector string_vector_init();

void string_vector_append(StringVector* vector, const char* str);

void string_vector_free(StringVector* vector);

int compare_string_vector(const StringVector* vec1, const StringVector* vec2);

char* concatenate_strings(const StringVector* vec);

void* get_json_value(const json_object* obj, json_type type, const char* key, void* default_value);

#endif