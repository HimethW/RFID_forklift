#ifndef PN5180_COMMANDS_H
#define PN5180_COMMANDS_H

#define WRITE_REGISTER      0x00
#define WRITE_REGISTER_OR   0x01
#define WRITE_REGISTER_AND  0x02

#define READ_REGISTER       0x04

#define READ_EEPROM         0x07

#define WRITE_TX_DATA       0x08
#define SEND_DATA           0x09
#define READ_DATA           0x0A

#define LOAD_RF_CONFIG      0x11

#define RF_ON               0x16
#define RF_OFF              0x17

#endif