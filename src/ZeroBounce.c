#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "ZeroBounce/ZeroBounce.h"

typedef struct {
  char *response;
  size_t size;
} memory;
 
static size_t write_callback(void *data, size_t size, size_t nmemb, void *clientp) {
  size_t real_size = size * nmemb;
  memory *mem = (memory*)clientp;
 
  char *ptr = realloc(mem->response, mem->size + real_size + 1);
  if(ptr == NULL)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, real_size);
  mem->size += real_size;
  mem->response[mem->size] = 0;
 
  return real_size;
}


SendFileOptions new_send_file_options() {
    SendFileOptions options;

    options.return_url = "";
    options.first_name_column = 0;
    options.last_name_column = 0;
    options.gender_column = 0;
    options.ip_address_column = 0;
    options.has_header_row = true;
    options.remove_duplicate = true;

    return options;
}


ZeroBounce* new_zero_bounce_instance() {
    ZeroBounce* zb = (ZeroBounce*) malloc(sizeof(ZeroBounce));
    zb->api_key = NULL;
    zb->api_base_url = "https://api.zerobounce.net/v2";
    zb->bulk_api_base_url = "https://bulkapi.zerobounce.net/v2";
    zb->bulk_api_scoring_base_url = "https://bulkapi.zerobounce.net/v2/scoring";
    return zb;
}


ZeroBounce* zero_bounce_get_instance() {
    if (zero_bounce_instance == NULL) {
        zero_bounce_instance = new_zero_bounce_instance();
    }
    return zero_bounce_instance;
}


void zero_bounce_initialize(ZeroBounce* zb, const char* api_key) {
    if (zb->api_key) {
        free(zb->api_key);
    }
    zb->api_key = strdup(api_key);
}


bool zero_bounce_invalid_api_key(ZeroBounce *zb, OnErrorCallback error_callback) {
    if (zb->api_key == NULL || strlen(zb->api_key) == 0) {
        ZBErrorResponse error_response = parse_error(
            "ZeroBounce is not initialized. Please call zero_bounce_initialize(zb_instance, api_key); first"
        );
        error_callback(error_response);
        return true;
    }
    return false;
}


static void send_file_internal(
    ZeroBounce *zb,
    bool scoring,
    char* file_path,
    int email_address_column_index,
    SendFileOptions options,
    OnSuccessCallbackSendFile success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    const char *url_pattern = "%s/sendFile";
    const char* base_url = scoring ? zb->bulk_api_scoring_base_url : zb->bulk_api_base_url;

    int url_path_len = snprintf(
        NULL, 0, url_pattern, base_url
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, base_url
    );

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_mime *multipart = curl_mime_init(curl);

    curl_mimepart *part = curl_mime_addpart(multipart);
    curl_mime_name(part, "api_key");
    curl_mime_data(part, zb->api_key, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, file_path);

    char int_str_option[10];

    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "email_address_column");
    sprintf(int_str_option, "%d", email_address_column_index);
    curl_mime_data(part, int_str_option, CURL_ZERO_TERMINATED);

    if (!scoring) {
        if (options.first_name_column > 0) {
            part = curl_mime_addpart(multipart);
            curl_mime_name(part, "first_name_column");
            sprintf(int_str_option, "%d", options.first_name_column);
            curl_mime_data(part, int_str_option, CURL_ZERO_TERMINATED);
        }
        if (options.last_name_column > 0) {
            part = curl_mime_addpart(multipart);
            curl_mime_name(part, "last_name_column");
            sprintf(int_str_option, "%d", options.last_name_column);
            curl_mime_data(part, int_str_option, CURL_ZERO_TERMINATED);
        }
        if (options.gender_column > 0) {
            part = curl_mime_addpart(multipart);
            curl_mime_name(part, "gender_column");
            sprintf(int_str_option, "%d", options.gender_column);
            curl_mime_data(part, int_str_option, CURL_ZERO_TERMINATED);
        }
        if (options.ip_address_column > 0) {
            part = curl_mime_addpart(multipart);
            curl_mime_name(part, "ip_address_column");
            sprintf(int_str_option, "%d", options.ip_address_column);
            curl_mime_data(part, int_str_option, CURL_ZERO_TERMINATED);
        }
    }

    if (!strlen(options.return_url) == 0) {
        part = curl_mime_addpart(multipart);
        curl_mime_name(part, "return_url");
        curl_mime_data(part, options.return_url, CURL_ZERO_TERMINATED);
    }

    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "has_header_row");
    curl_mime_data(part, options.has_header_row ? "true" : "false", CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(multipart);
    curl_mime_name(part, "remove_duplicate");
    curl_mime_data(part, options.has_header_row ? "true" : "false", CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, multipart);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_send_file_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    curl_mime_free(multipart);
}


void get_credits(
    ZeroBounce *zb,
    OnSuccessCallbackCredits success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    const char *url_pattern = "%s/getcredits?api_key=%s";

    int url_path_len = snprintf(
        NULL, 0, url_pattern, zb->api_base_url, zb->api_key
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, zb->api_base_url, zb->api_key
    );

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_credits_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}


void get_api_usage(
    ZeroBounce *zb,
    struct tm start_date,
    struct tm end_date,
    OnSuccessCallbackApiUsage success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    char* date_format = "%Y-%m-%d";
    char start_date_str[11];
    char end_date_str[11];

    strftime(start_date_str, sizeof(start_date_str), date_format, &start_date);
    strftime(end_date_str, sizeof(end_date_str), date_format, &end_date);

    const char *url_pattern = "%s/getapiusage?api_key=%s&start_date=%s&end_date=%s";

    int url_path_len = snprintf(
        NULL, 0, url_pattern, zb->api_base_url, zb->api_key,
        start_date_str, end_date_str
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, zb->api_base_url, zb->api_key,
        start_date_str, end_date_str
    );

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_get_api_usage_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}


void validate_email(
    ZeroBounce *zb,
    char* email,
    char* ip_address,
    OnSuccessCallbackValidate success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    const char *url_pattern = "%s/validate?api_key=%s&email=%s&ip_address=%s";

    int url_path_len = snprintf(
        NULL, 0, url_pattern, zb->api_base_url, zb->api_key, email, ip_address
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, zb->api_base_url, zb->api_key, email, ip_address
    );

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_validate_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}


void validate_batch(
    ZeroBounce *zb,
    EmailToValidateVector email_batch,
    OnSuccessCallbackValidateBatch success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    const char *url_pattern = "%s/validatebatch";

    int url_path_len = snprintf(
        NULL, 0, url_pattern, zb->bulk_api_base_url
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, zb->bulk_api_base_url
    );

    json_object *payload = json_object_new_object();
    json_object *email_batch_json = json_object_new_array();

    for(size_t i = 0; i < email_batch.size; i++) {
        json_object *email_obj = json_object_new_object();
        json_object_object_add(email_obj, "email_address", json_object_new_string(email_batch.data[i].email_address));
        json_object_object_add(email_obj, "ip_address", json_object_new_string(email_batch.data[i].ip_address));
        json_object_array_add(email_batch_json, email_obj);
    }

    json_object_object_add(payload, "api_key", json_object_new_string(zb->api_key));
    json_object_object_add(payload, "email_batch", email_batch_json);

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, json_object_to_json_string(payload));

    json_object_put(payload);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_validate_batch_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}


void send_file(
    ZeroBounce *zb,
    char* file_path,
    int email_address_column_index,
    SendFileOptions options,
    OnSuccessCallbackSendFile success_callback,
    OnErrorCallback error_callback
) {
    send_file_internal(zb, false, file_path, email_address_column_index, options, success_callback, error_callback);
}


void get_activity_data(
    ZeroBounce *zb,
    char* email,
    OnSuccessCallbackActivityData success_callback,
    OnErrorCallback error_callback
) {
    if (zero_bounce_invalid_api_key(zb, error_callback)) return;

    const char *url_pattern = "%s/activity?api_key=%s&email=%s";

    int url_path_len = snprintf(
        NULL, 0, url_pattern, zb->api_base_url, zb->api_key, email
    );

    char *url_path = malloc(url_path_len + 1);
    if (!url_path) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(
        url_path, url_path_len + 1, url_pattern, zb->api_base_url, zb->api_key, email
    );

    CURL* curl = curl_easy_init();
    if (!curl) {
        error_callback(parse_error("Failed to initialize libcurl"));
        return;
    }

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(curl, CURLOPT_URL, url_path);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    memory response_data = {0};

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        error_callback(parse_error("Failed to perform request"));
        return;
    }

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    if (http_code > 299) {
        if (error_callback) {
            error_callback(parse_error(response_data.response));
        }
    } else {
        if (success_callback) {
            json_object *j_obj = json_tokener_parse(response_data.response);
            if (j_obj == NULL) {
                error_callback(parse_error("Failed to parse json string"));
                return;
            }

            success_callback(zb_activity_data_response_from_json(j_obj));
            json_object_put(j_obj);
        }
    }

    free(url_path);
    free(response_data.response);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}

// void ZeroBounce::sendFile(
//     std::string filePath,
//     int emailAddressColumnIndex,
//     SendFileOptions options,
//     OnSuccessCallback<ZBSendFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     sendFileInternal(false, filePath, emailAddressColumnIndex, options, successCallback, errorCallback);
// }

// void ZeroBounce::fileStatus(
//     std::string fileId,
//     OnSuccessCallback<ZBFileStatusResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     fileStatusInternal(false, fileId, successCallback, errorCallback);
// }

// void ZeroBounce::getFile(
//     std::string fileId,
//     std::string localDownloadPath,
//     OnSuccessCallback<ZBGetFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     getFileInternal(false, fileId, localDownloadPath, successCallback, errorCallback);
// }

// void ZeroBounce::deleteFile(
//     std::string fileId,
//     OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     deleteFileInternal(false, fileId, successCallback, errorCallback);
// }

// void ZeroBounce::scoringSendFile(
//     std::string filePath,
//     int emailAddressColumnIndex,
//     SendFileOptions options,
//     OnSuccessCallback<ZBSendFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     sendFileInternal(true, filePath, emailAddressColumnIndex, options, successCallback, errorCallback);
// }

// void ZeroBounce::scoringFileStatus(
//     std::string fileId,
//     OnSuccessCallback<ZBFileStatusResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     fileStatusInternal(true, fileId, successCallback, errorCallback);
// }

// void ZeroBounce::scoringGetFile(
//     std::string fileId,
//     std::string localDownloadPath,
//     OnSuccessCallback<ZBGetFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     getFileInternal(true, fileId, localDownloadPath, successCallback, errorCallback);
// }

// void ZeroBounce::scoringDeleteFile(
//     std::string fileId,
//     OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     deleteFileInternal(true, fileId, successCallback, errorCallback);
// }

// void ZeroBounce::getActivityData(
//     std::string email,
//     OnSuccessCallback<ZBActivityDataResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     sendRequest(
//         apiBaseUrl + "/activity?api_key=" + apiKey + "&email=" + email,
//         successCallback,
//         errorCallback
//     );
// }

// template <typename T>
// void ZeroBounce::sendRequest(
//     std::string urlPath,
//     OnSuccessCallback<T> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     try {
//         cpr::Response reqResponse = requestHandler.Get(
//             cpr::Url{urlPath},
//             cpr::Header{{"Accept", "application/json"}}
//         );
        
//         std::string rsp = reqResponse.text;

//         if (reqResponse.status_code > 299) {
//             if (errorCallback) {
//                 ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
//                 errorCallback(errorResponse);
//             }
//         } else {
//             if (successCallback) {
//                 T response = T::from_json(json::parse(rsp));
//                 successCallback(response);
//             }
//         }
//     } catch (std::exception e) {
//         ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
//         errorCallback(errorResponse);
//     }
// }

// void ZeroBounce::sendFileInternal(
//     bool scoring,
//     std::string filePath,
//     int emailAddressColumnIndex,
//     SendFileOptions options,
//     OnSuccessCallback<ZBSendFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     try {
//         std::string urlPath = (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/sendFile";

//         cpr::Multipart multipart{
//             {"api_key", apiKey},
//             {"file", cpr::File{filePath}},
//             {"email_address_column", emailAddressColumnIndex}
//         };

//         if (!scoring) {
//             if (options.firstNameColumn > 0) {
//                 multipart.parts.emplace_back(cpr::Part{"first_name_column", options.firstNameColumn});
//             }
//             if (options.lastNameColumn > 0) {
//                 multipart.parts.emplace_back(cpr::Part{"last_name_column", options.lastNameColumn});
//             }
//             if (options.genderColumn > 0) {
//                 multipart.parts.emplace_back(cpr::Part{"gender_column", options.genderColumn});
//             }
//             if (options.ipAddressColumn > 0) {
//                 multipart.parts.emplace_back(cpr::Part{"ip_address_column", options.ipAddressColumn});
//             }
//         }

//         if (!options.returnUrl.empty()) {
//                 multipart.parts.emplace_back(cpr::Part{"return_url", options.returnUrl});
//         }
        
//         multipart.parts.emplace_back(cpr::Part{"has_header_row", options.hasHeaderRow});
//         multipart.parts.emplace_back(cpr::Part{"remove_duplicate", options.removeDuplicate});

//         cpr::Response reqResponse = requestHandler.Post(
//             cpr::Url{urlPath},
//             cpr::Header{{"Content-Type", "multipart/form-data"}},
//             multipart
//         );

//         std::string rsp = reqResponse.text;

//         if (reqResponse.status_code > 299) {
//             if (errorCallback) {
//                 ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
//                 errorCallback(errorResponse);
//             }
//         } else {
//             if (successCallback) {
//                 ZBSendFileResponse response = ZBSendFileResponse::from_json(json::parse(rsp));
//                 successCallback(response);
//             }
//         }
//     } catch (std::exception e) {
//         ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
//         errorCallback(errorResponse);
//     }
// }

// void ZeroBounce::fileStatusInternal(
//     bool scoring,
//     std::string fileId,
//     OnSuccessCallback<ZBFileStatusResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     sendRequest(
//         (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/filestatus?api_key=" + apiKey
//             + "&file_id=" + fileId,
//         successCallback,
//         errorCallback
//     );
// }

// void ZeroBounce::getFileInternal(
//     bool scoring,
//     std::string fileId,
//     std::string localDownloadPath,
//     OnSuccessCallback<ZBGetFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     try {
//         std::string urlPath = (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl)
//             + "/getFile?api_key=" + apiKey + "&file_id=" + fileId;
        
//         cpr::Response reqResponse = requestHandler.Get(cpr::Url{urlPath});

//         std::string contentType = reqResponse.header["Content-Type"];

//         std::string rsp = reqResponse.text;

//         if (reqResponse.status_code > 299) {
//             if (errorCallback) {
//                 ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
//                 errorCallback(errorResponse);
//             }
//         } else {
//             if (successCallback) {
//                 if (contentType != "application/json") {
//                     fs::path filePath(localDownloadPath);

//                     if (fs::is_directory(filePath)) {
//                         ZBErrorResponse errorResponse = ZBErrorResponse::parseError("Invalid file path");
//                         errorCallback(errorResponse);
//                         return;
//                     }

//                     fs::create_directories(filePath.parent_path());

//                     std::ofstream fileStream(filePath, std::ofstream::out | std::ofstream::binary);

//                     fileStream.write(rsp.c_str(), rsp.size());
//                     fileStream.close();

//                     ZBGetFileResponse response;
//                     response.success = true;
//                     response.localFilePath = localDownloadPath;
//                     successCallback(response);
//                 } else {
//                     ZBGetFileResponse response = ZBGetFileResponse::from_json(json::parse(rsp));
//                     successCallback(response);
//                 }
//             }
//         }
//     } catch (std::exception e) {
//         ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
//         errorCallback(errorResponse);
//     }
// }

// void ZeroBounce::deleteFileInternal(
//     bool scoring,
//     std::string fileId,
//     OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     sendRequest(
//         (scoring ? bulkApiScoringBaseUrl : bulkApiBaseUrl) + "/deletefile?api_key=" + apiKey
//             + "&file_id=" + fileId,
//         successCallback,
//         errorCallback
//     );
// }
