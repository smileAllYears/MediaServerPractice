//
// Created by 张新军 on 2023/1/16.
//

#ifndef PEMEDIASERVER_APPLICATION_H
#define PEMEDIASERVER_APPLICATION_H
#include <string>
namespace PE
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
