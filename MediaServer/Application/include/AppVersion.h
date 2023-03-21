//
// Created by 张新军 on 2023/1/16.
//

#ifndef PEMEDIASERVER_APPVERSION_H
#define PEMEDIASERVER_APPVERSION_H

#define VER_MAIN 1  //主版本
#define VER_SUB_ADD //功能添加
#define VER_SUB_MODIFY //bug修改

#define STR(s) #s
#define VERSION(a, b, c) "Version " STR(a) "." STR(b) "." STR(C) " " __DATE__

namespace PE
{
    namespace VER
    {
        // 应用描述
        const char *getProgramDesc();

        // 版本号
        const char *getVersion();
    }
}

#endif//PEMEDIASERVER_APPVERSION_H
