
#pragma once
#include <stdint.h>

#define CCSDS_PROTOCOL_VERSION 0

enum CCSDS_sequence_type {
    CONTINUATION = 0b00,
    FIRST = 0b01,
    LAST = 0b10,
    UNSEGMENTED = 0b11
};



void create_CCSDS_header(bool isCommand, uint16_t APID, CCSDS_sequence_type seq_type, uint16_t count, uint16_t length, uint8_t* destination);

void parse_CCSDS_header(bool *isCommand, uint16_t *APID, CCSDS_sequence_type *seq_type, uint16_t *count, uint16_t *length, uint8_t* data);