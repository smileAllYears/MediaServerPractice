//
// Created by smileForCode on 2023/1/16.
//
#include <signal.h>
#include "glog_fmt.h"
#include "AppVersion.h"
#include "Application.h"


namespace PE
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