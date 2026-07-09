#ifndef ZEUS_KERNEL_NATIVE_SURFACE_H
#define ZEUS_KERNEL_NATIVE_SURFACE_H

// User libs
#include "Draw.h"
#include "TouchHelperA.h"
#include <tool.h>
#include <UnitInt.h>
#include <array>
// Func
//momo//
MyTextureData 悬浮球;
MyTextureData 背景;
MyTextureData 悬浮自瞄开关;
MyTextureData 没人图;
MyTextureData 真人图;
MyTextureData 人机图;
MyTextureData ID101005;
MyTextureData ID101001;
MyTextureData ID101002;
MyTextureData ID101003;
MyTextureData ID101004;
MyTextureData ID101006;
MyTextureData ID101007;
MyTextureData ID101008;
MyTextureData ID101009;
MyTextureData ID101010;
MyTextureData ID101011;
MyTextureData ID101012;

MyTextureData ID102001;
MyTextureData ID102002;
MyTextureData ID102003;
MyTextureData ID102004;
MyTextureData ID102005;
MyTextureData ID102007;
MyTextureData ID102105;

MyTextureData ID103005;
MyTextureData ID103001;
MyTextureData ID103002;
MyTextureData ID103003;
MyTextureData ID103004;
MyTextureData ID103006;
MyTextureData ID103007;
MyTextureData ID103008;
MyTextureData ID103009;
MyTextureData ID103010;
MyTextureData ID103011;
MyTextureData ID103012;
MyTextureData ID103013;
MyTextureData ID103014;
MyTextureData ID103015;

MyTextureData ID103100;
MyTextureData ID103901;
MyTextureData ID103902;
MyTextureData ID103903;

MyTextureData ID104001;
MyTextureData ID104002;
MyTextureData ID104003;
MyTextureData ID104004;

MyTextureData ID104100;

MyTextureData ID105001;
MyTextureData ID105002;
MyTextureData ID105010;

MyTextureData ID106001;
MyTextureData ID106002;
MyTextureData ID106003;
MyTextureData ID106004;
MyTextureData ID106005;
MyTextureData ID106006;
MyTextureData ID106007;
MyTextureData ID106008;
MyTextureData ID106010;

MyTextureData ID107001;
MyTextureData ID107006;
//拳头
MyTextureData ID0;

//投掷物

MyTextureData ID602004;
MyTextureData ID602003;
MyTextureData ID602002;
MyTextureData ID602001;
bool IsDown = true, IsDown1 = true;
bool IsBall = true;
bool voice = true;
static float ANIM_SPEED = 0.5f;
static float Velua = IsBall ? 0.0f : 1.0f;
ImVec2 ImagePos;
int 自身动作;
bool BallSwitch = true, MemuSwitch = true;
bool IsWin = true;
ImGuiWindow *g_window = nullptr;
ImGuiIO *Io = nullptr;
ImGuiIO & io = ImGui::GetIO();
//momo//
int style_idx2 = 0, FPS, style_idx3 = 0;
FILE *numSave = nullptr;
float NumIo[300], win宽=1050, win高=700;
int NumIos[300];
float fwjl = NumIo[3];
long int CurrentVehicle;
bool DrawIo[300];
bool 初始化 = true;
int intIo[10];
struct
{
bool 侧边1=true;
    bool 侧边2;
    bool 侧边3;
    bool 侧边4;
    bool 侧边5;
    bool 侧边6;
    int 主题 =0;
}
界面;
void win1();
void win2();
string 提示 = "No action selected";
namespace ImAnim {
    // 三级贝塞尔曲线计算
    float Bezier(float t, float p0, float p1, float p2, float p3) {
        float u = 1.0f - t;
        float uu = u * u;
        float uuu = uu * u;
        float tt = t * t;
        float ttt = tt * t;
        
        return uuu * p0 + 3 * uu * t * p1 + 3 * u * tt * p2 + ttt * p3;
    }

    // 预定义的缓入缓出曲线控制点
    const ImVec2 easeInControl1 = ImVec2(0.42f, 0.0f);
    const ImVec2 easeInControl2 = ImVec2(1.0f, 1.0f);
    const ImVec2 easeOutControl1 = ImVec2(0.0f, 0.0f);
    const ImVec2 easeOutControl2 = ImVec2(0.58f, 1.0f);
}

#include <unordered_map>

std::unordered_map<int, float> g_WeaponPressureDB;  // 战力
std::unordered_map<int, float> g_WeaponPressureDX;  // 蹲下压枪
std::unordered_map<int, float> g_WeaponPressurePX;  // 趴下压枪

// 添加的结构体用于统一管理
struct WeaponPressureConfig {
    float db;
    float dx;
    float px;
};
std::unordered_map<int, WeaponPressureConfig> g_WeaponConfigs;

void SaveWeaponConfig() {
    std::ofstream configFile("PUBG_weapon_config.cfg");
    if (configFile.is_open()) {
        // 以战力map为主，三个map key相同
        for (const auto& [weaponID, pressure] : g_WeaponPressureDB) {
            configFile << weaponID << " "
                       << pressure << " "
                       << g_WeaponPressureDX[weaponID] << " "
                       << g_WeaponPressurePX[weaponID] << "\n";
        }
        configFile.close();
    }
}

void LoadWeaponConfig() {
    std::ifstream configFile("PUBG_weapon_config.cfg");
    if (configFile.is_open()) {
        g_WeaponPressureDB.clear();
        g_WeaponPressureDX.clear();
        g_WeaponPressurePX.clear();
        g_WeaponConfigs.clear();

        int weaponID;
        float db, dx, px;
        while (configFile >> weaponID >> db >> dx >> px) {
            g_WeaponPressureDB[weaponID] = db;
            g_WeaponPressureDX[weaponID] = dx;
            g_WeaponPressurePX[weaponID] = px;
            g_WeaponConfigs[weaponID] = {db, dx, px};
        }
        configFile.close();
    }
}

void RenderWeaponSlider() {
    static bool isInitialized = false;
    if (!isInitialized) {
        LoadWeaponConfig();
        isInitialized = true;
    }

    if (g_WeaponPressureDB.find(MyWeapon) == g_WeaponPressureDB.end()) {
        g_WeaponPressureDB[MyWeapon] = 1.0f;
        g_WeaponPressureDX[MyWeapon] = 1.0f;
        g_WeaponPressurePX[MyWeapon] = 1.0f;
        g_WeaponConfigs[MyWeapon] = {1.0f, 1.0f, 1.0f};
    }

    // 原有的UI渲染代码...
}

// Func->
// 布局UI<


//Var->

// GUI_是否继续

#endif//ZEUS_KERNEL_NATIVE_SURFACE_H