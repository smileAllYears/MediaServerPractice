/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-03-21 11:08:24
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 13:46:47
 * @FilePath: /MediaServerPractice/MediaServer/Util/include/CMD.H
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef ZLWMEDIASERVER_CMD_H
#define ZLWMEDIASERVER_CMD_H

#include <map>
#include <mutex>
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace ZLW
{
    namespace UTIL
    {
        class OptionParser
        {
            public: 
                using OptionCompleted = std::function<bool(const std::shared_ptr)>
        }
    }
}

#endif