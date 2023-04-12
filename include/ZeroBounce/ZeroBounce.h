#ifndef ZEROBOUNCE_H
#define ZEROBOUNCE_H

#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "ZeroBounce/ZBErrorResponse.h"
// #include "ZeroBounce/ZBCreditsResponse.h"
// #include "ZeroBounce/ZBGetApiUsageResponse.h"
// #include "ZeroBounce/ZBValidateResponse.h"
// #include "ZeroBounce/ZBValidateBatchResponse.h"
// #include "ZeroBounce/ZBSendFileResponse.h"
// #include "ZeroBounce/ZBFileStatusResponse.h"
// #include "ZeroBounce/ZBGetFileResponse.h"
// #include "ZeroBounce/ZBDeleteFileResponse.h"
#include "ZeroBounce/ZBActivityDataResponse.h"

typedef void (*OnSuccessCallback)(ZBActivityDataResponse response);
typedef void (*OnErrorCallback)(ZBErrorResponse error_response);

typedef struct {
    char* return_url;
    int first_name_column;
    int last_name_column;
    int gender_column;
    int ip_address_column;
    bool has_header_row;
    bool remove_duplicate;
} SendFileOptions;

typedef struct {
    char* api_key;
    const char* api_base_url;
    const char* bulk_api_base_url;
    const char* bulk_api_scoring_base_url;
} ZeroBounce;

static ZeroBounce* zero_bounce_instance = NULL;

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

ZeroBounce* new_zero_bounce_instance();

ZeroBounce* zero_bounce_get_instance();

void zero_bounce_initialize(ZeroBounce* zb, const char* apiKey);

bool zero_bounce_invalid_api_key(ZeroBounce *zb, OnErrorCallback error_callback);

void get_activity_data(
    ZeroBounce *zb,
    char* email,
    OnSuccessCallback success_callback,
    OnErrorCallback error_callback
);


// class ZeroBounce {
//     private:

//         bool invalidApiKey(OnErrorCallback errorCallback);

//         template <typename T>
//         void sendRequest(
//             std::string urlPath,
//             OnSuccessCallback<T> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void sendFileInternal(
//             bool scoring,
//             std::string filePath,
//             int emailAddressColumnIndex,
//             SendFileOptions options,
//             OnSuccessCallback<ZBSendFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void fileStatusInternal(
//             bool scoring,
//             std::string fileId,
//             OnSuccessCallback<ZBFileStatusResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void getFileInternal(
//             bool scoring,
//             std::string fileId,
//             std::string localDownloadPath,
//             OnSuccessCallback<ZBGetFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void deleteFileInternal(
//             bool scoring,
//             std::string fileId,
//             OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );
    
//     public:
//         void getCredits(
//             OnSuccessCallback<ZBCreditsResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

        // void getApiUsage(
        //     std::tm startDate,
        //     std::tm endDate,
        //     OnSuccessCallback<ZBGetApiUsageResponse> successCallback,
        //     OnErrorCallback errorCallback
        // );

//         void validate(
//             std::string email,
//             std::string ipAddress,
//             OnSuccessCallback<ZBValidateResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void validateBatch(
//             std::vector<ZBEmailToValidate> emailBatch,
//             OnSuccessCallback<ZBValidateBatchResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void sendFile(
//             std::string filePath,
//             int emailAddressColumnIndex,
//             SendFileOptions options,
//             OnSuccessCallback<ZBSendFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void fileStatus(
//             std::string fileId,
//             OnSuccessCallback<ZBFileStatusResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void getFile(
//             std::string fileId,
//             std::string localDownloadPath,
//             OnSuccessCallback<ZBGetFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void deleteFile(
//             std::string fileId,
//             OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void scoringSendFile(
//             std::string filePath,
//             int emailAddressColumnIndex,
//             SendFileOptions options,
//             OnSuccessCallback<ZBSendFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void scoringFileStatus(
//             std::string fileId,
//             OnSuccessCallback<ZBFileStatusResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void scoringGetFile(
//             std::string fileId,
//             std::string localDownloadPath,
//             OnSuccessCallback<ZBGetFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void scoringDeleteFile(
//             std::string fileId,
//             OnSuccessCallback<ZBDeleteFileResponse> successCallback,
//             OnErrorCallback errorCallback
//         );

//         void getActivityData(
//             std::string email,
//             OnSuccessCallback<ZBActivityDataResponse> successCallback,
//             OnErrorCallback errorCallback
//         );
// };

#endif