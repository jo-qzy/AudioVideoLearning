#ifndef __PCM_PARSER_H__
#define __PCM_PARSER_H__

#include <string>

class PcmParser
{
public:
    static bool pcm_split(const std::string url);
};

#endif