/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-03-21 14:30:43
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 14:44:35
 * @FilePath: /MediaServerPractice/MediaServer/Common/include/ZLWSingleton.h
 * @Description: 通用单例模版
 */
#ifndef ZLWMEDIASERVER_ZLWSINGLETON_H
#define ZLWMEDIASERVER_ZLWSINGLETON_H

#include <mutex>

namespace ZLW
{
    namespace COMMON
    {
        template<class T>
        class ZLWSingleton
        {
        protected:
            ZLWSingleton() {}
            ZLWSingleton(const ZLWSingleton&) {}
            ZLWSingleton& operator= (const ZLWSingleton&) {}  
        public:
            virtual ~ZLWSingleton() {}
            static T* getInstance();
        protected:
            static T* m_pInstance;
            static std::mutex m_mtx;
        };

        template<class T>
        T* ZLWSingleton<T>::m_pInstance = nullptr;
        
        template<class T>
        std::mutex ZLWSingleton<T>::m_mtx;
    }
}

#endif