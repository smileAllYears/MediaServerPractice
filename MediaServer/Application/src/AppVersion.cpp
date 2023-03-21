/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-01-16 17:18:57
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 11:15:50
 * @FilePath: /MediaServerPractice/MediaServer/Application/src/AppVersion.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by 张新军 on 2023/1/16.
//
#include "AppVersion.h"

namespace ZLW
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