#include "CCSDS.h"

void serialize_CCSDS_header(ccsds_header_t info, uint8_t* destination){
    destination[0] = 0;
    destination[0] |= CCSDS_PROTOCOL_VERSION << 5;
    destination[0] |= ((info.isCommand ? 1 : 0 ) << 4);
    destination[0] |= ((info.hasSecondaryHeader ? 1 : 0 ) << 3);
    destination[0] |= (info.APID >> 8);
    //destination[1] = 0;
    destination[1] = (info.APID & 0xff);
    destination[2] = 0;
    destination[2] |= (info.seq_type << 6);
    destination[2] |= (info.seq_count >> 8);
    destination[3] = (info.seq_count & 0xff);
    destination[4] = (info.length >> 8);
    destination[5] = (info.length & 0xff);
    return;
}

void serialize_CCSDS_FRC_header(ccsds_frc_header_t info, uint8_t *destination){
    serialize_CCSDS_header(info.ccsds_header, destination);
    destination[6] = (info.integer_time >> 24);
    destination[7] = (info.integer_time >> 16);
    destination[8] = (info.integer_time >> 8);
    destination[9] = info.integer_time;
    destination[10] = (info.fractional_time >> 16);
    destination[11] = (info.fractional_time >> 8);
    destination[12] = (info.fractional_time);
    destination[13] = (info.DFI_type >> 8);
    destination[14] = (info.DFI_type);
    destination[15] = info.DFI_version;
}

void parse_CCSDS_header(ccsds_header_t *info, uint8_t* data){
    info->isCommand = (data[0] >> 4) & 1;
    info->APID = 0; 
    info->APID |= (uint16_t)(data[0] & 0b111)<<8;
    info->APID |= data[1];
    info->seq_type = (CCSDS_sequence_type)(data[2]>>6);
    info->seq_count = 0; 
    info->seq_count |= (uint16_t)(data[2] & 0b111111) << 8;
    info->seq_count |= data[3];
    info->length = ((uint16_t)data[4] << 8) | data[5];
    return;
}

void parse_CCSDS_FRC_header(ccsds_frc_header_t *header, uint8_t *data){
    parse_CCSDS_header(&header->ccsds_header, data);
    header->integer_time = 0;
    header->integer_time |= (data[6] << 24);
    header->integer_time |= (data[7] << 16);
    header->integer_time |= (data[8] << 8);
    header->integer_time |= data[9];
    header->fractional_time = 0;
    header->fractional_time |= (data[10] << 16);
    header->fractional_time |= (data[11] << 8);
    header->fractional_time |= data[12];
    header->DFI_type = 0;
    header->DFI_type |= (data[13] << 8);
    header->DFI_type |= data[14];
    header->DFI_version = data[15];
}

void make_timestamp(uint32_t *integer, uint32_t *fractional){
    auto now_epoch = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::seconds now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now_epoch);
    *integer = now_seconds.count();
    uint16_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now_epoch-now_seconds).count();
    *fractional = ((uint32_t)((float)milliseconds * 16777.216)) & 0xffffff; //2^24/1000
    printf("Integer: %d\n", *integer);
    printf("Fractional: %d\n", *fractional);
}