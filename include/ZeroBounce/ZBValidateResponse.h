#ifndef ZBVALIDATERESPONSE_H
#define ZBVALIDATERESPONSE_H

#include <stdbool.h>

#include <json-c/json.h>

typedef enum {
    Unknown,
    Valid,
    Invalid,
    CatchAll,
    Spamtrap,
    Abuse,
    DoNotMail
} ZBValidateStatus;

ZBValidateStatus status_from_string(const char* string);


typedef enum {
    None,
    AntispamSystem,
    Greylisted,
    MailServerTemporaryError,
    ForcibleDisconnect,
    MailServerDidNotRespond,
    TimeoutExceeded,
    FailedSmtpConnection,
    MailboxQuotaExceeded,
    ExceptionOccurred,
    PossibleTrap,
    RoleBased,
    GlobalSuppression,
    MailboxNotFound,
    NoDnsEntries,
    FailedSyntaxCheck,
    PossibleTypo,
    UnroutableIpAddress,
    LeadingPeriodRemoved,
    DoesNotAcceptMail,
    AliasAddress,
    RoleBasedCatchAll,
    Disposable,
    Toxic
} ZBValidateSubStatus;

ZBValidateSubStatus sub_status_from_string(const char* string);


typedef struct {
    char* address;
    ZBValidateStatus status;
    ZBValidateSubStatus sub_status;
    char* account;
    char* domain;
    char* did_you_mean;
    char* domain_age_days;
    bool free_email;
    bool mx_found;
    char* mx_record;
    char* smtp_provider;
    char* first_name;
    char* last_name;
    char* gender;
    char* city;
    char* region;
    char* zip_code;
    char* country;
    char* processed_at;
    char* error;
} ZBValidateResponse;

ZBValidateResponse new_zb_validate_response();

char* zb_validate_response_to_string(ZBValidateResponse* response);

ZBValidateResponse zb_validate_response_from_json(const json_object* j);

#endif