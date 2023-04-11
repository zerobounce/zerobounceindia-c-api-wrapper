#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "ZeroBounce/ZeroBounce.h"

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

bool zero_bounce_invalid_api_key(ZeroBounce *zb, OnErrorCallback errorCallback) {
    if (zb->api_key == NULL || strlen(zb->api_key) == 0) {
        ZBErrorResponse errorResponse = parseError(
            "ZeroBounce is not initialized. Please call zero_bounce_initialize(zb_instance, api_key); first"
        );
        errorCallback(errorResponse);
        return true;
    }
    return false;
}

void get_credits(ZeroBounce *zb, OnErrorCallback errorCallback) {
    if (zero_bounce_invalid_api_key(zb, errorCallback)) return;

    printf("%s\n", zb->api_key);
}

// void ZeroBounce::getCredits(
//     OnSuccessCallback<ZBCreditsResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     sendRequest(
//         apiBaseUrl + "/getcredits?api_key=" + apiKey,
//         successCallback,
//         errorCallback
//     );

// }

// void ZeroBounce::getApiUsage(
//     std::tm startDate,
//     std::tm endDate,
//     OnSuccessCallback<ZBGetApiUsageResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     std::string dateFormat = "%Y-%m-%d";
//     std::ostringstream url;

//     url << apiBaseUrl << "/getapiusage?api_key=" << apiKey
//         << "&start_date=" << std::put_time(&startDate, dateFormat.c_str())
//         << "&end_date=" << std::put_time(&endDate, dateFormat.c_str());

//     sendRequest(
//         url.str(),
//         successCallback,
//         errorCallback
//     );
// }

// void ZeroBounce::validate(
//     std::string email,
//     std::string ipAddress,
//     OnSuccessCallback<ZBValidateResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     sendRequest(
//         apiBaseUrl + "/validate?api_key=" + apiKey +
//             "&email=" + email +
//             "&ip_address=" + (ipAddress.empty() ? "" : ipAddress),
//         successCallback,
//         errorCallback
//     );
// }

// void ZeroBounce::validateBatch(
//     std::vector<ZBEmailToValidate> emailBatch,
//     OnSuccessCallback<ZBValidateBatchResponse> successCallback,
//     OnErrorCallback errorCallback
// ) {
//     if (invalidApiKey(errorCallback)) return;

//     try {
//         json payload;
//         payload["api_key"] = apiKey;

//         for (auto& email : emailBatch) {
//             json emailObj;
//             emailObj["email_address"] = email.emailAddress;

//             if (email.ipAddress.empty()) {
//                 emailObj["ip_address"] = json::value_t::null;
//             } else {
//                 emailObj["ip_address"] = email.ipAddress;
//             }

//             payload["email_batch"].push_back(emailObj);
//         }

//         cpr::Response reqResponse = requestHandler.Post(
//             cpr::Url{bulkApiBaseUrl + "/validatebatch"},
//             cpr::Header{
//                 {"Accept", "application/json"},
//                 {"Content-Type", "application/json"}
//             },
//             cpr::Body{payload.dump()}
//         );
        
//         std::string rsp = reqResponse.text;

//         if (reqResponse.status_code > 299) {
//             if (errorCallback) {
//                 ZBErrorResponse errorResponse = ZBErrorResponse::parseError(rsp);
//                 errorCallback(errorResponse);
//             }
//         } else {
//             if (successCallback) {
//                 ZBValidateBatchResponse response = ZBValidateBatchResponse::from_json(json::parse(rsp));
//                 successCallback(response);
//             }
//         }
//     } catch (std::exception e) {
//         ZBErrorResponse errorResponse = ZBErrorResponse::parseError(e.what());
//         errorCallback(errorResponse);
//     }
// }

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
