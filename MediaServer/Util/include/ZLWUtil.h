/*
 * @Author: zhangxinjun 
 * @Date: 2023-03-21 14:05:01
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 17:13:59
 * @FilePath: /MediaServerPractice/MediaServer/Util/include/Util.h
 * @Description: 通用组件
 */
#ifndef ZLWMEDIASERVER_UTIL_H
#define ZLWMEDIASERVER_UTIL_H

#include <string>

namespace ZLW
{
    namespace UTIL
    {
        std::string exePath(bool isExe = true);
        std::string exeDir(bool isExe = true);
        std::string exeName(bool isExe = true);
        std::string& trim(std::string &s, const std::string& chars = " \r\n\t");
        std::string trim(std::string &&s, const std::string& chars = " \r\n\t");
    }
}

#endif