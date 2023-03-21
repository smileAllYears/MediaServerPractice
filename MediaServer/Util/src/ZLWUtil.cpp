/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-03-21 14:05:07
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 17:13:33
 * @FilePath: /MediaServerPractice/MediaServer/Util/src/Util.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ZLWUtil.h"
#include <limits.h>
#include <unistd.h>

#define TRIM(s, chars) \
do {\
    std::string strmap(0xFF, '\0');\
    for (auto &ch : chars) \
    { \
        strmap[(unsigned char&)ch] = '\1';\
    } \
    while( s.size() && strmap.at((unsigned char&)s.back())) s.pop_back(); \
    while( s.size() && strmap.at((unsigned char&)s.front())) s.erase(0, 1); \
}while(0)

namespace ZLW
{
    namespace UTIL
    {
        std::string exePath(bool isExe)
        {
            char buffer[PATH_MAX * 2 + 1] = {0};
            int n = -1;
            n = readlink("/proc/self/exe", buffer, sizeof(buffer));
            std::string filePath;
            if (n <= 0)
            {
                filePath = "./";
            }
            else
            {
                filePath = buffer;
            }   
            return std::move(filePath);     
        }

        std::string exeDir(bool isExe)
        {
            auto path = exePath(isExe);

            return std::move(path.substr(0, path.rfind('/') + 1));
        }

        std::string exeName(bool isExe)
        {
            auto path = exePath(isExe);
            return std::move(path.substr(path.rfind('/') + 1));
        }

        std::string& trim(std::string &s, const std::string &chars)
        {
            TRIM(s, chars);
            return s;
        }

        std::string trim(std::string &&s, const std::string &chars) {
            TRIM(s, chars);
            return std::move(s);
        }
    }
}