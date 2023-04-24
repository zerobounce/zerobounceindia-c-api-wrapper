#include <curl/curl.h>
#include "unity.h"
#include "fff.h"

#include "ZeroBounce/ZeroBounce.h"

DEFINE_FFF_GLOBALS;

static ZeroBounce* zb;
static ZBErrorResponse expected_error_response;
static ZBCreditsResponse expected_credits_response;
static ZBGetApiUsageResponse expected_api_usage_response;
static ZBValidateResponse expected_validate_response;
static ZBValidateBatchResponse expected_validate_batch_response;
static ZBSendFileResponse expected_send_file_response;
static ZBFileStatusResponse expected_file_status_response;
static ZBGetFileResponse expected_get_file_response;
static ZBDeleteFileResponse expected_delete_file_response;
static ZBActivityDataResponse expected_activity_data_response;

static char* response_json = "";

void on_error_valid(ZBErrorResponse error_response) {
    TEST_FAIL_MESSAGE(zb_error_response_to_string(&error_response));
}

void on_error_invalid(ZBErrorResponse error_response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_error_response, &error_response, sizeof(ZBErrorResponse));
}

void on_success_credits_valid(ZBCreditsResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_credits_response, &response, sizeof(ZBCreditsResponse));
}

void on_success_credits_invalid(ZBCreditsResponse response) {
    TEST_FAIL_MESSAGE(zb_credits_response_to_string(&response));
}

void on_success_api_usage_valid(ZBGetApiUsageResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_api_usage_response, &response, sizeof(ZBGetApiUsageResponse));
}

void on_success_api_usage_invalid(ZBGetApiUsageResponse response) {
    TEST_FAIL_MESSAGE(zb_get_api_usage_response_to_string(&response));
}

void on_success_validate_valid(ZBValidateResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_validate_response, &response, sizeof(ZBValidateResponse));
}

void on_success_validate_invalid(ZBValidateResponse response) {
    TEST_FAIL_MESSAGE(zb_validate_response_to_string(&response));
}

void on_success_validate_batch_valid(ZBValidateBatchResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_validate_batch_response, &response, sizeof(ZBValidateBatchResponse));
}

void on_success_validate_batch_invalid(ZBValidateBatchResponse response) {
    TEST_FAIL_MESSAGE(zb_validate_batch_response_to_string(&response));
}

void on_success_send_file_valid(ZBSendFileResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_send_file_response, &response, sizeof(ZBSendFileResponse));
}

void on_success_send_file_invalid(ZBSendFileResponse response) {
    TEST_FAIL_MESSAGE(zb_send_file_response_to_string(&response));
}

void on_success_file_status_valid(ZBFileStatusResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_file_status_response, &response, sizeof(ZBFileStatusResponse));
}

void on_success_file_status_invalid(ZBFileStatusResponse response) {
    TEST_FAIL_MESSAGE(zb_file_status_response_to_string(&response));
}

void on_success_get_file_valid(ZBGetFileResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_get_file_response, &response, sizeof(ZBGetFileResponse));
}

void on_success_get_file_invalid(ZBGetFileResponse response) {
    TEST_FAIL_MESSAGE(zb_get_file_response_to_string(&response));
}

void on_success_delete_file_valid(ZBDeleteFileResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_delete_file_response, &response, sizeof(ZBDeleteFileResponse));
}

void on_success_delete_file_invalid(ZBDeleteFileResponse response) {
    TEST_FAIL_MESSAGE(zb_delete_file_response_to_string(&response));
}

void on_success_activity_data_valid(ZBActivityDataResponse response) {
    TEST_ASSERT_EQUAL_MEMORY(&expected_activity_data_response, &response, sizeof(ZBActivityDataResponse));
}

void on_success_activity_data_invalid(ZBActivityDataResponse response) {
    TEST_FAIL_MESSAGE(zb_activity_data_response_to_string(&response));
}


size_t mock_write_callback(void *data, size_t size, size_t nmemb, void *clientp) {
    size_t real_size = strlen(response_json);
    memory *mem = (memory*)clientp;
    
    mem->response = (char*)malloc(real_size + 1);
    strcpy(mem->response, response_json);
    mem->size = real_size;

    return real_size;
}

CURLcode mock_curl_easy_perform(CURL *curl) {
    return CURLE_OK;
}

FAKE_VALUE_FUNC(size_t, write_callback, void*, size_t, size_t, void*);
//FAKE_VALUE_FUNC(CURLcode, curl_easy_perform, CURL*);

void setUp(void) {
    zb = zero_bounce_get_instance();
    zero_bounce_initialize(zb, "api_key");

    RESET_FAKE(write_callback);
    //RESET_FAKE(curl_easy_perform);
}

void tearDown(void) {
}

void test_get_credits_invalid(void)
{
    char* response_json = "{\"Credits\":\"-1\"}";

    expected_error_response = parse_error(response_json);
    get_credits(zb, on_success_credits_invalid, on_error_invalid);
}

void test_get_credits_valid(void)
{
    response_json = "{\"Credits\":\"100\"}";

    write_callback_fake.custom_fake = mock_write_callback;
    //curl_easy_perform_fake.custom_fake = mock_curl_easy_perform;

    expected_credits_response = zb_credits_response_from_json(json_tokener_parse(response_json));
    get_credits(zb, on_success_credits_valid, on_error_valid);
}

int main(void)
{
    UNITY_BEGIN();

    // RUN_TEST(test_get_credits_invalid);
    RUN_TEST(test_get_credits_valid);

    return UNITY_END();
}