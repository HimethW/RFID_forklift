/*
    MIFARE_Classic_Commands.h

    compiled by Pulasthi Udugamasooriya, July 12, 2025

    for "RFID Reader for Forklift"
    Course Project,
    EN2160 - Electronic Design Realization,
    Semester 4, University of Moratuwa

    
    Commands supported by MIFARE Classic Cards.

    The following sources were referenced.

    https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf
    https://www.nxp.com/docs/en/user-guide/141520.pdf
*/

#ifndef MIFARE_CLASSIC_COMMANDS_H
#define MIFARE_CLASSIC_COMMANDS_H

#define AUTHENTICATE_KEY_A  0x60
#define AUTHENTICATE_KEY_B  0x61
#define READ_BLOCK          0x30
#define WRITE_BLOCK         0xA0
#define DECREMENT_BLOCK     0xC0
#define INCREMENT_BLOCK     0xC1
#define RESTORE_BLOCK       0xC2
#define TRANSFER_BLOCK      0xB0

#endif