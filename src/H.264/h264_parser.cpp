#include "h264_parser.h"

#define BUFFER_SIZE 100000

const char *NALU::NaluType[] = {
    "SLICE", "DPA", "DPB",
    "DPC", "IDR", "SEI",
    "SPS", "PPS", "AUD",
    "EOSEQ", "EOSTREAM", "FILL"
};

const char *NALU::NaluPriority[] = {
    "DISPOS", "LOW", "HIGH", "HIGHEST"
};

NALU::NALU()
    : start_code_len(0)
    , sodb_len(0)
    , max_size(BUFFER_SIZE)
    , forbidden_bit(0)
    , nalu_reference_idc(0)
    , nalu_unit_type(0)
    , buffer(nullptr)
{
    buffer = new char[BUFFER_SIZE];
}

NALU::~NALU()
{
    if (buffer)
        delete[] buffer;
}

H264Parser::H264Parser(const std::string url = std::string())
    : h264_stream(nullptr)
    , nalu(nullptr)
{
    if (url.size() != 0) {
        h264_stream = fopen(url.c_str(), "rb+");
        if (!h264_stream) {
            return;
        }
    }
    nalu = new NALU;
}

H264Parser::~H264Parser()
{
    if (nalu)
        delete nalu;
}

bool H264Parser::h264_parser()
{
    if (!h264_stream)
        return false;

    int data_offset = 0;
    int nalu_num = 0;
    int type_pos = 0, priority_pos = 0;

    std::cout << "+-----+--------- NALU Table -------+---------+" << std::endl;
    std::cout << "| NUM |   POS   |   IDC   |  TYPE  |   LEN   |" << std::endl;
    std::cout << "+-----+---------+---------+--------+---------+" << std::endl;

    while (!feof(h264_stream)) {
        if (!GetNextNALU()) {
            std::cout << "Error: get next nalu failed\n";
            return false;
        }

        type_pos = nalu->nalu_unit_type;
        priority_pos = nalu->nalu_reference_idc >> 5;
        if (data_offset == 0)
            printf("|%5d|%9d|%9s|%8s|%9d|\n", nalu_num, nalu->start_code_len, NALU::NaluPriority[priority_pos], NALU::NaluType[type_pos], nalu->sodb_len);
        else
            printf("|%5d|%9d|%9s|%8s|%9d|\n", nalu_num, data_offset, NALU::NaluPriority[priority_pos], NALU::NaluType[type_pos], nalu->sodb_len);
        data_offset += nalu->start_code_len + nalu->sodb_len;
        nalu_num++;
    }
    std::cout << "+-----+---------+ Complete --------+---------+" << std::endl;

    return true;
}

bool H264Parser::GetNextNALU()
{
    int cur_pos = 0;
    bool start_code_found = false;
    bool info2 = false, info3 = false;
    bool eof_flag = false;
    char start_code[4];

    if (3 != fread(start_code, 1, 3, h264_stream)) {
        return false;
    }
    info2 = FindStartCode2(start_code);
    if (info2) {
        nalu->start_code_len = 3;
    } else {
        if (1 != fread(start_code + 3, 1, 1, h264_stream)) {
            return false;
        }
        info3 = FindStartCode3(start_code);
        if (info3) {
            nalu->start_code_len = 4;
        } else {
            return false;
        }
    }

    start_code_found = false;
    info2 = info3 = false;
    while (!start_code_found) {
        if (feof(h264_stream)) {
            eof_flag = true;
            break;
        }
        nalu->buffer[cur_pos++] = fgetc(h264_stream);
        info3 = FindStartCode3(nalu->buffer + cur_pos - 4);
        if (!info3) {
            info2 = FindStartCode2(nalu->buffer + cur_pos - 3);
        }
        start_code_found = (info2 || info3);
    }

    if (eof_flag) {
        nalu->sodb_len = cur_pos - 1;
    } else {
        int rwind = info2 ? -3 : -4;
        if (0 != fseek(h264_stream, rwind, SEEK_CUR)) {
            std::cout << "Error: fseek error\n";
            return false;
        }
        nalu->sodb_len = cur_pos + rwind;
    }
    nalu->forbidden_bit = nalu->buffer[0] & 0x80;      //1 bit
    nalu->nalu_reference_idc = nalu->buffer[0] & 0x60; // 2 bit
    nalu->nalu_unit_type = (nalu->buffer[0]) & 0x1f;   // 5 bit
    return true;
}

bool H264Parser::FindStartCode2(char* buffer)
{
    if (buffer[0] != 0 || buffer[1] != 0 || buffer[2] != 1)
        return false;
    else
        return true;
}

bool H264Parser::FindStartCode3(char* buffer)
{
    if (buffer[0] != 0 || buffer[1] != 0 || buffer[2] != 0 || buffer[3] != 1)
        return false;
    else
        return true;
}
