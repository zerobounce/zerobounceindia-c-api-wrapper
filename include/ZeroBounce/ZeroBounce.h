#ifndef ZEROBOUNCE_H
#define ZEROBOUNCE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <curl/curl.h>

#include "ZeroBounce/ZBErrorResponse.h"
#include "ZeroBounce/ZBCreditsResponse.h"
#include "ZeroBounce/ZBGetApiUsageResponse.h"
#include "ZeroBounce/ZBValidateResponse.h"
#include "ZeroBounce/ZBValidateBatchResponse.h"
#include "ZeroBounce/ZBSendFileResponse.h"
#include "ZeroBounce/ZBFileStatusResponse.h"
#include "ZeroBounce/ZBGetFileResponse.h"
#include "ZeroBounce/ZBDeleteFileResponse.h"
#include "ZeroBounce/ZBActivityDataResponse.h"

typedef void (*OnErrorCallback)(ZBErrorResponse error_response);
typedef void (*OnSuccessCallbackCredits)(ZBCreditsResponse response);
typedef void (*OnSuccessCallbackApiUsage)(ZBGetApiUsageResponse response);
typedef void (*OnSuccessCallbackValidate)(ZBValidateResponse response);
typedef void (*OnSuccessCallbackValidateBatch)(ZBValidateBatchResponse response);
typedef void (*OnSuccessCallbackSendFile)(ZBSendFileResponse response);
typedef void (*OnSuccessCallbackFileStatus)(ZBFileStatusResponse response);
typedef void (*OnSuccessCallbackGetFile)(ZBGetFileResponse response);
typedef void (*OnSuccessCallbackDeleteFile)(ZBDeleteFileResponse response);
typedef void (*OnSuccessCallbackActivityData)(ZBActivityDataResponse response);


typedef struct {
    char* return_url;
    int first_name_column;
    int last_name_column;
    int gender_column;
    int ip_address_column;
    bool has_header_row;
    bool remove_duplicate;
} SendFileOptions;

SendFileOptions new_send_file_options();

typedef struct {
    char* api_key;
    const char* api_base_url;
    const char* bulk_api_base_url;
    const char* bulk_api_scoring_base_url;
} ZeroBounce;

static ZeroBounce* zero_bounce_instance = NULL;

typedef struct {
  char *response;
  size_t size;
} memory;

static size_t write_callback(void *data, size_t size, size_t nmemb, void *clientp);

void set_write_callback(CURL* curl, memory* response_data);

long get_http_code(CURL* curl);

char* get_content_type_value(CURL* curl);

ZeroBounce* new_zero_bounce_instance();

ZeroBounce* zero_bounce_get_instance();

void zero_bounce_initialize(ZeroBounce* zb, const char* apiKey);

static bool zero_bounce_invalid_api_key(ZeroBounce *zb, OnErrorCallback error_callback);

static void send_file_internal(
    ZeroBounce *zb,
    bool scoring,
    char* file_path,
    int email_address_column_index,
    SendFileOptions options,
    OnSuccessCallbackSendFile success_callback,
    OnErrorCallback error_callback
);

static void file_status_internal(
    ZeroBounce *zb,
    bool scoring,
    char* file_id,
    OnSuccessCallbackFileStatus success_callback,
    OnErrorCallback error_callback
);

static void get_file_internal(
    ZeroBounce *zb,
    bool scoring,
    char* file_id,
    char* local_download_path,
    OnSuccessCallbackGetFile success_callback,
    OnErrorCallback error_callback
);

static void delete_file_internal(
    ZeroBounce *zb,
    bool scoring,
    char* file_id,
    OnSuccessCallbackDeleteFile success_callback,
    OnErrorCallback error_callback
);

void get_credits(
    ZeroBounce *zb,
    OnSuccessCallbackCredits success_callback,
    OnErrorCallback error_callback
);

void get_api_usage(
    ZeroBounce *zb,
    struct tm start_date,
    struct tm end_date,
    OnSuccessCallbackApiUsage success_callback,
    OnErrorCallback error_callback
);

void validate_email(
    ZeroBounce *zb,
    char* email,
    char* ip_address,
    OnSuccessCallbackValidate success_callback,
    OnErrorCallback error_callback
);

void validate_batch(
    ZeroBounce *zb,
    EmailToValidateVector email_batch,
    OnSuccessCallbackValidateBatch success_callback,
    OnErrorCallback error_callback
);

void send_file(
    ZeroBounce *zb,
    char* file_path,
    int email_address_column_index,
    SendFileOptions options,
    OnSuccessCallbackSendFile success_callback,
    OnErrorCallback error_callback
);

void file_status(
    ZeroBounce *zb,
    char* file_id,
    OnSuccessCallbackFileStatus success_callback,
    OnErrorCallback error_callback
);

void get_file(
    ZeroBounce *zb,
    char* file_id,
    char* local_download_path,
    OnSuccessCallbackGetFile success_callback,
    OnErrorCallback error_callback
);

void delete_file(
    ZeroBounce *zb,
    char* file_id,
    OnSuccessCallbackDeleteFile success_callback,
    OnErrorCallback error_callback
);

void scoring_send_file(
    ZeroBounce *zb,
    char* file_path,
    int email_address_column_index,
    SendFileOptions options,
    OnSuccessCallbackSendFile success_callback,
    OnErrorCallback error_callback
);

void scoring_file_status(
    ZeroBounce *zb,
    char* file_id,
    OnSuccessCallbackFileStatus success_callback,
    OnErrorCallback error_callback
);

void scoring_get_file(
    ZeroBounce *zb,
    char* file_id,
    char* local_download_path,
    OnSuccessCallbackGetFile success_callback,
    OnErrorCallback error_callback
);

void scoring_delete_file(
    ZeroBounce *zb,
    char* file_id,
    OnSuccessCallbackDeleteFile success_callback,
    OnErrorCallback error_callback
);

void get_activity_data(
    ZeroBounce *zb,
    char* email,
    OnSuccessCallbackActivityData success_callback,
    OnErrorCallback error_callback
);

#endif