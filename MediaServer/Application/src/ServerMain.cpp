/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-01-14 16:55:55
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 11:16:15
 * @FilePath: /MediaServerPractice/MediaServer/Application/src/ServerMain.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//
// Created by smileForCode on 2023/1/14.
//
#include "Application.h"

int main(int argc, char** argv)
{
    ZLW::APP::CApplication app("PEMediaServer");
    app.run("..");
    return 0;
}