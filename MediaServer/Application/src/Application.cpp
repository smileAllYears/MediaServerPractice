/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-01-16 10:37:54
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 11:15:38
 * @FilePath: /MediaServerPractice/MediaServer/Application/src/Application.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by smileForCode on 2023/1/16.
//
#include <signal.h>
#include "glog_fmt.h"
#include "AppVersion.h"
#include "Application.h"


namespace ZLW
{
    namespace APP
    {
        CApplication::CApplication(std::string strAppName):
            m_strAppName(std::move(strAppName))
        {
        }

        void CApplication::run(const char *basedir)
        {
            try
            {
                signal(SIGILL, SIG_IGN);
                signal(SIGPIPE, SIG_IGN);

                // 日志初始化
                std::string strLogPath(basedir);
                strLogPath.append("/logs/");
                LOG::logInit(m_strAppName.c_str(), strLogPath.c_str());
                LOG::setLogCleanLogMode(1);
                // 日志文件最大保存30天
                LOG::setMaxLogFileKeepDays(30);
                // 日志文件最大size 500M
                LOG::setMaxLogFileSize(500);
                LOG::setLogLevel(PE_LOG_LEVEL_INFO);
                // 每隔1秒刷新一次

                LOG::setLogFileFlushInterval(1);

                LOG::PE_LOG_INFO("Application : %s", VER::getProgramDesc());
                LOG::PE_LOG_INFO("Version : %s", VER::getVersion());



            }
            catch (std::exception& ex)
            {

            }
        }

    }
}