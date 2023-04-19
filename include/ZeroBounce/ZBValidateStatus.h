#ifndef ZBVALIDATESTATUS_H
#define ZBVALIDATESTATUS_H

#include <string.h>

typedef enum {
    Unknown,
    Valid,
    Invalid,
    CatchAll,
    Spamtrap,
    Abuse,
    DoNotMail
} ZBValidateStatus;

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
    } else {
        return Unknown;
    }
}

#endif