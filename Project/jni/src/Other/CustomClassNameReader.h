#pragma once
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>

class CustomClassNameReader
{
public:
    explicit CustomClassNameReader(const std::string& filename) { LoadFromFile(filename); }

    // 查询成功返回 true，并把动态分配的显示名和颜色带出来
    bool GetCustomClassName(const char* gname,
                            char**      outName,
                            ImVec4*     outColor) const
    {
        auto it = table_.find(gname);
        if (it == table_.end()) return false;

        *outName = strdup(it->second.name.c_str());
        *outColor = it->second.color;
        return true;
    }

private:
    struct Entry
    {
        std::string name;
        ImVec4      color;
    };
    std::unordered_map<std::string, Entry> table_;

    void LoadFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file)
        {
          //  std::cerr << "[CustomClassNameReader] Cannot open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty() || line[0] == '#') continue;   // 空行或注释

            std::istringstream iss(line);
            std::string className, displayName, colorStr;

            if (!std::getline(iss, className, '|'))   continue;
            if (!std::getline(iss, displayName, '|')) continue;
            std::getline(iss, colorStr);                      // 可选

            Entry ent;
            ent.name = std::move(displayName);
            ent.color = ParseColorString(colorStr);           // 失败会返回白色
            table_[className] = std::move(ent);
        }
    }

    // 把 "R,G,B,A" 转成 ImVec4（0-1），失败返回白色
    static ImVec4 ParseColorString(const std::string& s)
    {
        if (s.empty()) return ImVec4(1, 1, 1, 1);

        int r = 255, g = 255, b = 255, a = 255;
        char dummy;
        std::istringstream ss(s);
        ss >> r >> dummy >> g >> dummy >> b >> dummy >> a;
        if (ss.fail()) return ImVec4(1, 1, 1, 1);

        return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }
};