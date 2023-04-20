#ifndef ZBVALIDATEBATCHRESPONSE_H
#define ZBVALIDATEBATCHRESPONSE_H

#include "ZBValidateResponse.h"

typedef struct {
    char* email_address;
    char* ip_address;
} ZBEmailToValidate;

ZBEmailToValidate new_zb_email_to_validate();


typedef struct {
    ZBEmailToValidate* data;
    size_t size;
} EmailToValidateVector;

EmailToValidateVector email_to_validate_vector_init();

void email_to_validate_vector_append(EmailToValidateVector* vector, const ZBEmailToValidate email);

void email_to_validate_vector_free(EmailToValidateVector* vector);


typedef struct {
    char* error;
    char* email_address;
} ZBValidateError;

ZBValidateError new_zb_validate_error();

char* zb_validate_error_to_string(ZBValidateError* response);

ZBValidateError zb_validate_error_from_json(const json_object* j);


typedef struct {
    ZBValidateError* data;
    size_t size;
} ValidateErrorVector;

ValidateErrorVector validate_error_vector_init();

void validate_error_vector_append(ValidateErrorVector* vector, const ZBValidateError error);

void validate_error_vector_free(ValidateErrorVector* vector);


typedef struct {
    ZBValidateResponse* data;
    size_t size;
} ValidateResponseVector;

ValidateResponseVector validate_response_vector_init();

void validate_response_vector_append(ValidateResponseVector* vector, const ZBValidateResponse response);

void validate_response_vector_free(ValidateResponseVector* vector);


typedef struct {
    ValidateResponseVector email_batch;
    ValidateErrorVector errors;
} ZBValidateBatchResponse;

ZBValidateBatchResponse new_zb_validate_batch_response();

char* zb_validate_batch_response_to_string(ZBValidateBatchResponse* response);

ZBValidateBatchResponse zb_validate_batch_response_from_json(const json_object* j);

#endif