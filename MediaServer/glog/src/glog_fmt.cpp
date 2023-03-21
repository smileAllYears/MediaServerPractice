//
// Created by smileForCode on 2023/1/16.
//
#include "logging.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#ifdef __linux__
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#include "glog_fmt.h"

// 记录日志级别
static std::atomic<int> g_nLogLevel(PE_LOG_LEVEL_INFO);
// 记录打印模块
static std::atomic<int> g_nPrintModule(PE_DEFAULT_MODULE);
// 记录日志文件最大个数
static std::atomic<int> g_nMaxLogFileCount(200);
// 历史日志文件最大个数
static std::atomic<int> g_nMaxHxLogFileCount(200);
// 自动清理日志
static std::atomic<bool> g_bAutoClean(true);
// 日志清理间隔(s)
static std::atomic<int> g_nSecCleanLogFile(60);
// 日志刷新间隔
static std::atomic<int> g_nSecFlushLogFile(60);
// 日志保留最大天数
static std::atomic<int> g_nMaxLogFileKeepDays(30);
// 日志是按照天数还是个数保存 0表示最大个数 1表示最大天数
static std::atomic<int> g_nCleanLogMode(1);
// 自定义回调函数
static logCallback g_logCallback(nullptr);
static void logFailurWriter(const char *szData, int nSize) {
    google::LogMessage("FATAL", 0, google::GLOG_ERROR);
    google::FlushLogFiles(google::GLOG_INFO);
}

static void setLogDest(int nSeverity, const std::string &fileName)
{
    google::SetLogDestination(nSeverity,
                              (fileName + "." + google::GetLogSeverityName(nSeverity) + ".").c_str());
}

/**
 * Description : 查找日志文件
 * Input       : logAppName 程序名
 *               logSeverityName 级别名称集合
 * Return      : 级别名称与文件名的映射
 */
 static std::map<std::string, std::vector<std::string>>
        getFileWithPrefix(
            const std::string &appName,
            const std::vector<std::string> &severityName,
            const std::string &logPath = ""
        )
{
    std::map<std::string, std::vector<std::string>> mapFileNames;
    DIR *dir;
    struct dirent *ptr;

    std::string findDir = FLAGS_log_dir;
    if (!logPath.empty())
    {
        findDir = logPath;
    }

    if ((dir = opendir(findDir.c_str())) != nullptr)
    {
        std::vector<std::string> excludeDir;

        while ((ptr = readdir(dir)) != nullptr)
        {
            if (0 != strcmp(ptr->d_name, ".") &&
                0 != strcmp(ptr->d_name, ".."))
            {
                std::string fileName = ptr->d_name;
                auto it = find(excludeDir.begin(), excludeDir.end(), fileName);
                if (fileName.find(appName) == 0u && it == excludeDir.end())
                {
                    for (auto &severiry : severityName) {
                        if (fileName != appName + "." + severiry)
                        {
                            if (fileName.find(severiry, appName.length() + 1) == appName.length() + 1)
                            {
                                mapFileNames[severiry].push_back(fileName);
                            }
                        }
                        else
                        {
                            char nameBuf[1024];
                            ssize_t len = readlink((findDir + fileName).c_str(), nameBuf, sizeof nameBuf);
                            if (len != -1)
                            {
                                nameBuf[len] = '\0';
                                excludeDir.push_back(nameBuf);

                                auto &serFiles = mapFileNames[severiry];
                                auto it = std::find(serFiles.begin(), serFiles.end(), nameBuf);
                                if (it != serFiles.end())
                                {
                                    serFiles.erase(it);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    closedir(dir);

    for (auto &eachNameVec : mapFileNames) {
        std::sort(std::begin(eachNameVec.second), std::end(eachNameVec.second));
    }

    return mapFileNames;
 }

static void cleanLogFile(const std::string &logAppName, const std::vector<std::string> &logSeverityName)
{
    const auto &mapFiles = getFileWithPrefix(logAppName, logSeverityName);
    for (auto &vecPairFiles : mapFiles)
    {

        auto &vecFiles = vecPairFiles.second;
        if (vecFiles.empty())
        {
            continue;
        }

        auto remFunc = [](const std::string &fileName){
            int ret = std::remove((FLAGS_log_dir + fileName).c_str());
            printf("remove %s: %s.\n", fileName.c_str(), ret == 0 ? "success" : "fail");
            if (0 != ret)
            {
                perror("remove");
            }
        };

        if (google::GetLogSeverityName(google::GLOG_WARNING) == vecPairFiles.first ||
            google::GetLogSeverityName(google::GLOG_ERROR) == vecPairFiles.first)
        {
            // 删除WARING、ERROR
            std::for_each(std::begin(vecFiles),
                          std::end(vecFiles),
                          remFunc);
        }
        else
        {
            // 改成保留多少天的
            size_t removeCount = 0;
            int size = vecFiles.size();
            if (size > g_nMaxLogFileCount)
            {
                removeCount = size - g_nMaxLogFileCount;
                std::for_each(std::begin(vecFiles),
                              std::begin(vecFiles) + removeCount,
                              remFunc);
            }

            // 压缩日志文件
            std::for_each(std::begin(vecFiles) + removeCount, std::end(vecFiles),
                          [](const std::string& fileName){
                              auto len = fileName.size();
                              if (len > 3 && fileName.substr(len - 3, 3) != ".gz")
                              {
                                  auto execStr("gzip " + FLAGS_log_dir + fileName);
                                  printf("execute: %s\n", execStr.c_str());
                                  system(execStr.c_str());
                                  // 将压缩包移动到文件夹中
                                  auto execStr2("mv " + FLAGS_log_dir + fileName + ".gz " + FLAGS_log_dir + "log_history/");
                                  printf("execute: %s\n", execStr2.c_str());
                                  system(execStr2.c_str());
                              }
                          });
        }
    }
}

static void cleanHxLogFile(const std::string &logAppName, const std::vector<std::string> &logSeverityName)
{
    auto logPath = FLAGS_log_dir + "log_history/";
    const auto &mapFiles = getFileWithPrefix(logAppName, logSeverityName, logPath);
    printf("CleanHxLogFile\n");
    for (auto &vecPairFiles : mapFiles) {
        auto &vecFiles = vecPairFiles.second;

        if (vecFiles.empty())
        {
            continue;
        }

        auto remFunc = [=](const std::string& fileName){
            int ret = std::remove((logPath + fileName).c_str());
            printf("remove %s%s: %s.\n", logPath.c_str(), fileName.c_str(), ret == 0 ? "success" : "fail");

            if (0 != ret)
            {
                perror("remove");
            }
        };

        if (google::GetLogSeverityName(google::GLOG_WARNING) == vecPairFiles.first ||
            google::GetLogSeverityName(google::GLOG_ERROR) == vecPairFiles.first)
        {
            // 删除WARNING、ERROR
            std::for_each(std::begin(vecFiles),
                          std::end(vecFiles),
                          remFunc);
        }
        else
        {
            size_t removeCount = 0;
            if (g_nCleanLogMode)
            {
                for (auto &file : vecFiles) {
                    struct stat buf;
                    int result = -1;

                    // 获取文件状态信息
                    std::string path = logPath + file;
                    result = stat(path.c_str(), &buf);
                    if (0 == result)
                    {
                        time_t t = time(nullptr);
                        time_t c_time = buf.st_ctime;
                        if (t - c_time >= g_nMaxLogFileKeepDays * 24 * 3600)
                        {
                            removeCount++;
                        }
                    }
                }

            }
            else
            {
                if (vecFiles.size() > (size_t)g_nMaxHxLogFileCount)
                {
                    // 删除较老的文件
                    removeCount = vecFiles.size() - g_nMaxHxLogFileCount;
                }
            }

            std::for_each(std::begin(vecFiles),
                          std::end(vecFiles),
                          remFunc);
        }
    }
}

namespace PE {
    namespace LOG {


        /**
         * Description : 初始化日志
         * Input       : szAppName 文件名
         *               szLogDir  日志路径
         */
        void logInit(const char *szAppName, const char *szLogDir)
        {
            system(("mkdir -p " + std::string(szLogDir)).c_str());

            system(("mkdir -p " + std::string(szLogDir) + "/log_history").c_str());
            printf("exec cmd: %s\n", ("mkdir -p " + std::string(szLogDir) + "log_history").c_str());

            google::InitGoogleLogging(szAppName);
            FLAGS_log_dir = szLogDir;
            FLAGS_max_log_size = 10;

            std::string appName = szAppName;

            FLAGS_log_dir = (FLAGS_log_dir.back() == '/') ? FLAGS_log_dir : FLAGS_log_dir + '/';
            auto pos = appName.find_last_of('/');
            if (pos != std::string::npos)
            {
                ++pos;
                appName = appName.substr(pos, appName.length() - pos);
            }
            setLogDest(google::GLOG_INFO, FLAGS_log_dir + appName);
            setLogDest(google::GLOG_WARNING, FLAGS_log_dir + appName);
            setLogDest(google::GLOG_ERROR, FLAGS_log_dir + appName);

            // 段错误详细信息记录
            google::InstallFailureSignalHandler();
            google::InstallFailureWriter(&logFailurWriter);

            // 日志清理县城， 变量复制传入，主线程不必等待清理线程
            std::vector<std::string> vecSeverity {
                    google::GetLogSeverityName(google::GLOG_INFO),
                    google::GetLogSeverityName(google::GLOG_WARNING),
                    google::GetLogSeverityName(google::GLOG_ERROR)

            };
            std::thread([appName, vecSeverity](){
                const auto interval = std::chrono::seconds();
                size_t times = 0;
                for (;;++times) {
                    if (g_bAutoClean && times != 0 && times % g_nSecCleanLogFile == 0)
                    {
                        cleanLogFile(appName, vecSeverity);
                        cleanHxLogFile(appName, vecSeverity);
                    }
                    if (times % g_nSecFlushLogFile == 0)
                    {
                        google::FlushLogFiles(google::GLOG_INFO);
                    }
                    std::this_thread::sleep_for(interval);
                }

            }).detach();
        }

        /**
         * Description :  glog终止
         */
        void LogShutDown()
        {
            google::ShutdownGoogleLogging();
        }

        /**
         * Description : 设置日志级别
         * Input       : nLogLevel 级别
         */
        void setLogLevel(int nLogLevel)
        {
            int nPrevLevel = g_nLogLevel;
            if (nLogLevel < 0)
            {
                g_nLogLevel = 0;
            }
            else if (nLogLevel > 3)
            {
                g_nLogLevel = 3;
            }
            else
            {
                g_nLogLevel = nLogLevel;
            }
        }

        /**
         * Description : 获取日志级别
         */
        int getLogLevel()
        {
            PE_LOG_INFO("query glog level, curr-level: %d", static_cast<int>(g_nLogLevel));
            return g_nLogLevel;
        }

        /**
         * Description : 打印模块设置
         * Input       : nModule 模块
         *               {
         *                   0 : 除模块1外都不打印
         *                   1 : 打印所有模块
         *                   其他 : 打印指定模块
         *               }
         */
        void setLogModule(int nModule)
        {
            int nPrevModule = g_nPrintModule;
            if (nModule < 0)
            {
                g_nPrintModule = PE_DEFAULT_MODULE;
            }
            else
            {
                g_nPrintModule = nModule;
            }
        }

        /**
         * Description : 获取打印模块设置
         */
        int getLogModule()
        {
            PE_LOG_INFO("query glog  module, module: %d", static_cast<int>(g_nPrintModule));
            return g_nPrintModule;
        }

        /**
         * Description : 日志文件最大个数
         * Input       : 个数
         */
        void setMaxLogFileCount(int nFileCount)
        {
            if (nFileCount >= 1)
            {
                g_nMaxLogFileCount = nFileCount;
            }
        }

        /**
         * Description : 历史日志文件最大个数
         * Input       : 个数
         */
        void setMaxHxLogFileCount(int nFileCount)
        {
            if (nFileCount >= 1)
            {
                g_nMaxHxLogFileCount = nFileCount;
            }
        }

        /**
         * Description : 历史日志文件保存最大天数
         * Input       : 天数
         */
        void setMaxLogFileKeepDays(int nFileDays)
        {
            if (nFileDays >= 1)
            {
                g_nMaxLogFileKeepDays = nFileDays;
            }
        }

        /**
         * Description : 历史日志按照日期保存还是按照最大个数保存
         * Input       : 模式
         */
        void setLogCleanLogMode(int mode)
        {
            if (0 == mode || 1 == mode)
            {
                g_nCleanLogMode = mode;
            }
        }

        /**
         * Description : 日志内容输出定义回调
         */
        void setLogCallback(logCallback func)
        {
            g_logCallback = func;
        }

        /**
         * Description : 日志刷新时间
         * Input       : 秒(最新1秒)
         */
        void setLogFileFlushInterval(int nSeconds)
        {
            int secFlush = g_nSecFlushLogFile;
            if (nSeconds < 1)
            {
                nSeconds = 1;
            }
            g_nSecFlushLogFile = nSeconds;
            PE_LOG_INFO("change flush interval, prev-interval: %d, curr-interval: %d", secFlush, static_cast<int>(g_nSecFlushLogFile));
        }
        /**
         * Description : 日志文件最大容量
         * Input       : nMegaByte兆字节(最高1GB， 最低1MB)
         */
        void setMaxLogFileSize(int nMegeByte)
        {
            if (nMegeByte > 1024)
            {
                nMegeByte = 1024;
            }
            else if (nMegeByte < 1)
            {
                nMegeByte = 1;
            }
            FLAGS_max_log_size = nMegeByte;
        }
        /**
         * Description : 自动清理日志文件(默认自动清理)
         * Input       : 是否自动清理
         */
        void setAutoCleanLogFile(bool  bAutoClean)
        {
            g_bAutoClean = bAutoClean;
        }
        /**
         * Description : 格式化日志
         * Input       : szFileName 源文件名
         *               szFunctionName 函数名
         *               nLine 行
         *               nModule 模块{<=1:普通模块}
         *               nSeverity 日志级别
         *               szFmt 字符串格式
         * Return      : 字符串长度
         */
        int logFormat(const char *szCppFileName,
                      const char *szFunctionName,
                      int nLine,
                      int nModule,
                      int nSeverity,
                      const char *szFmt,
                      ...) {
            int ret = 0;

            if (nSeverity > g_nLogLevel) {
                return ret;
            }

            // 增加DEBUG级别为0
            if (nSeverity > 0) {
                --nSeverity;
            }

            // 记录条件：
            // 1、nModule >= PE_DEFAULT_MODULE;
            // 2、PE_PRINT_ALL_MODULE == g_nPrintModule;
            // 3、nModule == g_nPrintModule
            if (PE_DEFAULT_MODULE > nModule &&
                PE_PRINT_ALL_MODULE != g_nPrintModule &&
                nModule != g_nPrintModule)
            {
                return ret;
            }

            va_list arg;
            va_start(arg, szFmt);
            int bufSize = 1024;
            std::vector<char> buf(bufSize);
            ret = vsnprintf(&buf[0], buf.size(), szFmt, arg);
            if (ret < 0)
            {
                va_end(arg);
                return ret;
            }

            // 缓存不够时，重新申请缓存
            if (ret > bufSize)
            {
                va_end(arg);
                bufSize = std::min(static_cast<size_t>(ret) + 1, google::LogMessage::kMaxLogMessageLen);
                buf.resize(bufSize);
                va_start(arg, szFmt);
                ret = vsnprintf(&buf[0], buf.size(), szFmt, arg);
            }
            va_end(arg);

            if (ret == static_cast<int>(buf.size()))
            {
                buf.back() = '\0';
            }

            if (PE_DEFAULT_MODULE == nModule)
            {
                google::LogMessage(szCppFileName, nLine, nSeverity).stream()
                        << '[' << szFunctionName << "] " << &buf[0];
            }
            else
            {
                google::LogMessage(szCppFileName, nLine, nSeverity).stream()
                        << '[' << szFunctionName << "][" << nModule << "] " << &buf[0];
            }

            std::string cppFileName(szCppFileName);
            std::string::size_type pos = cppFileName.find_last_of('/');
            if (pos != std::string::npos)
            {
                ++pos;
                cppFileName = cppFileName.substr(pos, cppFileName.length() - pos);
            }

            if (g_logCallback)
            {
                g_logCallback(cppFileName.c_str(), szFunctionName, nLine, buf.data(), buf.size());
            }

            return ret;
        }
    }// namespace LOG
}// namespace PE