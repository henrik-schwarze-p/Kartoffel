
#include "UI.h"
#include "Drawing.h"
#include "Text.h"
#include "UINumPad.h"
#include "UIPort.h"
#include "UIRC.h"
#include "RCSwitch.h"
#include "Namespaces.h"
#include "Arduino.h"
#include "Resources.h"

/*
 Gruppe I:
 1 ON: 1381717     1 OFF: 1381716
 2 ON: 1394005     2 OFF: 1394004
 3 ON: 1397077     3 OFF: 1397076
 4 ON: 1397845     4 OFF: 1397844

 Gruppe II:
 1 ON: 4527445     1 OFF: 4527444
 2 ON: 4539733     2 OFF: 4539732
 3 ON: 4542805     3 OFF: 4542804
 4 ON: 4543573     4 OFF: 4543572

 Gruppe III:
 1 ON: 5313877     1 OFF: 5313876
 2 ON: 5326165     2 OFF: 5326164
 3 ON: 5329237     3 OFF: 5329236
 4 ON: 5330005     4 OFF: 5330004

 Gruppe IV:
 1 ON: 5510485     1 OFF: 5510484
 2 ON: 5522773     2 OFF: 5522772
 3 ON: 5525845     3 OFF: 5525844
 4 ON: 5526613     4 OFF: 5526612

 */

// RC DIALOG

namespace rcDialog
{
    RCConfig *conf;
    const char *name = 0;

    void (*screenCallback)();

    void test();
    void configureRC();
    void configure();
    void relay();
    void groupsX();
    void groups();
    void groupOfCont2();
    void codes();

    void configureOffCallback(int ok, long value)
    {
        if (ok)
            conf->codeOff = (long)value;
        goToScreen(configureRC);
    }

    void configureOff(int param)
    {
        showNumPad(PSTR("Off Code"), conf->codeOff, 0, 100000000, configureOffCallback);
    }

    void configureOnCallback(int ok, long value)
    {
        if (ok)
            conf->codeOn = (long)value;
        goToScreen(configureRC);
    }

    void configureOn(int param)
    {
        showNumPad(PSTR("On Code"), conf->codeOn, 0, 100000000, configureOnCallback);
    }

    void configureRCPortCallback(int port)
    {
        if (port)
            conf->port = port;
        goToScreen(configureRC);
    }

    void configureRCPort(int param)
    {
        showDigitalPortDialog(conf->port, configureRCPortCallback);
    }

    void relay4(int port)
    {
        if (port)
        {
            conf->type = RCCONFIG_TYPE_RELAY;
            conf->port = port;
        }
        goToScreen(relay);
    }

    void relay3(int param)
    {
        showDigitalPortDialog(conf->port, relay4);
    }

    void relay()
    {
        println(PSTR("The on/off state will be sentt to a digital pin. You can attach a Relay to this pin."));
        println();
        setBoldStyle();
        print(PSTR("Warning"));
        setNormalStyle();
        println(
            PSTR(": if you are not an electrician, don't use a Relay with high voltages. It can be dangerous. "
                 "One fault, and your "
                 "whole circuit can become on high voltage. Do it only if you are an expert!"));
        toolbarAdd(PSTR("|Pin"), relay3);
        toolbarAddScreenBack(configure);
    }

    // -------- ---------
    void printRoman(int param)
    {
        if (param == 1)
            print(PSTR("I"));
        if (param == 2)
            print(PSTR("II"));
        if (param == 3)
            print(PSTR("III"));
        if (param == 4)
            print(PSTR("IV"));
    }

    // -------- GROUPS -----------

    void numberCont2()
    {
        rcPrintConfig(conf, 1);
        toolbarAddScreenBack(configure);
    }

    void numberOnCont(int param)
    {
        conf->number = param;
        goToScreen(numberCont2);
    }

    void groupCont2()
    {
        print(PSTR("You chose group "));
        printRoman(conf->group);
        println(PSTR("."));
        println();
        println(PSTR("Which socket? Sockets can be 1, 2, 3 or 4."));
        toolbarAdd(PSTR("1"), numberOnCont, 1);
        toolbarAdd(PSTR("2"), numberOnCont, 2);
        toolbarAdd(PSTR("3"), numberOnCont, 3);
        toolbarAdd(PSTR("4"), numberOnCont, 4);
        toolbarAddScreenBack(groups);
    }

    void groupCont(int param)
    {
        conf->group = param;
        goToScreen(groupCont2);
    }

    void portCallback(int port)
    {
        if (port)
            conf->port = port;
        goToScreen(groups);
    }

    void port(int param)
    {
        showDigitalPortDialog(conf->port, portCallback);
    }

    void groups()
    {
        conf->type = RCCONFIG_TYPE_GROUPS;
        if (!conf->group)
            conf->group = 1;
        if (!conf->number)
            conf->number = 1;
        println(PSTR("Do you want to set the port or the group? Groups are I, II, III and IV."));
        toolbarAdd(PSTR(" Pin "), port);
        toolbarAdd(PSTR("  I  "), groupCont, 1);
        toolbarAdd(PSTR("  II  "), groupCont, 2);
        toolbarAdd(PSTR("  III  "), groupCont, 3);
        toolbarAdd(PSTR("  IV  "), groupCont, 4);
        toolbarAddScreenBack(configure);
    }

    void codesPortCallback(int port)
    {
        if (port)
            conf->port = port;
        goToScreen(codes);
    }

    void codesPort(int param)
    {
        showDigitalPortDialog(conf->port, codesPortCallback);
    }

    void codesOnCallback(int succes, long value)
    {
        if (succes)
        {
            conf->codeOn = value;
        }
        goToScreen(codes);
    }

    void codesOn(int param)
    {
        showNumPad(PSTR("Please set the code for On"), conf->codeOn, 0, 1000000000, codesOnCallback);
    }

    void codesOffCallback(int succes, long value)
    {
        if (succes)
        {
            conf->codeOff = value;
        }
        goToScreen(codes);
    }

    void codesOff(int param)
    {
        showNumPad(PSTR("Please set the code for Off"), conf->codeOff, 0, 1000000000, codesOffCallback);
    }

    void protocol(int param)
    {
        conf->protocol++;
        if (conf->protocol == 13)
            conf->protocol = 1;
        goToScreen(codes);
    }

    void codes()
    {
        conf->type = RCCONFIG_TYPE_CODES;
        if (!conf->codeOn)
            conf->codeOn = 12999333;
        if (!conf->codeOff)
            conf->codeOff = 12999334;
        print(PSTR("Protocol is "));
        print(conf->protocol);
        println(PSTR("."));
        println();
        println(PSTR("Do you want to set the code for On or Off or the pin where the RC is?"));
        toolbarAdd(PSTR(" |Pin "), codesPort);
        toolbarAdd(PSTR("Protocol"), protocol);
        toolbarAdd(PSTR("  On  "), codesOn);
        toolbarAdd(PSTR("  Off  "), codesOff);
        toolbarAddScreenBack(configure);
    }

    void configure()
    {
        print(PSTR("You can configure the "));
        print(name);
        println(PSTR(" as one of the following:"));
        println();
        println(PSTR("1. RC with groups (f.e.: II/3)"));
        println(PSTR("2. RC with codes for on and off"));
        setPrintX(21);
        println(PSTR("(f.e.: 16377783/16377848)"));
        setPrintX(margin);
        println(PSTR("3. A normal digital pin (or Relay)"));
        toolbarAddScreen(PSTR("Groups"), groups);
        toolbarAddScreen(PSTR("Codes"), codes);
        toolbarAddScreen(PSTR("Pin/Relay"), relay);
        toolbarAddScreenBack(configureRC);
    }

    void testOn(int param)
    {
        rcSendOn(conf);
        goToScreen(configureRC);
    }

    void testOff(int param)
    {
        rcSendOff(conf);
        goToScreen(configureRC);
    }

    void configureRC()
    {
        print(PSTR("Do you want to configure the "));
        print(name);
        println(PSTR(" or to test it? "));
        println();
        rcPrintConfig(conf, RC_WITH_TITLE);
        toolbarAddScreen(PSTR("Configure"), configure);
        toolbarAdd(rcConfigured(conf), PSTR("Test On"), testOn);
        toolbarAdd(rcConfigured(conf), PSTR("Test Off"), testOff);
        toolbarAddScreenBack(screenCallback);
    }

    void rcPrintConfigImpl(RCConfig *rcconfig, int withTitle)
    {
        if (rcconfig->type == RCCONFIG_TYPE_GROUPS)
        {
            if (withTitle)
            {
                print(PSTR("* Switch configured as "));
                setBoldStyle();
                print(PSTR("Groups "));
                setNormalStyle();
                println();
            }
            addPrintX(12);
            print(PSTR("Configuration: "));
            printRoman(rcconfig->group);
            print(PSTR("/"));
            println(rcconfig->number);
        }
        else if (rcconfig->type == RCCONFIG_TYPE_CODES)
        {
            if (withTitle)
            {
                print(PSTR("* Switch configured as "));
                setBoldStyle();
                println(PSTR("Codes"));
                setNormalStyle();
            }
            print(PSTR("On Code: "));
            println(rcconfig->codeOn);
            print(PSTR("Off Code: "));
            println(rcconfig->codeOff);
        }
        else if (rcconfig->type == RCCONFIG_TYPE_RELAY)
        {
            if (withTitle)
            {
                print(PSTR("* Switch configured as "));
                setBoldStyle();
                println(PSTR("Relay"));
                setNormalStyle();
            }
        }
        else
        {
            println(PSTR("* Switch currently not configured."));
            println();
        }
        // indent
        addPrintX(12);
        if (rcconfig->type != RCCONFIG_TYPE_NOT_CONFIGURED)
        {
            if (rcconfig->port)
            {
                print(PSTR("Switch Pin: "));
                println(rcconfig->port);
            }
            else
            {
                println(PSTR("Pin not set."));
            }
        }
        addPrintX(12);
        if (rcConfigured(rcconfig))
        {
            if (rcconfig->on)
            {
                cprintln(PSTR("The device is ON"));
            }
            else
            {
                cprintln(PSTR("The device is OFF"));
            }
        }
    }
    void sendOn(RCConfig *config)
    {
        config->on = true;
        if (config->type == RCCONFIG_TYPE_RELAY)
        {
            pinMode(config->port, OUTPUT);
            pmDigitalWrite(config->port, 1);
        }
        else if (config->type == RCCONFIG_TYPE_CODES)
        {
            rcSwitch.enableTransmit(config->port);
            rcSwitch.setProtocol(config->protocol);
            int bits = 24;
            if (config->codeOn >= 16777216)
            {
                bits = 32;
            }
            rcSwitch.send(config->codeOn, bits);
        }
        else
        {
            rcSwitch.setProtocol(1);
            rcSwitch.enableTransmit(config->port);
            rcSwitch.setProtocol(config->protocol);
            rcSwitch.switchOn(config->group, config->number);
        }
    }
    void sendOff(RCConfig *config)
    {
        config->on = false;
        if (config->type == RCCONFIG_TYPE_RELAY)
        {
            pinMode(config->port, OUTPUT);
            pmDigitalWrite(config->port, 0);
        }
        else if (config->type == RCCONFIG_TYPE_CODES)
        {
            rcSwitch.enableTransmit(config->port);
            rcSwitch.setProtocol(config->protocol);
            int bits = 24;
            if (config->codeOff >= 16777216)
            {
                bits = 32;
            }
            rcSwitch.send(config->codeOff, bits);
        }
        else
        {
            rcSwitch.setProtocol(1);
            rcSwitch.enableTransmit(config->port);
            rcSwitch.switchOff(config->group, config->number);
        }
    }
}

void rcPrintConfig(RCConfig *rcconfig, int withTitle)
{
    rcDialog::rcPrintConfigImpl(rcconfig, withTitle);
}

int rcConfigured(RCConfig *config)
{
    int c = config->type == RCCONFIG_TYPE_GROUPS && config->group && config->number;
    c += config->type == RCCONFIG_TYPE_CODES && config->codeOn && config->codeOff;
    c += config->type == RCCONFIG_TYPE_RELAY;
    return c && config->port;
}

void rcSendOn(RCConfig *config)
{
    rcDialog::sendOn(config);
}

void rcSendOff(RCConfig *config)
{
    rcDialog::sendOff(config);
}

void showRCDialog(RCConfig *rcconfig, const char *name, void (*callback)())
{
    rcDialog::name = name;
    rcDialog::conf = rcconfig;
    rcDialog::screenCallback = callback;
    goToScreen(rcDialog::configureRC);
}

void rcInitCodes(RCConfig *config, char port, unsigned long codeOn, unsigned long codeOff, int protocol)
{
    rcInit(config);
    config->protocol = protocol;
    config->type = RCCONFIG_TYPE_CODES;
    config->port = port;
    config->codeOn = codeOn;
    config->codeOff = codeOff;
}

void rcInitGroups(RCConfig *config, char port, char group, char number)
{
    rcInit(config);
    config->type = RCCONFIG_TYPE_GROUPS;
    config->port = port;
    config->group = group;
    config->number = number;
}

void rcInitRelay(RCConfig *config, char port)
{
    rcInit(config);
    config->type = RCCONFIG_TYPE_RELAY;
    config->port = port;
}

void rcInit(RCConfig *config)
{
    config->type = 0;
    config->port = 0;
    config->group = 0;
    config->number = 0;
    config->codeOn = 0;
    config->codeOff = 0;
    config->protocol = 1;

    config->on = 0;
}

int rcState(RCConfig *config)
{
    return config->port + (int)config->codeOn + (int)config->codeOff;
}

int rcOnEnabled(RCConfig *config)
{
    return config->port && config->codeOn;
}

int rcOffEnabled(RCConfig *config)
{
    return config->port && config->codeOff;
}
