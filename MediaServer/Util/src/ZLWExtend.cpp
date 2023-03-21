/*
 * @Author: zhangxinjun zhangxinjun@minivision.cn
 * @Date: 2023-03-21 13:54:09
 * @LastEditors: zhangxinjun zhangxinjun@minivision.cn
 * @LastEditTime: 2023-03-21 17:29:59
 * @FilePath: /MediaServerPractice/MediaServer/Util/src/ZLWExtend.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ZLWExtend.h"

namespace ZLW
{
    namespace EXTEND
    {
        template<typename key, typename variant>
        std::vector<std::string> CExtendMap<key, variant>::tokenize(const std::string& self, const std::string& chars) const
        {
            std::vector<std::string> tokens(1);
            std::string strMap(256, '\0');
            for (char ch : chars)
            {
                strMap[(uint8_t) ch] = '\1';
            }

            for (char ch : self)
            {
                if (!strMap.at((uint8_t) ch))
                {
                    tokens.back().push_back(ch);   
                }
                else if (tokens.back().size())
                {
                    tokens.push_back(std::string());   
                }
            }

            while (tokens.size() && tokens.back().empty())
            {
                tokens.pop_back();
            }
            return tokens;
        }

        template<typename key, typename variant>
        void CExtendMap<key, variant>::parse(const std::string& parseCon)
        {
            std::vector<std::string> lines = tokenize(parseCon, "\n");
            std::string symbol, tag;
            for (auto &line : lines)
            {
                line = UTIL::trim(line);
                if (line.empty() || line.front() == ';' || line.front() == '#')
                {
                    continue;
                }

                if (line.size() >= 3 && line.front() == '[' && line.back() == ']')
                {
                    tag = UTIL::trim(line.substr(1, line.size() - 2));
                }
                else
                {
                    auto atStr = line.find('=');
                    symbol = UTIL::trim(tag + "." + line.substr(0, atStr));
                    (*this)[symbol] = (atStr == std::string::npos ? std::string() : UTIL::trim(line.substr(atStr + 1)));
                }                
            }
        }

        template<typename key, typename variant>
        void CExtendMap<key, variant>::parseFile(const std::string& fileName)
        {
            std::ifstream inStream(fileName, std::ios::in | std::ios::binary | std::ios::ate);
            if (!inStream.good())
            {
                throw std::invalid_argument("Invalid ini file: " + fileName);
            }
            
            auto size = inStream.tellg();
            
        }
    }
}