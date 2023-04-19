#include <stdio.h>
#include <string.h>

#include "ZeroBounce/ZBValidateResponse.h"
#include "ZeroBounce/utils.h"

ZBValidateStatus status_from_string(const char* string) {
    if (strcmp(string, "valid") == 0) {
        return Valid;
    } else if (strcmp(string, "invalid") == 0) {
        return Invalid;
    } else if (strcmp(string, "catch-all") == 0) {
        return CatchAll;
    } else if (strcmp(string, "spamtrap") == 0) {
        return Spamtrap;
    } else if (strcmp(string, "abuse") == 0) {
        return Abuse;
    } else if (strcmp(string, "do_not_mail") == 0) {
        return DoNotMail;
    }

    return Unknown;
}

ZBValidateSubStatus sub_status_from_string(const char* string) {
    if (strcmp(string, "antispam_system") == 0) {
        return AntispamSystem;
    } else if (strcmp(string, "greylisted") == 0) {
        return Greylisted;
    } else if (strcmp(string, "mail_server_temporary_error") == 0) {
        return MailServerTemporaryError;
    } else if (strcmp(string, "forcible_disconnect") == 0) {
        return ForcibleDisconnect;
    } else if (strcmp(string, "mail_server_did_not_respond") == 0) {
        return MailServerDidNotRespond;
    } else if (strcmp(string, "timeout_exceeded") == 0) {
        return TimeoutExceeded;
    } else if (strcmp(string, "failed_smtp_connection") == 0) {
        return FailedSmtpConnection;
    } else if (strcmp(string, "mailbox_quota_exceeded") == 0) {
        return MailboxQuotaExceeded;
    } else if (strcmp(string, "exception_occurred") == 0) {
        return ExceptionOccurred;
    } else if (strcmp(string, "possible_trap") == 0) {
        return PossibleTrap;
    } else if (strcmp(string, "role_based") == 0) {
        return RoleBased;
    } else if (strcmp(string, "global_suppression") == 0) {
        return GlobalSuppression;
    } else if (strcmp(string, "mailbox_not_found") == 0) {
        return MailboxNotFound;
    } else if (strcmp(string, "no_dns_entries") == 0) {
        return NoDnsEntries;
    } else if (strcmp(string, "failed_syntax_check") == 0) {
        return FailedSyntaxCheck;
    } else if (strcmp(string, "possible_typo") == 0) {
        return PossibleTypo;
    } else if (strcmp(string, "unroutable_ip_address") == 0) {
        return UnroutableIpAddress;
    } else if (strcmp(string, "leading_period_removed") == 0) {
        return LeadingPeriodRemoved;
    } else if (strcmp(string, "does_not_accept_mail") == 0) {
        return DoesNotAcceptMail;
    } else if (strcmp(string, "alias_address") == 0) {
        return AliasAddress;
    } else if (strcmp(string, "role_based_catch_all") == 0) {
        return RoleBasedCatchAll;
    } else if (strcmp(string, "disposable") == 0) {
        return Disposable;
    } else if (strcmp(string, "toxic") == 0) {
        return Toxic;
    }

    return None;
}

ZBValidateResponse new_zb_validate_response() {
    ZBValidateResponse response;

    response.address = "";
    response.status = Unknown;
    response.sub_status = None;
    response.account = "";
    response.domain = "";
    response.did_you_mean = "";
    response.domain_age_days = "";
    response.free_email = false;
    response.mx_found = false;
    response.mx_record = "";
    response.smtp_provider = "";
    response.first_name = "";
    response.last_name = "";
    response.gender = "";
    response.city = "";
    response.region = "";
    response.zip_code = "";
    response.country = "";
    response.processed_at = "";
    response.error = "";

    return response;
}

char* zb_validate_response_to_string(ZBValidateResponse* response) {
    const char *serialization = "ZBValidateResponse{"
        "address='%s'"
        ", status=%d"
        ", sub_status=%d"
        ", free_email=%d"
        ", did_you_mean='%s'"
        ", account='%s'"
        ", domain='%s'"
        ", domain_age_days='%s'"
        ", smtp_provider='%s'"
        ", mx_found=%d"
        ", mx_record='%s'"
        ", first_name='%s'"
        ", last_name='%s'"
        ", gender='%s'"
        ", country='%s'"
        ", region='%s'"
        ", city='%s'"
        ", zip_code='%s'"
        ", processed_at='%s'"
        ", error='%s'"
        "}";

    int size = snprintf(
        NULL, 0, serialization,
        response->address,
        response->status,
        response->sub_status,
        response->account,
        response->domain,
        response->did_you_mean,
        response->domain_age_days,
        response->free_email,
        response->mx_found,
        response->mx_record,
        response->smtp_provider,
        response->first_name,
        response->last_name,
        response->gender,
        response->city,
        response->region,
        response->zip_code,
        response->country,
        response->processed_at,
        response->error
    );

    if (size < 0) {
        return NULL;
    }

    char* buffer = (char*) malloc(size + 1);
    if (!buffer) {
        return NULL;
    }

    snprintf(
        buffer, size + 1, serialization,
        response->address,
        response->status,
        response->sub_status,
        response->account,
        response->domain,
        response->did_you_mean,
        response->domain_age_days,
        response->free_email,
        response->mx_found,
        response->mx_record,
        response->smtp_provider,
        response->first_name,
        response->last_name,
        response->gender,
        response->city,
        response->region,
        response->zip_code,
        response->country,
        response->processed_at,
        response->error
    );
    return buffer;
}

ZBValidateResponse zb_validate_response_from_json(const json_object* j) {
    ZBValidateResponse r = new_zb_validate_response();

    r.address = (char*)get_json_value(j, json_type_string, "address", (void*)"");
    r.status = status_from_string((char*)get_json_value(j, json_type_string, "status", (void*)""));
    r.sub_status = sub_status_from_string((char*)get_json_value(j, json_type_string, "sub_status", (void*)""));
    r.account = (char*)get_json_value(j, json_type_string, "account", (void*)"");
    r.domain = (char*)get_json_value(j, json_type_string, "domain", (void*)"");
    r.did_you_mean = (char*)get_json_value(j, json_type_string, "did_you_mean", (void*)"");
    r.domain_age_days = (char*)get_json_value(j, json_type_string, "domain_age_days", (void*)"");
    r.free_email = *(bool*)get_json_value(j, json_type_boolean, "free_email", &(bool){false});

    if (strcasecmp((char*)get_json_value(j, json_type_string, "mx_found", (void*)"false"), "true") == 0) {
        r.mx_found = true;
    }

    r.mx_record = (char*)get_json_value(j, json_type_string, "mx_record", (void*)"");
    r.smtp_provider = (char*)get_json_value(j, json_type_string, "smtp_provider", (void*)"");
    r.first_name = (char*)get_json_value(j, json_type_string, "firstname", (void*)"");
    r.last_name = (char*)get_json_value(j, json_type_string, "lastname", (void*)"");
    r.gender = (char*)get_json_value(j, json_type_string, "gender", (void*)"");
    r.city = (char*)get_json_value(j, json_type_string, "city", (void*)"");
    r.region = (char*)get_json_value(j, json_type_string, "region", (void*)"");
    r.zip_code = (char*)get_json_value(j, json_type_string, "zipcode", (void*)"");
    r.country = (char*)get_json_value(j, json_type_string, "country", (void*)"");
    r.processed_at = (char*)get_json_value(j, json_type_string, "processed_at", (void*)"");
    r.error = (char*)get_json_value(j, json_type_string, "error", (void*)"");

    return r;
}