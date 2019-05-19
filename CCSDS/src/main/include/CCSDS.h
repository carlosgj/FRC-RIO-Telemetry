#pragma once
#include <stdint.h>
#include <chrono>
#include <iostream>

#define CCSDS_PROTOCOL_VERSION 0

enum CCSDS_sequence_type {
    CONTINUATION = 0b00,
    FIRST = 0b01,
    LAST = 0b10,
    UNSEGMENTED = 0b11
};

struct ccsds_header_t {
    bool isCommand;
    bool hasSecondaryHeader;
    unsigned int APID : 11;
    CCSDS_sequence_type seq_type;
    uint16_t seq_count;
    uint16_t length; //Remember: data length MINUS 1
};

struct ccsds_frc_header_t {
    ccsds_header_t ccsds_header;
    uint32_t integer_time;
    unsigned int fractional_time : 24;
    uint16_t DFI_type;
    uint8_t DFI_version;
};

void serialize_CCSDS_header(ccsds_header_t info, uint8_t* destination);
void parse_CCSDS_header(ccsds_header_t *info, uint8_t* data);
void serialize_CCSDS_FRC_header(ccsds_frc_header_t info, uint8_t *destination);
void parse_CCSDS_FRC_header(ccsds_frc_header_t *header, uint8_t *data);
void make_timestamp(uint32_t *integer, uint32_t *fractional);