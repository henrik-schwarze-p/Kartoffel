
#ifndef CoreUI_RC_h
#define CoreUI_RC_h

#include "RCSwitch.h"

#define RCCONFIG_TYPE_GROUPS 1
#define RCCONFIG_TYPE_RELAY 2
#define RCCONFIG_TYPE_CODES 3
#define RCCONFIG_TYPE_NOT_CONFIGURED 0

#define RC_WITH_TITLE 1
#define RC_WITHOUT_TITLE 0

// This structure allows to update the RCUI without breaking user space
typedef struct _RCConfig {
    char          port;
    unsigned long codeOn;
    unsigned long codeOff;
    unsigned char type;
    unsigned char group, number;
    unsigned char on;
    unsigned char protocol;  // 1 or 2
} RCConfig;

void showRCDialog(RCConfig* rcconfig, const char* name, void (*callback)());
int  rcConfigured(RCConfig* rcconfig);
void rcSendOn(RCConfig* config);
void rcSendOff(RCConfig* config);
void rcInitCodes(RCConfig* config, char port, unsigned long codeOn, unsigned long codeOff, int protocol);
void rcInitGroups(RCConfig* config, char port, char group, char number);
void rcInitRelay(RCConfig* config, char port);
int  rcState(RCConfig* config);
int  rcOnEnabled(RCConfig* config);
int  rcOffEnabled(RCConfig* config);
void rcPrintConfig(RCConfig* rcconfig, int withTitle);
void rcInit(RCConfig* config);

#endif
