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

/*uint8_t response_size(uint8_t opcode, uint8_t optional = 0) {
    switch (opcode) {
    case GET_FIRMWARE_VERSION:
        return 13;
    case GET_GENERAL_STATUS:
        if (optional == 2) {
            return 21;
        } else if (optional == 1) {
            return 17;
        } else {
            return 0;
        }
    case READ_REGISTER:
        return optional + 9;
    case WRITE_REGISTER:
        return 9;
    case SET_PARAMETERS:
        return 9;
    case SAM_CONFIGURATION:
        return 9;
    case POWER_DOWN:
        return 10;
    case RF_CONFIGURATION:
        return 9;
    case LIST_PASSIVE_TARGETS:
        // Hardcoded for ISO14443-A
        return 20;
    default:
        return 0;
    };
};*/

#endif