//
// Created by 张新军 on 2023/1/16.
//
#include "AppVersion.h"

namespace PE
{
    namespace VER
    {
        const char *getProgramDesc()
        {
            return "Media service, support RTSP, RTMP, HLS, http-flv and other media format distribution";
        }

        const char *getVersion()
        {
            return VERSION(VER_MAIN, VER_SUB_ADD, VER_SUB_MODIFY);
        }
    }
}