#ifndef __H264_PARSER_H__
#define __H264_PARSER_H__

#include <iostream>
#include <string>

struct NALU
{
    int      start_code_len;     // 4 for parameter sets and first slice in picture, 3 for everything else (suggested)
    unsigned sodb_len;           // Length of the NAL unit (Excluding the start code, which does not belong to the NALU)
    unsigned max_size;           // Nal Unit Buffer size
    int      forbidden_bit;      // should be always FALSE
    int      nalu_reference_idc; // NALU_PRIORITY_xxxx
    int      nalu_unit_type;     // NALU_TYPE_xxxx    
    char    *buffer;             // contains the first byte followed by the EBSP

    const static char* NaluType[];

    const static char* NaluPriority[];

    NALU();

    ~NALU();
};

class H264Parser
{
public:
    H264Parser(const std::string url);

    ~H264Parser();

    bool h264_parser();

private:
    bool GetNextNALU();

    bool FindStartCode2(char* buffer);

    bool FindStartCode3(char* buffer);

private:
    FILE* h264_stream;
    NALU* nalu;
};

#endif
