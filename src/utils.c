#include <stdio.h>
#include <string.h>

#include "ZeroBounce/utils.h"

StringVector string_vector_init() {
    StringVector vector;
    vector.size = 0;
    vector.data = NULL;
    return vector;
}

void string_vector_append(StringVector* vector, const char* str) {
    if (!vector) return;
    
    vector->data = realloc(vector->data, (vector->size + 1) * sizeof(char*));
    if (!vector->data) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    
    vector->data[vector->size] = strdup(str);
    vector->size++;
}

void string_vector_free(StringVector* vector) {
    if (!vector) return;
    
    for (size_t i = 0; i < vector->size; ++i) {
        free(vector->data[i]);
    }
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
}

char* concatenate_strings(const StringVector* vec) {
    size_t total_length = 1;

    for (size_t i = 0; i < vec->size; ++i) {
        total_length += strlen(vec->data[i]) + 2;
        if (i < vec->size - 1) {
            total_length += 2;
        }
    }

    char* result = (char*) malloc(total_length);
    if (!result) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';

    for (size_t i = 0; i < vec->size; ++i) {
        strcat(result, "'");
        strcat(result, vec->data[i]);
        strcat(result, "'");
        if (i < vec->size - 1) {
            strcat(result, ", ");
        }
    }

    return result;
}

void* get_json_value(const json_object* obj, json_type type, const char* key, void* default_value) {
    json_object* value_obj = NULL;
    int* int_value;
    if (json_object_object_get_ex(obj, key, &value_obj)) {
        if (json_object_is_type(value_obj, type)) {
            switch (type) {
                case json_type_boolean:
                    return (void*)json_object_get_boolean(value_obj);
                case json_type_int:
                    int_value = (int*) malloc(sizeof(int));
                    if (!int_value) {
                        fprintf(stderr, "Memory allocation failed.\n");
                        exit(EXIT_FAILURE);
                    }
                    *int_value = json_object_get_int(value_obj);
                    return (void*)int_value;
                case json_type_string:
                    return (void*)json_object_get_string(value_obj);
                default:
                    return NULL;
            }
        }
    }
    return default_value;
}