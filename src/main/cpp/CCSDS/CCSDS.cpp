#include "CCSDS/CCSDS.h"

void create_CCSDS_header(bool isCommand, uint16_t APID, CCSDS_sequence_type seq_type, uint16_t count, uint16_t length, uint8_t* destination){
    APID &= 0x7ff; //Sanitize APID to 11 bits
    destination[0] = 0;
    destination[0] |= CCSDS_PROTOCOL_VERSION << 5;
    destination[0] |= ((isCommand ? 1 : 0 ) << 4);
    //destination[0] |= 0b00001000; //Sec hdr flag
    destination[0] |= (APID >> 8);
    //destination[1] = 0;
    destination[1] = (APID & 0xff);
    destination[2] = 0;
    destination[2] |= (seq_type << 6);
    destination[2] |= (count >> 8);
    destination[3] = (count & 0xff);
    destination[4] = (length >> 8);
    destination[5] = (length & 0xff);
    return;
}

void parse_CCSDS_header(bool *isCommand, uint16_t *APID, CCSDS_sequence_type *seq_type, uint16_t *count, uint16_t *length, uint8_t* data){
    *isCommand = (data[0] >> 4) & 1;
    *APID = 0; 
    *APID |= (uint16_t)(data[0] & 0b111)<<8;
    *APID |= data[1];
    *seq_type = (CCSDS_sequence_type)(data[2]>>6);
    *count = 0; 
    *count |= (uint16_t)(data[2] & 0b111111) << 8;
    *count |= data[3];
    *length = ((uint16_t)data[4] << 8) | data[5];
    return;
}