/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-01-16 10:38:20
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 11:15:45
 * @FilePath: /MediaServerPractice/MediaServer/Application/include/Application.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by 张新军 on 2023/1/16.
//

#ifndef PEMEDIASERVER_APPLICATION_H
#define PEMEDIASERVER_APPLICATION_H
#include <string>
namespace ZLW
{
    namespace APP
    {
        class CApplication
        {
        public:
            explicit CApplication(std::string strAppName);
            virtual ~CApplication() = default;

            void run(const char* basedir);
        private:
            std::string m_strAppName;
        };
    }
}

#endif//PEMEDIASERVER_APPLICATION_H
