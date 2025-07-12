/*
    PN532_Commands.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Commands supported by the PN532 RFID chip.

    The following sources were referenced.

    https://github.com/adafruit/Adafruit-PN532/blob/bf9f3f31961474fdbca23cb57d604c7f27269baf/Adafruit_PN532.h

    https://www.nxp.com/docs/en/user-guide/141520.pdf, Sections 7.2 and 7.3
*/

#ifndef PN532_COMMANDS_H
#define PN532_COMMANDS_H

#define GET_FIRMWARE_VERSION    0x02
#define GET_GENERAL_STATUS      0x04
#define READ_REGISTER           0x06
#define WRITE_REGISTER          0x08
#define SET_PARAMETERS          0x12
#define SAM_CONFIGURATION       0x14
#define POWER_DOWN              0x16

#define RF_CONFIGURATION        0x32

#define LIST_PASSIVE_TARGETS    0x4A
#define DATA_EXCHANGE           0x40

#endif