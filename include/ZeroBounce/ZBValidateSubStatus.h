#ifndef ZBVALIDATESUBSTATUS_H
#define ZBVALIDATESUBSTATUS_H

#include <string.h>

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
    } else {
        return None;
    }

#endif