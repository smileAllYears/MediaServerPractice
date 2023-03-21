/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-01-16 17:18:39
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 11:15:43
 * @FilePath: /MediaServerPractice/MediaServer/Application/include/AppVersion.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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

namespace ZLW
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
