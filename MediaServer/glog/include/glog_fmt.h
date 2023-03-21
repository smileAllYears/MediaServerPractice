//
// Created by smileForCode on 2023/1/16.
//

#ifndef PEMEDIASERVER_GLOG_FMT_H
#define PEMEDIASERVER_GLOG_FMT_H
#include <cstdlib>

#define PE_DEFAULT_MODULE 255 //默认模块编号
#define PE_PRINT_ALL_MODULE 256 // 打印所有模块 (setLogModule使用)

// 日志记录级别
#define PE_LOG_LEVEL_DEBUG 0
#define PE_LOG_LEVEL_INFO  1
#define PE_LOG_LEVEL_WARN  2
#define PE_LOG_LEVEL_ERRO  3


#ifdef __cplusplus
extern "C"
{
#endif
    typedef void (*logCallback)(const char *fileName,
                                const char *functionName,
                                int line,
                                const char *data,
                                int dataSize);
    namespace ZLW {
        namespace LOG {
            /**
             * Description : 初始化日志
             * Input       : szAppName 文件名
             *               szLogDir  日志路径
             */
            void logInit(const char *szAppName, const char *szLogDir);

            /**
             * Description :  glog终止
             */
            void LogShutDown();

            /**
             * Description : 设置日志级别
             * Input       : nLogLevel 级别
             */
            void setLogLevel(int nLogLevel);

            /**
             * Description : 获取日志级别
             */
            int getLogLevel();

            /**
             * Description : 打印模块设置
             * Input       : nModule 模块
             *               {
             *                   0 : 除模块1外都不打印
             *                   1 : 打印所有模块
             *                   其他 : 打印指定模块
             *               }
             */
            void setLogModule(int nModule);

            /**
             * Description : 获取打印模块设置
             */
            int getLogModule();

            /**
             * Description : 日志文件最大个数
             * Input       : 个数
             */
            void setMaxLogFileCount(int nFileCount);

            /**
             * Description : 历史日志文件最大个数
             * Input       : 个数
             */
            void setMaxHxLogFileCount(int nFileCount);

            /**
             * Description : 历史日志文件保存最大天数
             * Input       : 天数
             */
            void setMaxLogFileKeepDays(int nFileDays);


            /**
             * Description : 历史日志按照日期保存还是按照最大个数保存
             * Input       : 模式
             */
            void setLogCleanLogMode(int mode);

            /**
             * Description : 日志内容输出定义回调
             */
            void setLogCallback(logCallback func);

            /**
             * Description : 日志刷新时间
             * Input       : 秒(最新1秒)
             */
            void setLogFileFlushInterval(int nSeconds);

            /**
             * Description : 日志文件最大容量
             * Input       : nMegaByte兆字节(最高1GB， 最低1MB)
             */
            void setMaxLogFileSize(int nMegeByte);

            /**
             * Description : 自动清理日志文件(默认自动清理)
             * Input       : 是否自动清理
             */
            void setAutoCleanLogFile(bool  bAutoClean);

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
            int logFormat(const char *szCopFileName, const char *szFunctionName,
                          int nLine, int nModule, int nSeverity, const char *szFmt, ...);


        }// namespace LOG
    }    // namespace ZLW
#ifdef __cplusplus
}
#endif

// 记录日志级别、模块、格式
#define PE_LOG_SEVERITY_MODULE_FMT(severity,module,...) logFormat(__FILE__, __FUNCTION__, __LINE__, module, severity, __VA_ARGS__)

// DEBUG日志
#define PE_LOG_DEBUG(...) PE_LOG_SEVERITY_MODULE_FMT(0, PE_DEFAULT_MODULE,__VA_ARGS__)
// INFO日志
#define PE_LOG_INFO(...) PE_LOG_SEVERITY_MODULE_FMT(1, PE_DEFAULT_MODULE,__VA_ARGS__)
// WARNING日志
#define PE_LOG_WARN(...) PE_LOG_SEVERITY_MODULE_FMT(2, PE_DEFAULT_MODULE,__VA_ARGS__)
// ERROR日志
#define PE_LOG_ERRO(...) PE_LOG_SEVERITY_MODULE_FMT(3, PE_DEFAULT_MODULE,__VA_ARGS__)

// 当setLogModule设置为与module相同时才记录
#define MV_LOG_DEBUG_MDL(module,...) MV_LOG_SEVERITY_MODULE_FMT(0,module,__VA_ARGS__)
#define MV_LOG_INFO_MDL(module,...) MV_LOG_SEVERITY_MODULE_FMT(1,module,__VA_ARGS__)
#define MV_LOG_WARN_MDL(module,...) MV_LOG_SEVERITY_MODULE_FMT(2,module,__VA_ARGS__)
#define MV_LOG_ERRO_MDL(module,...) MV_LOG_SEVERITY_MODULE_FMT(3,module,__VA_ARGS__)

#endif//PEMEDIASERVER_GLOG_FMT_H
