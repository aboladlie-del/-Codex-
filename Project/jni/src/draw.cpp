#include "main.h"
#include "offsets.h"
#include <fstream>
#include <unordered_set>
#include "ImGui/font2.h"
#include "aimbot.cpp"
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "PCUI.h"
#include "Other/Auth.h"
extern ImFont* Font2;  // 临时声明
bool IsDev = false;
// Add this line near the top of draw.cpp
int PTarget = -1;
bool AntiRecord;
bool Notice = true; 
static float g_timeout = 300.0f;   //Timeout
static float countdown_timer = 0.0f;
static bool countdown_active = false;

int Decrypt = 0;
int Quantity;
pid_t get_name_pid(char* name) {
    FILE* fp;
    pid_t pid = -1;
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "pidof %s", name);
    fp = popen(cmd,"r");
    if (fp) {
        if (fscanf(fp,"%d", &pid) != 1) {
            pid = -1;
        }
        pclose(fp);
    }
    return pid;
}
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
int DrawInt()
{
char module_name[0x100] = "libUE4.so";
const char* game_package_names[] = {"com.tencent.ig", "com.rekoo.pubgm", "com.pubg.krmobile","com.pubg.imobile"};
        size_t game_count = sizeof(game_package_names) / sizeof(game_package_names[0]);
        for(size_t i = 0; i < game_count; i++) {
           pid = get_name_pid((char*)game_package_names[i]);
            if (pid > 0 && pid < 100000) {
                driver->initialize(pid); // Initialize Driver
                break;
            }
        }
        if (pid <= 0) {
            return 0;
        }
   libUE4 = driver->getModuleBase(module_name); // Get Module Base
   return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_A 1103515245   // 例如用于随机数生成
#define RAND_C 12345
#define M 0xFFFFFFFF

unsigned int lcg_next() {
    static unsigned int seed = 0;
    seed = (RAND_A * seed + RAND_C) % M;
    return seed;
}

float lcg_random_float(unsigned int *seed) {
    return (float)(*seed = lcg_next()) / (float)M;
}
    void DrawBackgroundImage(ImTextureID texture, float rounding = 0.0f)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 pos = window->Pos;
    ImVec2 size = window->Size;

    if (rounding > 0.0f)
    {
        // 绘制带圆角的背景图
        draw_list->AddImageRounded(
            texture,
            pos, { pos.x + size.x, pos.y + size.y }, // 图片覆盖整个窗口
            ImVec2(0, 0), ImVec2(1, 1), // UV 坐标（全图）
            IM_COL32_WHITE, // 颜色（不改变原图）
            rounding // 圆角半径
        );
    }
    else
    {
        // 绘制普通背景图
        draw_list->AddImage(
            texture,
            pos, { pos.x + size.x, pos.y + size.y },
            ImVec2(0, 0), ImVec2(1, 1),
            IM_COL32_WHITE
        );
    }
}
void DrawCircularImage(ImVec2 center, float radius, ImTextureID icon, ImU32 color, bool isCircular = true)
{
    ImGui::SetCursorPos({ 0, 180 });
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // 可以三
    ImVec2 image_min = { center.x - radius, center.y - radius };
    ImVec2 image_max = { center.x + radius, center.y + radius };

    if (isCircular)
    {
        // 使用 AddImageRounded 方法绘制圆形图片
        draw_list->AddImageRounded(icon, image_min, image_max, ImVec2(0, 0), ImVec2(1, 1), color, radius, ImDrawFlags_RoundCornersAll);
    }
    else
    {
        // 直接绘制普通矩形图片
        draw_list->AddImage(icon, image_min, image_max, ImVec2(0, 0), ImVec2(1, 1), color);
    }
}
ImVec4 teamColors[] = {
ImColor(255 * 0.7, 255 * 0.7, 0, 255), // 深黄色
ImColor(255 * 0.7, 0, 255 * 0.7, 255), // 深紫色
ImColor(255 * 0.7, 0, 0, 255),         // 深红色
ImColor(0, 0, 255 * 0.7, 255),         // 深蓝色
ImColor(255 * 0.7, 165 * 0.7, 0, 255), // 深橙色
ImColor(165 * 0.7, 42 * 0.7, 42 * 0.7, 255), // 深棕红色
ImColor(173 * 0.7, 216 * 0.7, 230 * 0.7, 255), // 深天蓝色
ImColor(255 * 0.7, 215 * 0.7, 0, 255), // 深金色
ImColor(135 * 0.7, 206 * 0.7, 235 * 0.7, 255), // 深浅天蓝色
ImColor(75 * 0.7, 0, 130 * 0.7, 255), // 深深紫色
ImColor(255 * 0.7, 127 * 0.7, 80 * 0.7, 255), // 深珊瑚色
ImColor(0, 191 * 0.7, 255 * 0.7, 255), // 深亮蓝色
ImColor(255 * 0.7, 105 * 0.7, 180 * 0.7, 255), // 深热粉色
ImColor(210 * 0.7, 180 * 0.7, 140 * 0.7, 255), // 深沙褐色
ImColor(255 * 0.7, 192 * 0.7, 203 * 0.7, 255), // 深粉红色
ImColor(255 * 0.7, 69 * 0.7, 0, 255),         // 深番茄色
ImColor(255 * 0.7, 140 * 0.7, 0, 255),        // 深橙色
ImColor(210 * 0.7, 105 * 0.7, 30 * 0.7, 255), // 深巧克力色
ImColor(255 * 0.7, 160 * 0.7, 122 * 0.7, 255), // 深浅珊瑚色
ImColor(218 * 0.7, 112 * 0.7, 214 * 0.7, 255), // 深兰花紫
ImColor(176 * 0.7, 224 * 0.7, 230 * 0.7, 255), // 深粉蓝色
ImColor(255 * 0.7, 20 * 0.7, 147 * 0.7, 255),  // 深深洋红色
ImColor(255 * 0.7, 218 * 0.7, 185 * 0.7, 255), // 深蜜露色
ImColor(255 * 0.7, 182 * 0.7, 193 * 0.7, 255), // 深荷粉色
ImColor(0, 162 * 0.7, 232 * 0.7, 255),        // 深道奇蓝
ImColor(255 * 0.7, 193 * 0.7, 7 * 0.7, 255),   // 深芒果色
ImColor(255 * 0.7, 111 * 0.7, 207 * 0.7, 255), // 深艳粉
ImColor(238 * 0.7, 232 * 0.7, 170 * 0.7, 255), // 深卡其色
ImColor(255 * 0.7, 181 * 0.7, 167 * 0.7, 255), // 深桃色
ImColor(255 * 0.7, 69 * 0.7, 0, 255),         // 深亮橙色
ImColor(255 * 0.7, 0, 255 * 0.7, 255),        // 深洋红色
ImColor(255 * 0.7, 99 * 0.7, 71 * 0.7, 255),  // 深印度红
ImColor(0, 206 * 0.7, 209 * 0.7, 255),        // 深亮海绿
ImColor(30 * 0.7, 144 * 0.7, 255 * 0.7, 255), // 深道奇蓝
ImColor(255 * 0.7, 20 * 0.7, 147 * 0.7, 255), // 深热粉色
ImColor(0, 191 * 0.7, 255 * 0.7, 255),        // 深亮蓝色

};
int teamCount = sizeof(teamColors) / sizeof(teamColors[0]); // 队伍数量

int TeamID = driver->read<int>(Objaddr + 0x998); // 读取队伍ID
// 根据队伍ID获取颜色
ImColor GetTeamColor(int teamID) {
    return teamColors[teamID % teamCount]; // 使用队伍ID对颜色数组进行模运算，以循环使用颜色数组
}

void 绘制加粗文本(float size, float x, float y, ImColor color, ImColor color1, const char* str)
{
    ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x-0.1, y-0.1), color1, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x+0.1, y+0.1), color1, str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), color, str);
}
void DrawTextOutline(float size,int x, int y, ImVec4 color, const char* str)
{
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, size,ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
}

void DrawPlayer(ImDrawList *Draw)
{
    
    float top,right,left,bottom,x1,top1;
    ImColor 颜色1;
    px = displayInfo.width/2;
    py = displayInfo.height/2;
   
    DrawTextOutline(35, px/10, py-480, 白色, extra);
    DrawTextOutline(35, px/10, py+450, 白色, chufa666);
    float px = displayInfo.width / 2;//变量
    float py = displayInfo.height / 2;
    ImColor textColor = ImColor(255, 192, 203); // 字体颜色
    
    
                    
                    
    // 自瞄开关
    if (DrawIo[20])
    {
    
   if (DrawIo[27]&& Firing == 1 && DrawIo[31] && Gmin != -1) {
    Draw->AddCircle(
        {
            px, py
        }
        , fwjl,自瞄圈,0,0.0f);
        } else if (!DrawIo[27]) {
    Draw->AddCircle(
        {
            px, py
        }
        , NumIo[3],自瞄圈,0,0.0f);
        }
        
if(DrawIo[26]){
DrawIo[27]=false;
}else{

}
DrawIo[24]=true;
        
        
if (DrawIo[24] && Gmin != -1 && ToReticleDistance <= NumIo[3]&&!DrawIo[26])
{

        Draw->AddLine(
            { px, py },
            { 瞄准x, 瞄准y },
            黄色,
            2.0f
        );
    }
    if(DrawIo[26]&&DrawIo[24]&& (px >= Aim[Gmin].boxLeft && px <= Aim[Gmin].boxRight && py >= Aim[Gmin].boxTop && py <= Aim[Gmin].boxBottom)){        Draw->AddLine(
            { px, py },
            { 瞄准x, 瞄准y },
            黄色,
            2.0f
        );}
        
    }else{
    DrawIo[24]=false;
    }
if (DrawIo[64] && (Aiming == 0 || Aiming == 256)) {

    Draw->AddLine({px - NumIo[46], py}, {px + NumIo[46], py}, 十字架, 3.0f); // 横线
    Draw->AddLine({px, py - NumIo[46]}, {px, py + NumIo[46]}, 十字架, 3.0f); // 竖线
}

    
    if (DrawIo[21])
    {
        Draw->AddRectFilled(
        {
            0,0
        }
        ,
        {
            px*2, py*2
        }
        ,ImColor(0,0,0,110));
        std::string ssf;
        ssf += "Long press and drag the control";
        auto textSize = ImGui::CalcTextSize(ssf.c_str(), 0, 25);
        Draw->AddRectFilled(
        {
            NumIo[6] - NumIo[7]/2, py*2 - NumIo[5] + NumIo[7]/2
        }
        ,
        {
            NumIo[6] + NumIo[7]/2, py*2 - NumIo[5] - NumIo[7]/2
        }
        ,ImColor(255,0,0,120));
        Draw->AddText(NULL,32,
        {
            NumIo[6] - (textSize.x / 2),py*2 - NumIo[5]
        }
        ,ImColor(255,255,255),ssf.c_str());
    }
    
    if (DrawIo[56]/*连点*/)
    {
        Draw->AddRectFilled(
        {
            0,0
        }
        ,
        {
            px*2, py*2
        }
        ,ImColor(0,0,0,110));
        std::string ssf;
        ssf += "Place on fire button";
        auto textSize = ImGui::CalcTextSize(ssf.c_str(), 0, 25);
        Draw->AddRectFilled(
        {
            NumIo[49] - NumIo[50]/2, py*2 - NumIo[48] + NumIo[50]/2
        }
        ,
        {
            NumIo[49] + NumIo[50]/2, py*2 - NumIo[48] - NumIo[50]/2
        }
        ,ImColor(0,0,255,120));
        Draw->AddText(NULL,32,
        {
            NumIo[49] - (textSize.x / 2),py*2 - NumIo[48]
        }
        ,ImColor(255,255,255),ssf.c_str());
    }
   
   
    uintptr_t EngineBase = driver->read<uintptr_t>(libUE4 + OFFSET_VMATRIX);
    if (Decrypt != 0){        
        Matrix = driver->read<uintptr_t>(EngineBase + 0xC0) + 0x590;
    } else {
        Matrix = driver->read<uintptr_t>(EngineBase + 0xC0) + 0x9D0;
    }      

    // PUBG 4.5.0 world/name resolving. Primary path keeps the old engine-chain logic;
    // direct global offsets are used as fallback for the new dump.
    Uworld = driver->read<uintptr_t>(driver->read<uintptr_t>(EngineBase + 0x58) + 0x78); // 游戏世界
    if (Uworld <= 0xffff || Uworld <= 0x10000000 || Uworld >= 0x10000000000) {
        Uworld = driver->read<uintptr_t>(libUE4 + OFFSET_GWORLD);
    }

    Gname = driver->read<uintptr_t>(driver->read<uintptr_t>(libUE4 + OFFSET_GNAME) + OFFSET_GNAME_PTR);
    if (Gname <= 0xffff || Gname <= 0x10000000 || Gname >= 0x10000000000) {
        Gname = driver->read<uintptr_t>(libUE4 + OFFSET_GNAMES);
    }
    Uleve = driver->read<uintptr_t>(Uworld + OFFSET_UWORLD_PERSISTENT_LEVEL);// Uleve
    Arrayaddr = driver->read<uintptr_t>(Uleve + OFFSET_LEVEL_ACTORS_ARRAY);
    countddr = driver->read<int>(Uleve + OFFSET_LEVEL_ACTORS_COUNT);// 阵列数量

    oneself  = driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(Uworld + OFFSET_UWORLD_NETDRIVER) + OFFSET_NETDRIVER_SERVERCONNECTION) + OFFSET_SERVERCONNECTION_PLAYERCONTROLLER) + OFFSET_PLAYERCONTROLLER_SELF);
    
    Myteam = driver->read<int>(oneself + OFFSET_ACTOR_TEAMID);
    fov = driver->read<float>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + OFFSET_SELF_PLAYER_CONTROLLER) + OFFSET_PLAYER_CAMERA_MANAGER) + OFFSET_CAMERA_CACHE);
    
    Firing   = driver->read<int>(oneself + OFFSET_WEAPON_B_IS_FIRING);
    Aiming   = driver->read<int>(oneself + OFFSET_WEAPON_B_IS_GUN_ADS);
    自身动作 = driver->read<int>(oneself + OFFSET_ACTOR_STATUS);//状态       
    
    MyWeapon = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + OFFSET_ACTOR_WEAPON_MANAGER) + OFFSET_WEAPON_CURRENT_REPLICATED) + OFFSET_WEAPON_SHOOT_ENTITY) + OFFSET_WEAPON_ID);
    uintptr_t GameState = driver->read<uintptr_t>(Uworld + OFFSET_UWORLD_GAMESTATE);
    int 玩家 = driver->read<int>(GameState + OFFSET_GAMESTATE_REALPLAYER); // RealPlayerNum
    int 真实 = driver->read<int>(GameState + OFFSET_GAMESTATE_ALIVEPLAYER); // AlivePlayerNum
    int 队伍 = driver->read<int>(GameState + OFFSET_GAMESTATE_ALIVETEAM); // AliveTeamNum

    memset(matrix, 0, 16);
    driver->read(Matrix, matrix, 16 * 4);
    
    PlayerCount = 0;
    RobotCount = 0;
    AimCount = 0;
    被瞄准对象数量 = 0;
    DrawIo[91] = false; // Object Names feature disabled.
    DrawIo[90] = false;

    // Radar base is drawn once per frame. Entity dots are drawn later per valid player.
    float zeusRadarSize = NumIo[17];
    if (zeusRadarSize < 80.0f) zeusRadarSize = 80.0f;
    if (zeusRadarSize > 260.0f) zeusRadarSize = 260.0f;
    float zeusRadarHalf = zeusRadarSize * 0.5f;
    float zeusRadarX = (NumIo[53] > 1.0f) ? NumIo[53] : 180.0f;
    float zeusRadarY = (NumIo[54] > 1.0f) ? NumIo[54] : 260.0f;
    if (DrawIo[10]) {
        zeusRadarX = ImClamp(zeusRadarX, zeusRadarHalf + 4.0f, (float)displayInfo.width - zeusRadarHalf - 4.0f);
        zeusRadarY = ImClamp(zeusRadarY, zeusRadarHalf + 4.0f, (float)displayInfo.height - zeusRadarHalf - 4.0f);
        NumIo[53] = zeusRadarX;
        NumIo[54] = zeusRadarY;
        NumIo[17] = zeusRadarSize;
        ImVec2 radarMin(zeusRadarX - zeusRadarHalf, zeusRadarY - zeusRadarHalf);
        ImVec2 radarMax(zeusRadarX + zeusRadarHalf, zeusRadarY + zeusRadarHalf);
        Draw->AddRectFilled(radarMin, radarMax, ImColor(10, 12, 18, 150), 10.0f);
        Draw->AddRect(radarMin, radarMax, ImColor(170, 190, 220, 150), 10.0f, 0, 1.4f);
        Draw->AddLine(ImVec2(zeusRadarX - 9.0f, zeusRadarY), ImVec2(zeusRadarX + 9.0f, zeusRadarY), ImColor(220, 230, 245, 125), 1.0f);
        Draw->AddLine(ImVec2(zeusRadarX, zeusRadarY - 9.0f), ImVec2(zeusRadarX, zeusRadarY + 9.0f), ImColor(220, 230, 245, 125), 1.0f);
    }
   // UpdateSmokeCircles();
    for (int i = 0; i < countddr; i++)
    {
        
        Objaddr = driver->read<uintptr_t>(Arrayaddr+ 8 * i);// 遍历数量次数
           if (Objaddr <= 0xffff || Objaddr == 0 || Objaddr <= 0x10000000 || Objaddr % 4 != 0 || Objaddr >= 0x10000000000)
{   
 continue;
}

        int ClassID = driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_CLASSID);	// 物资id指针10h
			char ClassName[64] = "";
			long int FNameEntry = driver->read<uintptr_t>(driver->read<uintptr_t>(Gname + (ClassID / 0x4000) * 0x8) + (ClassID % 0x4000) * 0x8);	// 序
			driver->read(FNameEntry + 0xC, ClassName, sizeof(ClassName));
	


        
        Vector3A Z;
        driver->read(driver->read<uintptr_t>(oneself + OFFSET_ACTOR_ROOTCOMPONENT) + OFFSET_COMPONENT_POSITION, &Z, sizeof(Z));// 自己坐标
        // 敌人和物资坐标
        Vector3A D;
        driver->read(driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_ROOTCOMPONENT) + OFFSET_COMPONENT_POSITION, &D, sizeof(D));// 对象坐标
        if(D.X == 0 && D.Y == 0 && D.Z == 0)
		{
		 continue;
		};
        // 计算人物矩阵
        camera = matrix[3] * D.X + matrix[7] * D.Y + matrix[11] * D.Z + matrix[15];
        r_x =
        px + (matrix[0] * D.X + matrix[4] * D.Y + matrix[8] * D.Z +
        matrix[12]) / camera * px;
        r_y =
        py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z - 5) +
        matrix[13]) / camera * py;
        r_w =
        py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z + 205) +
        matrix[13]) / camera * py;
        float x = r_x - (r_y - r_w) / 4;
        float y = r_y;
        float w = (r_y - r_w) / 2;
        float 距离 = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;
        float Distance =
        sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;
        long int 载具指针 = driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_VEHICLE_COMMON); // VehicleCommon
    // 计算载具血量和油量
    载具耐久 = driver->read<float>(载具指针 + OFFSET_VEHICLE_HEALTH) / driver->read<float>(载具指针 + OFFSET_VEHICLE_HEALTH_MAX) * 100;
    载具油量 = driver->read<float>(载具指针 + OFFSET_VEHICLE_FUEL) / driver->read<float>(载具指针 + OFFSET_VEHICLE_FUEL_MAX) * 100;
    // Weapon ESP values are resolved safely later, only for valid player actors.
    int 手持 = 0;
    int 当前子弹 = 0;
    int 最大子弹 = 0;

if(camera > 0.01f){
if (DrawIo[92] && (strstr(ClassName, "CharacterDeadInventoryBox_C") != 0 ||
strstr(ClassName,"EscapePlayerTombBox_C") != 0 ||
strstr(ClassName,"aracterDeadInventoryBox_C") != 0 ||
strstr(ClassName,"SuperPeopleDeadBox_C") != 0 ||
strstr(ClassName,"_TrainingBoxListWrapper_C") != 0 ||
strstr(ClassName,"PickUpListWrapperActor") != 0) && 距离<=150)
{
float 准信距离= sqrt(pow(px - x, 2) + pow(py - y, 2));

if (准信距离 <= 180.0f)
{
uintptr_t 盒内物资指针= driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_DEADBOX_INV);
int 盒内物资= driver->read<uintptr_t>(盒内物资指针 + OFFSET_INV_COUNT);

if (盒内物资 > 0 && 盒内物资 < 1000)
{
uintptr_t 物资数组= 盒内物资指针 + OFFSET_INV_ARRAY;

float startY = y - 40;

int displayedItemCount = 0;

for (int i = 0; i < 盒内物资; i++)
{
int itemID= driver->read<int>(物资数组 + OFFSET_INV_ITEM_ID * i);

if (itemID > 0)
{
const char*itemName = GetItems(itemID);

if (!itemName || strlen(itemName) == 0 || strcmp(itemName, "Unknown") == 0)
{
continue;
}
char itemText[128];
snprintf(itemText, sizeof(itemText), "%s", itemName);

float fontSize = 10 *3.829;

auto itemSize = ImGui::CalcTextSize(itemText, 0, fontSize);

float textX = x - itemSize.x / 2;
float textY= y - 20*2.946 - (displayedItemCount * 19.175);

Draw->AddText(NULL, fontSize, {textX, textY},
白色,itemText);

displayedItemCount++;
}
}
}
}
}
}
        if (strstr(ClassName, "Pawn") || strstr(ClassName, "Character"))
        {
            x1 = x+w/2;
            left = (x + w / 2) - w / 2.6f;
            right = x + w / 1.12f;
            
            if (Distance > 600)///观透
            {
            continue;
            }               
            
            int TeamID = driver->read<int>(Objaddr + OFFSET_ACTOR_TEAMID);//队伍
            float CurrentHealth = driver->read<float>(Objaddr + OFFSET_ACTOR_HEALTH);
            float 最大血量 = CurrentHealth; // legacy bone-layout selector
            float MaxHealth = driver->read<float>(Objaddr + OFFSET_ACTOR_HEALTH_MAX);
            float Health = 0.0f;
            if (CurrentHealth >= 0.0f && MaxHealth > 1.0f && MaxHealth < 100000.0f) {
                Health = (CurrentHealth / MaxHealth) * 100.0f;
            } else if (CurrentHealth >= 0.0f && CurrentHealth <= 100.0f) {
                // Fallback for builds/entities that expose health directly as 0..100.
                Health = CurrentHealth;
            }
            if (Health < 0.0f) Health = 0.0f;
            if (Health > 100.0f) Health = 100.0f;
int isBotRaw = driver->read<int>(Objaddr + OFFSET_ACTOR_ISBOT);
unsigned char isBotByte = driver->read<unsigned char>(Objaddr + OFFSET_ACTOR_ISBOT);
int isEnsureRaw = driver->read<int>(Objaddr + OFFSET_ACTOR_ENSURE);
unsigned char isEnsureByte = driver->read<unsigned char>(Objaddr + OFFSET_ACTOR_ENSURE);
int isBot = (isBotByte != 0 || isEnsureByte != 0 || isBotRaw == 1 || isEnsureRaw == 1 ||
             isBotRaw == 16842753 || isBotRaw == 16843009 || isBotRaw == 16843008 ||
             isEnsureRaw == 16842753 || isEnsureRaw == 16843009 || isEnsureRaw == 16843008 ||
             TeamID == 0) ? 1 : 0;
                 if (DrawIo[35]) {
            if (isBot== 1)
            {
                continue;   
            }
            }            
            int State = driver->read<int>(Objaddr + OFFSET_ACTOR_STATUS);//状态
            if (isBot == 1 || TeamID == 0)
            颜色1 = 绿色;//绿色
            
            else
颜色1 = GetTeamColor(TeamID); // 按队伍分辨颜色


            Vector3A Movement;
            //向量，自瞄有的人没效果
            CurrentVehicle = driver->read<uintptr_t>(Objaddr + 0xeb0);
            if (CurrentVehicle)
            {
                driver->read(CurrentVehicle + 0x354, &Movement, sizeof(Movement));//向量
            }
            else
            {
            driver->read(driver->read<uintptr_t>(Objaddr + 0x43c) + 0x2c0, &Movement, sizeof(Movement));// 对象坐标
            }

            float angle = driver->read<float>(oneself + OFFSET_ACTOR_ROTATION)-90;
            float angle2 = driver->read<float>(Objaddr + OFFSET_ACTOR_ROTATION);
            Vector2A Radar = rotateCoord(angle, (Z.X - D.X) / (30000/NumIo[17]), (Z.Y - D.Y) / (30000/NumIo[17]));
            
           
	        if (State == 262144 || State == 262152)
			{
				continue;
			}

      
                  if (Myteam == TeamID||距离<=1||NumIo[96]<=距离)// 不输出自己和队友坐标
            {
                continue;
            }


            


            getUTF8(PlayerName, driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_PLAYERNAME));
            auto ZeusBadDisplayName = [](const char* n) -> bool {
                if (!n || n[0] == '\0') return true;
                int questionCount = 0;
                int asciiPrintable = 0;
                for (int zi = 0; n[zi] != '\0' && zi < 63; ++zi) {
                    unsigned char c = (unsigned char)n[zi];
                    if (c == '?') questionCount++;
                    if (c >= 32 && c < 127) asciiPrintable++;
                    if (c < 32) return true;
                }
                return questionCount >= 1 || asciiPrintable == 0;
            };
            if (isBot == 1) {
                snprintf(PlayerName, sizeof(PlayerName), "%s", "Bot");
            } else if (ZeusBadDisplayName(PlayerName)) {
                snprintf(PlayerName, sizeof(PlayerName), "%s", "Player");
            }
            // Player UID
            getUTF8(UID, driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_PLAYERUID));
            Mesh = driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_MESH);
            
           if (Mesh <= 0xffff) {
				continue;
				}
				// 骨骼阵列
            human = Mesh + OFFSET_ACTOR_ATTACH_INFO + 0x10;//+骨骼矩阵
            // 骨骼指针
            Bone = driver->read<uintptr_t>(Mesh + OFFSET_MESH_STATIC_MESH);
           if (Bone <= 0xffff) {
             continue;
            }
                  
	int p = (BoneColor == 68) ? 33 : 32; // 右肩膀
int o = (BoneColor == 68) ? 34 : 33; // 右手肘
int a = (BoneColor == 68) ? 13 : 63; // 左手腕
int b = (BoneColor == 68) ? 35 : 62; // 右手腕
int c = (BoneColor == 68) ? 55 : 52; // 左大腿
int d = (BoneColor == 68) ? 59 : 56; // 右大腿
int e = (BoneColor == 68) ? 56 : 53; // 左膝盖
int f = (BoneColor == 68) ? 60 : 57; // 右膝盖
int g = (BoneColor == 68) ? 57 : 54; // 左脚腕
int h = (BoneColor == 68) ? 61 : 58; // 右脚腕

FTransform meshtrans = getBone(human);
FMatrix c2wMatrix = TransformToMatrix(meshtrans);

// 头部
FTransform headtrans = getBone(Bone + 5 * 48);
FMatrix boneMatrix = TransformToMatrix(headtrans);
Vector3A relLocation = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
Vector2A NeckBase = WorldToScreen(relLocation, matrix, camera); 
relLocation.Z += 7; // 脖子长度
Vector2A Head = WorldToScreen(relLocation, matrix, camera);

// 胸部 (Neck)
FTransform chesttrans;

 chesttrans = getBone(Bone + 4 * 48);
FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
Vector3A relLocation1 = MarixToVector(MatrixMulti(boneMatrix1, c2wMatrix));
Vector2A Chest = WorldToScreen(relLocation1, matrix, camera);

// 脊椎 (Spine)
FTransform spinetrans = getBone(Bone + 2 * 48);
FMatrix boneMatrixSpine = TransformToMatrix(spinetrans);
Vector3A relLocationSpine = MarixToVector(MatrixMulti(boneMatrixSpine, c2wMatrix));
Vector2A Spine = WorldToScreen(relLocationSpine, matrix, camera);

// 盆骨
FTransform pelvistrans = getBone(Bone + 1 * 48); 
FMatrix boneMatrix2 = TransformToMatrix(pelvistrans);
Vector3A LrelLocation1 = MarixToVector(MatrixMulti(boneMatrix2, c2wMatrix));
Vector2A Pelvis = WorldToScreen(LrelLocation1, matrix, camera);

// 左肩膀
FTransform lshtrans;
if (最大血量<=150)
 lshtrans = getBone(Bone + 11 * 48);
else
 lshtrans = getBone(Bone + 13 * 48);
FMatrix boneMatrix3 = TransformToMatrix(lshtrans);
Vector3A relLocation2 = MarixToVector(MatrixMulti(boneMatrix3, c2wMatrix));
Vector2A Left_Shoulder = WorldToScreen(relLocation2, matrix, camera);

// 右肩膀
FTransform rshtrans;
if (最大血量<=150)
 rshtrans = getBone(Bone + p * 48);
else
 rshtrans = getBone(Bone + 34 * 48);
FMatrix boneMatrix4 = TransformToMatrix(rshtrans);
Vector3A relLocation3 = MarixToVector(MatrixMulti(boneMatrix4, c2wMatrix));
Vector2A Right_Shoulder = WorldToScreen(relLocation3, matrix, camera);

// 左手肘
FTransform lelbtrans;
if (最大血量<=150)
 lelbtrans = getBone(Bone + 12 * 48);
else
 lelbtrans = getBone(Bone + 14 * 48);
FMatrix boneMatrix5 = TransformToMatrix(lelbtrans);
Vector3A relLocation4 = MarixToVector(MatrixMulti(boneMatrix5, c2wMatrix));
Vector2A Left_Elbow = WorldToScreen(relLocation4, matrix, camera);

// 右手肘
FTransform relbtrans;
if (最大血量<=150)
 relbtrans = getBone(Bone + o * 48);
else
 relbtrans = getBone(Bone + 35 * 48);
FMatrix boneMatrix6 = TransformToMatrix(relbtrans);
Vector3A relLocation5 = MarixToVector(MatrixMulti(boneMatrix6, c2wMatrix));
Vector2A Right_Elbow = WorldToScreen(relLocation5, matrix, camera);

// 左手腕
FTransform lwtrans;
if (最大血量<=150)
 lwtrans = getBone(Bone + a * 48);
else
 lwtrans = getBone(Bone + 16 * 48);
FMatrix boneMatrix7 = TransformToMatrix(lwtrans);
Vector3A relLocation6 = MarixToVector(MatrixMulti(boneMatrix7, c2wMatrix));
Vector2A Left_Wrist = WorldToScreen(relLocation6, matrix, camera);

// 右手腕
FTransform rwtrans;
if (最大血量<=150)
 rwtrans = getBone(Bone + b * 48);
else
 rwtrans = getBone(Bone + 37 * 48);
FMatrix boneMatrix8 = TransformToMatrix(rwtrans);
Vector3A relLocation7 = MarixToVector(MatrixMulti(boneMatrix8, c2wMatrix));
Vector2A Right_Wrist = WorldToScreen(relLocation7, matrix, camera);

// 左大腿
FTransform Llshtrans;
if (最大血量<=150)
 Llshtrans = getBone(Bone + c * 48);
else
 Llshtrans = getBone(Bone + 54 * 48);
FMatrix boneMatrix9 = TransformToMatrix(Llshtrans);
Vector3A LrelLocation2 = MarixToVector(MatrixMulti(boneMatrix9, c2wMatrix));
Vector2A Left_Thigh = WorldToScreen(LrelLocation2, matrix, camera);

// 右大腿
FTransform Lrshtrans;
if (最大血量<=150)
 Lrshtrans = getBone(Bone + d * 48);
else
 Lrshtrans = getBone(Bone + 58 * 48);
FMatrix boneMatrix10 = TransformToMatrix(Lrshtrans);
Vector3A LrelLocation3 = MarixToVector(MatrixMulti(boneMatrix10, c2wMatrix));
Vector2A Right_Thigh = WorldToScreen(LrelLocation3, matrix, camera);

// 左膝盖
FTransform Llelbtrans;
if (最大血量<=150)
 Llelbtrans = getBone(Bone + e * 48);
else
 Llelbtrans = getBone(Bone + 55 * 48);
FMatrix boneMatrix11 = TransformToMatrix(Llelbtrans);
Vector3A LrelLocation4 = MarixToVector(MatrixMulti(boneMatrix11, c2wMatrix));
Vector2A Left_Knee = WorldToScreen(LrelLocation4, matrix, camera);

// 右膝盖
FTransform Lrelbtrans;
if (最大血量<=150)
 Lrelbtrans = getBone(Bone + f * 48);
else
 Lrelbtrans = getBone(Bone + 59 * 48);
FMatrix boneMatrix12 = TransformToMatrix(Lrelbtrans);
Vector3A LrelLocation5 = MarixToVector(MatrixMulti(boneMatrix12, c2wMatrix));
Vector2A Right_Knee = WorldToScreen(LrelLocation5, matrix, camera);

// 左脚腕
FTransform Llwtrans;
if (最大血量<=150)
 Llwtrans = getBone(Bone + g * 48);
else
 Llwtrans = getBone(Bone + 56 * 48);
FMatrix boneMatrix13 = TransformToMatrix(Llwtrans);
Vector3A LrelLocation6 = MarixToVector(MatrixMulti(boneMatrix13, c2wMatrix));
Vector2A Left_Ankle = WorldToScreen(LrelLocation6, matrix, camera);

// 右脚腕
FTransform Lrwtrans;
if (最大血量<=150)
 Lrwtrans = getBone(Bone + h * 48);
else
 Lrwtrans = getBone(Bone + 60 * 48);
FMatrix boneMatrix14 = TransformToMatrix(Lrwtrans);
Vector3A LrelLocation7 = MarixToVector(MatrixMulti(boneMatrix14, c2wMatrix));
Vector2A Right_Ankle = WorldToScreen(LrelLocation7, matrix, camera);

// Max
FTransform Maxs = getBone(Bone + 6 * 48); // 假设 Max 的索引是 6 * 48
FMatrix boneMatrix15 = TransformToMatrix(Maxs);
Vector3A Max = MarixToVector(MatrixMulti(boneMatrix15, c2wMatrix));

// Min
FTransform Mins = getBone(Bone + 58 * 48); // 假设 Min 的索引是 58 * 48
FMatrix boneMatrix16 = TransformToMatrix(Mins);
Vector3A Min = MarixToVector(MatrixMulti(boneMatrix16, c2wMatrix));
            
            if (LrelLocation6.Z < LrelLocation7.Z)
            {
                LrelLocation7.Z = LrelLocation6.Z;
            }
            
            
            
            auto ZeusValidScreenPoint = [](const Vector2A& p) -> bool {
                return p.X > -10000.0f && p.X < 10000.0f && p.Y > -10000.0f && p.Y < 10000.0f;
            };
            top1 = fabsf(Pelvis.Y - Head.Y);
            float bodyHeight = fabsf(Pelvis.Y - Head.Y) * 2.0f;
            if (bodyHeight < fabsf(r_y - r_w)) bodyHeight = fabsf(r_y - r_w);
            if (bodyHeight < 48.0f) bodyHeight = 48.0f;

            top = ZeusValidScreenPoint(Head) ? (Head.Y - bodyHeight * 0.08f) : r_w;
            float footBottom = top + bodyHeight;
            bool leftFootOk = ZeusValidScreenPoint(Left_Ankle);
            bool rightFootOk = ZeusValidScreenPoint(Right_Ankle);
            if (leftFootOk && rightFootOk) footBottom = (Left_Ankle.Y > Right_Ankle.Y) ? Left_Ankle.Y : Right_Ankle.Y;
            else if (leftFootOk) footBottom = Left_Ankle.Y;
            else if (rightFootOk) footBottom = Right_Ankle.Y;
            else footBottom = top + bodyHeight;
            bottom = footBottom + bodyHeight * 0.06f;
            if (bottom <= top + 12.0f) bottom = top + bodyHeight;

        float originalHeight = bottom - top;
        if (originalHeight < 40.0f) originalHeight = 40.0f;
        float originalWidth = originalHeight * 0.46f;

        // Clean, clamped box scaling. Default NumIo[77]/[78] is 1.0.
        float widthScale = NumIo[77];
        float heightScale = NumIo[78];
        if (widthScale < 0.70f || widthScale > 2.00f) widthScale = 1.0f;
        if (heightScale < 0.85f || heightScale > 2.20f) heightScale = 1.0f;
        float scaledWidth = originalWidth * widthScale;
        float scaledHeight = originalHeight * heightScale;

        float centerX = ZeusValidScreenPoint(Head) ? Head.X : x1;
        float scaledBottom = bottom;
        float scaledTop = scaledBottom - scaledHeight;
        float scaledLeft = centerX - scaledWidth / 2.0f;
        float scaledRight = centerX + scaledWidth / 2.0f;
bool is_special_state = (State == 16392 || State == 16416 || State == 16417 || 
                        State == 16400 || State == 17424 || State == 16401 || 
                        State == 17425 || State == 16393 || State == 17416 || 
                        State == 17417 || State == 16457 || State == 16456);

if (is_special_state)
{
    std::string full_text = "Throwable nearby!!!";
    
    Draw->AddText(NULL, 50, {px+10*-15.317, py+10*-75.994}, ImColor(255, 0, 0, 255), full_text.c_str());
}
if (camera > 0.01f)
            {
                if (DrawIo[20])
                {
                    
                   bool isDistanceValid = Distance <= NumIo[98] || 
                      (NumIo[98] < Distance && Distance < NumIo[30] && (Aiming == 1 || Aiming == 257));

bool isDrawIo22Valid = DrawIo[22] ? Health > 0 : true;

bool isDrawIo23Valid = DrawIo[23] ? (isBot != 1) : true;
                    
                         
if (isDistanceValid && isDrawIo22Valid && isDrawIo23Valid) {
    // --- START OF YOUR NEW CODE ---
    if (Firing == 1 || Aiming == 1) {
        if (DrawIo[29]) { // If BT button is ON
            auto DistAim = sqrt(pow(px - Head.X, 2) + pow(py - Head.Y, 2));
            float Kurr = NumIo[3]; // Uses your FOV setting
            if (DistAim < Kurr) {
                PTarget = i; // Now 'PTarget' is declared and will work
                Kurr = DistAim;
            }
        }
    // --- END OF YOUR NEW CODE ---

        strcpy(Aim[AimCount].Name, PlayerName);
        Aim[AimCount].WodDistance = Distance;
        Aim[AimCount].AimMovement = Movement;
        Aim[AimCount].jl = Distance;
        Aim[AimCount].血量 = Health;
        Aim[AimCount].名字 = PlayerName;
        Aim[AimCount].boxLeft = scaledLeft;
        Aim[AimCount].boxTop = scaledTop;
        Aim[AimCount].boxRight = scaledRight;
        Aim[AimCount].boxBottom = scaledBottom;

        // Bone selection
        if (NumIo[8] == 1.0f) {
            Aim[AimCount].ObjAim = relLocation;
            Aim[AimCount].ScreenDistance = sqrt(pow(px - Head.X, 2) + pow(py - Head.Y, 2));
        } else if (NumIo[8] == 2.0f || NumIo[8] == 0.0f) {
            Aim[AimCount].ObjAim = relLocation1;
            Aim[AimCount].ScreenDistance = sqrt(pow(px - Chest.X, 2) + pow(py - Chest.Y, 2));
        } else if (NumIo[8] == 3.0f) {
            Aim[AimCount].ObjAim = LrelLocation1;
            Aim[AimCount].ScreenDistance = sqrt(pow(px - Pelvis.X, 2) + pow(py - Pelvis.Y, 2));
        } else {
            Aim[AimCount].ObjAim = relLocation1;
            Aim[AimCount].ScreenDistance = sqrt(pow(px - Chest.X, 2) + pow(py - Chest.Y, 2));
        }
        AimCount++;
    }
}
                    
                }
                
                
            }
  
if (DrawIo[10] && Distance < 320.0f)
{
    float relativeX = (D.X - Z.X) * 0.01f;
    float relativeY = (D.Y - Z.Y) * 0.01f;
    float scale = zeusRadarHalf / 220.0f;
    if (scale <= 0.001f) scale = 0.001f;
    float radarX = zeusRadarX + relativeX * scale;
    float radarY = zeusRadarY + relativeY * scale;
    ImVec2 point = ImVec2(
        ImClamp(radarX, zeusRadarX - zeusRadarHalf + 6.0f, zeusRadarX + zeusRadarHalf - 6.0f),
        ImClamp(radarY, zeusRadarY - zeusRadarHalf + 6.0f, zeusRadarY + zeusRadarHalf - 6.0f)
    );
    ImColor dotColor = (isBot == 1) ? ImColor(230, 238, 250, 255) : ImColor(255, 60, 70, 255);
    Draw->AddCircleFilled(point, 5.0f, dotColor);
}

// 定义屏幕矩形
ImRect screen_rect = {px, py, px, py};
// 获取屏幕中心坐标
ImVec2 screen_center = screen_rect.GetCenter();
// 敌人屏幕坐标
ImVec2 Obj = ImVec2(r_x, r_y);

// 计算敌人相对于屏幕中心的角度（数学坐标系，弧度制）
float arrowAngle = atan2(screen_center.y - Obj.y, screen_center.x - Obj.x);

// Adjust angle based on camera direction
arrowAngle += camera > 0 ? M_PI : 0.0f;
float Radius = 90.0f;

// Calculate arrow center position
ImVec2 arrow_center = ImVec2(
    screen_center.x + Radius * cosf(arrowAngle),
    screen_center.y + Radius * sinf(arrowAngle)
);

// 计算透明度
float alpha = 1.0f;
if (camera > 0) {
    constexpr float nearThreshold = 200 * 200;
    ImVec2 screen_outer_diff = {
        Obj.x < 0 ? abs(Obj.x) : (Obj.x > screen_rect.Max.x ? Obj.x - screen_rect.Max.x : 0.0f),
        Obj.y < 0 ? abs(Obj.y) : (Obj.y > screen_rect.Max.y ? Obj.y - screen_rect.Max.y : 0.0f)
    };
    float distance = static_cast<float>(pow(screen_outer_diff.x, 2) + pow(screen_outer_diff.y, 2));
    alpha = camera < 0 ? 1.0f : (distance / nearThreshold);
}

// 设置箭头颜色 - 添加人机判断
ImColor arrowColor;
if (isBot == 1 || TeamID == 0) {
    // 人机或TeamID为0时显示绿色
    arrowColor = ImColor(255, 255, 255); // 绿色
} else {
    // 非人机时按队伍分辨颜色
    arrowColor = GetTeamColor(TeamID);
}

// 设置透明度
arrowColor.Value.w = std::min(alpha, 1.0f);

// 绘制中空的圆形箭头
float circleRadius = 10.0f; // 圆形箭头的半径
ImGui::GetBackgroundDrawList()->AddCircle(arrow_center, circleRadius, arrowColor, 12, 4.0f); // 只绘制边框，不填充


	
           

            
            if(w>0)
            {
                
                // 方框
                if (DrawIo[1]){
					if (NumIos[13] == 0)
					{
                    	                int 扩大 = 3;
Draw->AddRect({scaledLeft - 扩大, scaledTop - 扩大},
              {scaledRight + 扩大, scaledBottom + 扩大},
              颜色1, {0}, 0, {1.5});
                	}else if (NumIos[13] == 1){
						Draw3DBox(camera,Max,Min,绿色,matrix);
					}else{
						DrawPlayerBox(ImGui::GetForegroundDrawList(),left,right,bottom,top,x,y,绿色);
					}
                }
                
                   if (DrawIo[2])
{
    int targetY = (DrawIo[4]) ? top -50 : top;

    if (isBot == 1)
    {
        Draw->AddLine({px, 130}, {Head.X, targetY}, 人机射线, {1.0});
    }
    else
    {
        Draw->AddLine({px, 130}, {Head.X, targetY}, 真人射线, {1.0});
    }
}

                
                if (DrawIo[3]) {
                  if (isBot == 1)
                  {
                    Draw->AddCircle({Head.X , Head.Y}, w/7, 人机射线, 0);
                    Draw->AddLine({NeckBase.X, NeckBase.Y}, {Chest.X, Chest.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Spine.X, Spine.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Spine.X, Spine.Y}, {Pelvis.X, Pelvis.Y},人机射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Left_Shoulder.X,Left_Shoulder.Y},人机射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Right_Shoulder.X,Right_Shoulder.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Left_Shoulder.X, Left_Shoulder.Y}, {Left_Elbow.X,Left_Elbow.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Right_Shoulder.X, Right_Shoulder.Y},{Right_Elbow.X, Right_Elbow.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Left_Elbow.X, Left_Elbow.Y}, {Left_Wrist.X,Left_Wrist.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Right_Elbow.X, Right_Elbow.Y}, {Right_Wrist.X,Right_Wrist.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Pelvis.X, Pelvis.Y}, {Left_Thigh.X, Left_Thigh.Y},人机射线,{NumIo[32]});
                    Draw->AddLine({Pelvis.X, Pelvis.Y}, {Right_Thigh.X,Right_Thigh.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Left_Thigh.X, Left_Thigh.Y}, {Left_Knee.X,Left_Knee.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Right_Thigh.X, Right_Thigh.Y}, {Right_Knee.X,Right_Knee.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Left_Knee.X, Left_Knee.Y}, {Left_Ankle.X,Left_Ankle.Y}, 人机射线,{NumIo[32]});
                    Draw->AddLine({Right_Knee.X, Right_Knee.Y}, {Right_Ankle.X,Right_Ankle.Y}, 人机射线,{NumIo[32]});
                }else{
                    Draw->AddCircle({Head.X , Head.Y}, w/7, 真人射线, 0);
                    Draw->AddLine({NeckBase.X, NeckBase.Y}, {Chest.X, Chest.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Spine.X, Spine.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Spine.X, Spine.Y}, {Pelvis.X, Pelvis.Y},真人射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Left_Shoulder.X,Left_Shoulder.Y},真人射线,{NumIo[32]});
                    Draw->AddLine({Chest.X, Chest.Y}, {Right_Shoulder.X,Right_Shoulder.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Left_Shoulder.X, Left_Shoulder.Y}, {Left_Elbow.X,Left_Elbow.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Right_Shoulder.X, Right_Shoulder.Y},{Right_Elbow.X, Right_Elbow.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Left_Elbow.X, Left_Elbow.Y}, {Left_Wrist.X,Left_Wrist.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Right_Elbow.X, Right_Elbow.Y}, {Right_Wrist.X,Right_Wrist.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Pelvis.X, Pelvis.Y}, {Left_Thigh.X, Left_Thigh.Y},真人射线,{NumIo[32]});
                    Draw->AddLine({Pelvis.X, Pelvis.Y}, {Right_Thigh.X,Right_Thigh.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Left_Thigh.X, Left_Thigh.Y}, {Left_Knee.X,Left_Knee.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Right_Thigh.X, Right_Thigh.Y}, {Right_Knee.X,Right_Knee.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Left_Knee.X, Left_Knee.Y}, {Left_Ankle.X,Left_Ankle.Y}, 真人射线,{NumIo[32]});
                    Draw->AddLine({Right_Knee.X, Right_Knee.Y}, {Right_Ankle.X,Right_Ankle.Y}, 真人射线,{NumIo[32]});
                }
                }
                //名字

                //距离
                			if (DrawIo[5]) {
                    std::string s;
                    s += std::to_string((int) Distance);
                    s += "M";
                    const float distanceFontSize = 26.0f;
                    ImFont* distanceFont = Font2 ? Font2 : ImGui::GetFont();
                    auto textSize = distanceFont->CalcTextSizeA(distanceFontSize, FLT_MAX, 0.0f, s.c_str());
                    float distanceY = scaledBottom + 8.0f;
                    Draw->AddText(distanceFont, distanceFontSize, {centerX - (textSize.x * 0.5f), distanceY}, 白色, s.c_str());
                }
                					//动作
				if (DrawIo[13]) {
		    		std::string s;
		    		s += "\n";
                    s += 状态(State);
                //    s += to_string((int)State);
                    auto textSize = ImGui::CalcTextSize(s.c_str(), 0, 25);                    
                    Draw->AddText(NULL,30,{ x1 - (textSize.x / 2) - 97.791,top - 90 - NumIo[34] - -14.728}, 白色,s.c_str());
                }

// Name / Team ID are independent from Health Bar.
char TeamText[96] = "";
if (DrawIo[6] || DrawIo[8]) {
    if (DrawIo[6] && DrawIo[8]) {
        snprintf(TeamText, sizeof(TeamText), "%d.%s", TeamID, PlayerName);
    } else if (DrawIo[8]) {
        snprintf(TeamText, sizeof(TeamText), "%d", TeamID);
    } else {
        snprintf(TeamText, sizeof(TeamText), "%s", PlayerName);
    }
    float nameSize = 18.0f;
    ImFont* nameFont = Font2 ? Font2 : ImGui::GetFont();
    ImVec2 nameTextSize = nameFont->CalcTextSizeA(nameSize, FLT_MAX, 0.0f, TeamText);
    float nameY = scaledTop - (DrawIo[4] ? 52.0f : 24.0f);
    float nameX = centerX - nameTextSize.x * 0.5f;
    ImGui::GetBackgroundDrawList()->AddText(nameFont, nameSize, ImVec2(nameX + 1, nameY + 1), ImColor(0,0,0,220), TeamText);
    ImGui::GetBackgroundDrawList()->AddText(nameFont, nameSize, ImVec2(nameX, nameY), ImColor(255,255,255,255), TeamText);
}

if (DrawIo[4]) {
    float barWidth = 140.0f;
    float barHeight = 16.0f;
    float barX = centerX - barWidth * 0.5f;
    float barY = scaledTop - 28.0f;
    float healthRatio = Health / 100.0f;
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    if (healthRatio > 1.0f) healthRatio = 1.0f;
    ImColor fillColor = healthRatio >= 0.75f ? ImColor(40, 255, 40, 220) :
                        healthRatio >= 0.40f ? ImColor(255, 185, 35, 220) :
                        healthRatio >= 0.20f ? ImColor(255, 95, 40, 220) :
                                               ImColor(255, 35, 35, 220);
    if (isBot) fillColor = ImColor(120, 235, 120, 220);
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barWidth, barY + barHeight), ImColor(0, 0, 0, 175), 4.0f);
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barWidth * healthRatio, barY + barHeight), fillColor, 4.0f);
    ImGui::GetBackgroundDrawList()->AddRect(ImVec2(barX, barY), ImVec2(barX + barWidth, barY + barHeight), ImColor(255,255,255,120), 4.0f, 0, 1.0f);
}
     std::unordered_map<int, MyTextureData*> weaponMap = {
    {101001, &ID101001}, {1010011, &ID101001}, {1010012, &ID101001}, {1010013, &ID101001}, {1010014, &ID101001},
    {1010015, &ID101001}, {1010016, &ID101001}, {1010017, &ID101001}, {1010019, &ID101001},
    {101002, &ID101002}, {1010021, &ID101002}, {1010022, &ID101002}, {1010023, &ID101002},
    {1010024, &ID101002}, {1010025, &ID101002},
    {101003, &ID101003}, {101903, &ID101003}, {1010031, &ID101003}, {1010032, &ID101003},
    {1010033, &ID101003}, {1010034, &ID101003}, {1010035, &ID101003}, {1010036, &ID101003},
    {1010037, &ID101003}, {1010039, &ID101003},
    {101004, &ID101004}, {1010041, &ID101004}, {1010042, &ID101004}, {1010043, &ID101004},
    {1010044, &ID101004}, {1010045, &ID101004}, {1010046, &ID101004}, {1010047, &ID101004},
    {1010049, &ID101004},
    {101005, &ID101005}, {1010051, &ID101005}, {1010052, &ID101005}, {1010053, &ID101005},
    {1010054, &ID101005}, {1010055, &ID101005}, {1010056, &ID101005}, {1010057, &ID101005},
    {1010059, &ID101005},
    {101006, &ID101006}, {1010061, &ID101006}, {1010062, &ID101006}, {1010063, &ID101006},
    {1010064, &ID101006}, {1010065, &ID101006}, {1010066, &ID101006}, {1010067, &ID101006},
    {1010069, &ID101006},
    {101007, &ID101007}, {1010071, &ID101007}, {1010072, &ID101007}, {1010073, &ID101007},
    {1010074, &ID101007}, {1010075, &ID101007}, {1010076, &ID101007}, {1010077, &ID101007},
    {1010079, &ID101007},
    {101008, &ID101008}, {101908, &ID101008}, {1010081, &ID101008}, {1010082, &ID101008},
    {1010083, &ID101008}, {1010084, &ID101008}, {1010085, &ID101008}, {1010086, &ID101008},
    {1010087, &ID101008}, {1010089, &ID101008},
    {101009, &ID101009}, {1010091, &ID101009}, {1010092, &ID101009}, {1010093, &ID101009},
    {1010094, &ID101009}, {1010095, &ID101009},
    {101010, &ID101010}, {1010101, &ID101010}, {1010102, &ID101010}, {1010103, &ID101010},
    {1010104, &ID101010}, {1010105, &ID101010}, {1010106, &ID101010}, {1010107, &ID101010},
    {101011, &ID101011},
    {101012, &ID101012}, {1010121, &ID101012}, {1010122, &ID101012}, {1010123, &ID101012},
    {1010124, &ID101012}, {1010125, &ID101012}, {1010126, &ID101012}, {1010127, &ID101012},
    {102001, &ID102001}, {102901, &ID102001}, {1020011, &ID102001}, {1020012, &ID102001},
    {1020013, &ID102001}, {1020014, &ID102001}, {1020015, &ID102001},
    {102002, &ID102002}, {1020021, &ID102002}, {1020022, &ID102002}, {1020023, &ID102002},
    {1020024, &ID102002}, {1020025, &ID102002}, {1020029, &ID102002},
    {102003, &ID102003}, {102903, &ID102003}, {1020031, &ID102003}, {1020032, &ID102003},
    {1020033, &ID102003}, {1020034, &ID102003}, {1020035, &ID102003}, {1020036, &ID102003},
    {1020037, &ID102003},
    {102004, &ID102004}, {1020041, &ID102004}, {1020042, &ID102004}, {1020043, &ID102004},
    {1020044, &ID102004}, {1020045, &ID102004},
    {102005, &ID102005}, {1020051, &ID102005}, {1020052, &ID102005}, {1020053, &ID102005},
    {1020054, &ID102005}, {1020055, &ID102005}, {1020059, &ID102005},
    {102007, &ID102007}, {1020071, &ID102007}, {1020072, &ID102007}, {1020073, &ID102007},
    {1020074, &ID102007}, {1020075, &ID102007}, {1020076, &ID102007}, {1020077, &ID102007},
    {102105, &ID102105}, {1021051, &ID102105}, {1021052, &ID102105}, {1021053, &ID102105},
    {1021054, &ID102105}, {1021055, &ID102105},
    {103001, &ID103001}, {103901, &ID103001}, {1030011, &ID103001}, {1030012, &ID103001},
    {1030013, &ID103001}, {1030014, &ID103001}, {1030015, &ID103001}, {1030016, &ID103001},
    {1030017, &ID103001}, {1030019, &ID103001},
    {103002, &ID103002}, {103902, &ID103002}, {1030021, &ID103002}, {1030022, &ID103002},
    {1030023, &ID103002}, {1030024, &ID103002}, {1030025, &ID103002}, {1030026, &ID103002},
    {1030027, &ID103002}, {1030029, &ID103002},
    {103003, &ID103003}, {103903, &ID103003}, {1030031, &ID103003}, {1030032, &ID103003},
    {1030033, &ID103003}, {1030034, &ID103003}, {1030035, &ID103003}, {1030036, &ID103003},
    {1030037, &ID103003}, {1030039, &ID103003},
    {103004, &ID103004}, {1030041, &ID103004}, {1030042, &ID103004}, {1030043, &ID103004},
    {1030044, &ID103004}, {1030045, &ID103004}, {1030046, &ID103004}, {1030047, &ID103004},
    {1030049, &ID103004},
    {103005, &ID103005}, {1030051, &ID103005}, {1030052, &ID103005}, {1030053, &ID103005},
    {1030054, &ID103005}, {1030055, &ID103005}, {1030059, &ID103005},
    {103006, &ID103006}, {1030061, &ID103006}, {1030062, &ID103006}, {1030063, &ID103006},
    {1030064, &ID103006}, {1030065, &ID103006}, {1030069, &ID103006},
    {103007, &ID103007}, {1030071, &ID103007}, {1030072, &ID103007}, {1030073, &ID103007},
    {1030074, &ID103007}, {1030075, &ID103007}, {1030076, &ID103007}, {1030077, &ID103007},
    {103008, &ID103008}, {1030081, &ID103008}, {1030082, &ID103008}, {1030083, &ID103008},
    {1030084, &ID103008}, {1030085, &ID103008},
    {103009, &ID103009}, {1030091, &ID103009}, {1030092, &ID103009}, {1030093, &ID103009},
    {1030094, &ID103009}, {1030095, &ID103009}, {1030096, &ID103009}, {1030097, &ID103009},
    {1030099, &ID103009},
    {103010, &ID103010}, {1030101, &ID103010}, {1030102, &ID103010}, {1030103, &ID103010},
    {1030104, &ID103010}, {1030105, &ID103010},
    {103011, &ID103011},
    {103012, &ID103012}, {1030121, &ID103012}, {1030122, &ID103012}, {1030123, &ID103012},
    {1030124, &ID103012}, {1030125, &ID103012}, {1030126, &ID103012}, {1030127, &ID103012},
    {103013, &ID103013},
    {103014, &ID103014},
    {103015, &ID103015},
    {103100, &ID103100}, {1031001, &ID103100}, {1031002, &ID103100}, {1031003, &ID103100},
    {1031004, &ID103100}, {1031005, &ID103100}, {1031006, &ID103100}, {1031007, &ID103100},
    {104001, &ID104001}, {1040011, &ID104001}, {1040012, &ID104001}, {1040013, &ID104001},
    {1040014, &ID104001}, {1040015, &ID104001},
    {104002, &ID104002}, {1040022, &ID104002}, {1040023, &ID104002}, {1040024, &ID104002},
    {1040025, &ID104002},
    {104003, &ID104003}, {1040031, &ID104003}, {1040032, &ID104003}, {1040033, &ID104003},
    {1040034, &ID104003}, {1040035, &ID104003}, {1040039, &ID104003},
    {104004, &ID104004}, {1040041, &ID104004}, {1040042, &ID104004}, {1040043, &ID104004},
    {1040044, &ID104004}, {1040045, &ID104004}, {1040049, &ID104004},
    {104100, &ID104100}, {104102, &ID104100}, {1041021, &ID104100}, {1041022, &ID104100},
    {1041023, &ID104100}, {1041024, &ID104100}, {1041025, &ID104100},
    {105001, &ID105001}, {1050011, &ID105001}, {1050012, &ID105001}, {1050013, &ID105001},
    {1050014, &ID105001}, {1050015, &ID105001}, {1050016, &ID105001}, {1050017, &ID105001},
    {105002, &ID105002}, {1050021, &ID105002}, {1050022, &ID105002}, {1050023, &ID105002},
    {1050024, &ID105002}, {1050025, &ID105002}, {1050029, &ID105002},
    {105010, &ID105010}, {1050101, &ID105010}, {1050102, &ID105010}, {1050103, &ID105010},
    {1050104, &ID105010}, {1050105, &ID105010}, {1050106, &ID105010}, {1050107, &ID105010},
    {1050109, &ID105010},
    {106001, &ID106001}, {1060011, &ID106001}, {1060012, &ID106001}, {1060013, &ID106001},
    {1060014, &ID106001}, {1060015, &ID106001}, {1060019, &ID106001},
    {106002, &ID106002}, {1060021, &ID106002}, {1060022, &ID106002}, {1060023, &ID106002},
    {1060024, &ID106002}, {1060025, &ID106002},
    {106003, &ID106003}, {1060031, &ID106003}, {1060032, &ID106003}, {1060033, &ID106003},
    {1060034, &ID106003}, {1060035, &ID106003},
    {106004, &ID106004}, {1060041, &ID106004}, {1060042, &ID106004}, {1060043, &ID106004},
    {1060044, &ID106004}, {1060045, &ID106004},
    {106005, &ID106005}, {1060051, &ID106005}, {1060052, &ID106005}, {1060053, &ID106005},
    {1060054, &ID106005}, {1060055, &ID106005},
    {106006, &ID106006}, {1060061, &ID106006}, {1060062, &ID106006}, {1060063, &ID106006},
    {1060064, &ID106006}, {1060065, &ID106006}, {1060069, &ID106006},
    {106007, &ID106007}, {1060071, &ID106007},
    {106008, &ID106008}, {1060081, &ID106008}, {1060082, &ID106008}, {1060083, &ID106008},
    {1060084, &ID106008}, {1060085, &ID106008}, {1060089, &ID106008},
    {106010, &ID106010}, {1060101, &ID106010}, {1060102, &ID106010}, {1060103, &ID106010},
    {1060104, &ID106010}, {1060105, &ID106010}, {1060109, &ID106010},
    {602002, &ID602002},
    {602003, &ID602003},
    {602004, &ID602004}
};

if (DrawIo[11]) {
    if (Health != 0) {
        auto IsValidGamePtr = [](uintptr_t ptr) -> bool {
            return ptr > 0x10000000ULL && ptr < 0x10000000000ULL && (ptr % 4) == 0;
        };

        uintptr_t weaponManager = driver->read<uintptr_t>(Objaddr + OFFSET_ACTOR_WEAPON_MANAGER);
        if (IsValidGamePtr(weaponManager)) {
            uintptr_t currentWeapon = driver->read<uintptr_t>(weaponManager + OFFSET_WEAPON_CURRENT_REPLICATED);
            if (IsValidGamePtr(currentWeapon)) {
                uintptr_t shootEntity = driver->read<uintptr_t>(currentWeapon + OFFSET_WEAPON_SHOOT_ENTITY);
                if (IsValidGamePtr(shootEntity)) {
                    手持 = driver->read<int>(shootEntity + OFFSET_WEAPON_ID);
                }
                当前子弹 = driver->read<int>(currentWeapon + OFFSET_WEAPON_AMMO_CURRENT);
                最大子弹 = driver->read<int>(currentWeapon + OFFSET_WEAPON_AMMO_MAX);
            }
        }

        std::string weaponName;
        std::string ammoText;
        if (手持 > 0 && 手持 < 10000000) {
            const char* rawWeaponName = GetHolGunItem(手持);
            if (!rawWeaponName || strcmp(rawWeaponName, "Not listed yet") == 0) {
                weaponName = std::to_string((int)手持);
            } else {
                weaponName = rawWeaponName;
            }
        }

        if (最大子弹 > 0 && 最大子弹 < 1000 && 当前子弹 >= 0 && 当前子弹 <= 1000) {
            ammoText = std::to_string((int)当前子弹) + "/" + std::to_string((int)最大子弹);
        }

        if (GImGui == nullptr) {
            goto ZEUS_WEAPON_ESP_END;
        }
        ImGuiContext& g = *GImGui;
        ImFont* font = g.Font ? g.Font : ImGui::GetFont();
        if (font == nullptr) {
            goto ZEUS_WEAPON_ESP_END;
        }
        
        // Compact weapon text; keep it centered and above name/health to avoid overlap.
        const float weaponNameFontSize = 27.0f;
        const float ammoFontSize = 25.0f;
        float weaponBaseY = scaledTop - 88.0f - NumIo[34];
        if (!weaponName.empty()) {
            auto weaponNameSize = font->CalcTextSizeA(weaponNameFontSize, FLT_MAX, -1.0f, weaponName.c_str(), NULL, NULL);
            Draw->AddText(font, weaponNameFontSize, { centerX - (weaponNameSize.x / 2.0f), weaponBaseY }, 凯撒, weaponName.c_str());
        }
        
        if (!ammoText.empty()) {
            auto ammoSize = font->CalcTextSizeA(ammoFontSize, FLT_MAX, -1.0f, ammoText.c_str(), NULL, NULL);
            float ammoY = weaponBaseY + weaponNameFontSize + 2.0f;
            Draw->AddText(font, ammoFontSize, { centerX - (ammoSize.x / 2.0f), ammoY }, 白色, ammoText.c_str());
        }
        
        static int lastWeapon = -1;
        static MyTextureData* lastTexture = nullptr;
        
        // 只在武器变化时查找
        if (手持 != lastWeapon) {
            lastWeapon = 手持;
            lastTexture = nullptr;
            
            auto it = weaponMap.find(手持);
            if (it != weaponMap.end()) {
                lastTexture = it->second;
            }
        }
        
        // 保持原始绘制逻辑
        if (lastTexture && lastTexture->DS) {
            float enemyHeadX = centerX;
            float enemyHeadY = scaledTop;
            
            ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
            float width = 2.0f*47.128, height = 2.0f*18.851;
            float offsetY = 2.0f*42.121;
            
            ImVec2 top_left(enemyHeadX - width / 2, enemyHeadY - height / 2 - offsetY);
            ImVec2 bottom_right(enemyHeadX + width / 2, enemyHeadY + height / 2 - offsetY);
            
            draw_list->AddImage(
                (ImTextureID)lastTexture->DS,
                top_left,
                bottom_right,
                ImVec2(0, 0), ImVec2(1, 1),
                IM_COL32(255, 255, 255, 255)
            );
        }
    }
}
ZEUS_WEAPON_ESP_END:
    ;

  

				}
                
            
            //w >0
            人物总结:
            if (isBot == 1 || TeamID == 0)
            RobotCount++;
            else
            PlayerCount++;
        }
        //特征
        else{      

                float x = r_x - (r_y - r_w) / 4;
                float y = r_y;
                float w = (r_y - r_w) / 2;                
                float Distance = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;   
                if (camera <= 0.01f || Distance > 800)
                {
                    continue;
                }                
                char *vp_name;
                char* s_name = ClassName;
                ImVec4 vp_color;
              if (DrawIo[15]){

                 //手雷预警
                if((GetGrenadeInfo(s_name, &vp_name, &vp_color))&& Distance < 200.0f)
                {
               /* if(IsSmokeGrenade(s_name)) {
                        AddSmokeCircle(D);
                    }*/
                    std::string goods;
                    goods += vp_name;
    goods += "[";
    goods += std::to_string((int)Distance);
    goods += "M]";

                    auto textSize = ImGui::CalcTextSize(goods.c_str(), 0, 30);
                    ImColor vpColor = ImColor(vp_color);
                    DrawTextOutline(45, x, y, 红色, goods.c_str());
                
                }
                }                
                //显示物资
if (DrawIo[104])
{
    ZeusSyncItemESPConfig();
    float itemDistanceLimit = NumIo[33];
    if (itemDistanceLimit < 20.0f) itemDistanceLimit = 20.0f;
    if (itemDistanceLimit > 400.0f) itemDistanceLimit = 400.0f;
    NumIo[33] = itemDistanceLimit;

    if (Distance < itemDistanceLimit && GetItemInfo(s_name, &vp_name, &vp_color))
    {
        int itemIndex = ZeusFindItemESPIndex(vp_name);
        if (itemIndex >= 0 && g_ZeusItemESP[itemIndex].enabled)
        {
            std::string goods;
            goods += g_ZeusItemESP[itemIndex].name;
            goods += "[";
            goods += std::to_string((int)Distance);
            goods += "M]";
            ImColor itemColor = ImColor(g_ZeusItemESP[itemIndex].color);
            const float itemFontSize = 27.0f;
            ImFont* itemFont = Font2 ? Font2 : ImGui::GetFont();
            auto textSize = itemFont->CalcTextSizeA(itemFontSize, FLT_MAX, 0.0f, goods.c_str());
            float priorityLift = (float)g_ZeusItemESP[itemIndex].priority * 0.03f;
            ImVec2 textPos(x - textSize.x * 0.5f, y - priorityLift);

            // Small readable capsule; nearby items get a tiny deterministic stagger to reduce overlap.
            float stagger = (float)((itemIndex % 4) * 9);
            textPos.y -= stagger;
            ImVec2 boxMin(textPos.x - 5.0f, textPos.y - 3.0f);
            ImVec2 boxMax(textPos.x + textSize.x + 5.0f, textPos.y + textSize.y + 3.0f);
            Draw->AddRectFilled(boxMin, boxMax, ImColor(8, 10, 14, 160), 5.0f);
            Draw->AddRect(boxMin, boxMax, itemColor, 5.0f, 0, 1.2f);
            Draw->AddText(itemFont, itemFontSize, ImVec2(textPos.x + 1.0f, textPos.y + 1.0f), ImColor(0, 0, 0, 210), goods.c_str());
            Draw->AddText(itemFont, itemFontSize, textPos, itemColor, goods.c_str());
        }
    }
}
                
                //类名
// ClassName logging disabled: no ClassNames.txt file is created anymore.
// The optional on-screen class display is handled below by DrawIo[91].
if(DrawIo[91])
                {
                	DrawIo[90] = true;
                    std::string ClassNameStr;
                    ClassNameStr += s_name;                   
                    auto textSize = ImGui::CalcTextSize(ClassNameStr.c_str(), 0, 30);
                    ImColor vpColor = ImColor(vp_color);
                    DrawTextOutline(45, x, y, 白色, ClassNameStr.c_str());
                } else{
                	DrawIo[90] = false;
                }
            if (DrawIo[14] && Distance > 2)
            {
                // Draw vehicle
if (GetVehicleInfo(s_name, &vp_name, &vp_color))
{
std::string goods;
goods += vp_name;
goods += "[";
goods += std::to_string((int)Distance);
goods += "M]";

   
    
// Draw main info (name and distance)
auto textSize = ImGui::CalcTextSize(goods.c_str(), 0, 30);
DrawTextOutline(30.0, x - 25, y, 白色, goods.c_str());
// ===== 位置与尺寸调整 =====
float barStartX = x - 30;          // 横向基准不变
float barStartY = y + textSize.y - 12;   // 往上移 8 像素

float barWidth  = 100;             // 总宽度缩到 100
float barHeight = 6;
float spacing   = 3;
float cornerRadius = 2.0f;

float shrink = 5.0f;               // 左右各缩 5 像素
float usableWidth = barWidth - 2 * shrink;

// ===== 油量条（白色）=====
// 背景
// 计算载具中心点在屏幕上的位置
float vehicleCenterX = r_x;  // 载具的屏幕X坐标
float vehicleCenterY = r_y - w/2;  // 载具的屏幕Y坐标（中心位置）

// 计算准星与载具中心的距离
float distanceToVehicle = sqrt(pow(px - vehicleCenterX, 2) + pow(py - vehicleCenterY, 2));

// 只在准星指向该载具时显示油量和耐久条（设置一个阈值，比如50像素内
if (DrawIo[88]){
if (distanceToVehicle <= 110.0f) 
{
    // ===== 油量条（白色）=====
    // 背景
    ImGui::GetBackgroundDrawList()->AddRect(
        {barStartX + shrink, barStartY},
        {barStartX + shrink + usableWidth, barStartY + barHeight},
        ImColor(0, 0, 0, 255),
        cornerRadius
    );

    // 填充
    if (载具油量 > 0)
    {
        float fillWidth = (载具油量 / 100.0f) * usableWidth;
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            {barStartX + shrink + 1, barStartY + 1},
            {barStartX + shrink + 1 + fillWidth, barStartY + barHeight - 1},
            ImColor(255, 255, 255, 255),
            cornerRadius - 1
        );
    }

    // ===== 耐久条（红色）=====
    // 背景
    ImGui::GetBackgroundDrawList()->AddRect(
        {barStartX + shrink, barStartY + barHeight + spacing},
        {barStartX + shrink + usableWidth, barStartY + barHeight * 2 + spacing},
        ImColor(0, 0, 0, 255),
        cornerRadius
    );

    // 填充
    if (载具耐久 > 0)
    {
        float fillWidth = (载具耐久 / 100.0f) * usableWidth;
        ImGui::GetBackgroundDrawList()->AddRectFilled(
            {barStartX + shrink + 1, barStartY + barHeight + spacing + 1},
            {barStartX + shrink + 1 + fillWidth, barStartY + barHeight * 2 + spacing - 1},
            ImColor(255, 0, 0, 255),
            cornerRadius - 1
        );
    }
}
}
}
}
                                                  
        }      
    }     
if (IsUpdateAvailable)
{
       ImGui::SetNextWindowPos(ImVec2(px - 300, py - 400), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 800), ImGuiCond_Always);

    ImGui::Begin("User Notice",
                 &Notice,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);

ImGui::TextWrapped("%s", updateurl.c_str());
ImGui::TextWrapped("%s", updateshow.c_str());

   
    g_timeout -= ImGui::GetIO().DeltaTime;
    if (g_timeout <= 0.0f)
        exit(0);

    ImGui::Spacing();
    ImGui::Text("Countdown: ");
    ImGui::PushItemWidth(-1);                             
float progress = 1.0f - g_timeout / 300.0f;//时间
progress = ImClamp(progress, 0.0f, 1.0f);
ImVec2 region  = ImGui::GetContentRegionAvail();
ImVec2 bar_size(region.x, 20.0f);
ImVec2 bar_min = ImGui::GetCursorScreenPos();
ImVec2 bar_max = ImVec2(bar_min.x + bar_size.x, bar_min.y + bar_size.y);
ImDrawList* draw = ImGui::GetWindowDrawList();
draw->AddRectFilled(bar_min, bar_max, IM_COL32(50, 50, 50, 255), 4.0f);
ImVec2 green_max = ImVec2(bar_min.x + bar_size.x * (1.0f - progress), bar_max.y);
draw->AddRectFilled(bar_min, green_max, IM_COL32(0, 200, 100, 255), 4.0f);
char buf[32];
snprintf(buf, sizeof(buf), "%.1f sec", g_timeout);
ImVec2 text_size = ImGui::CalcTextSize(buf);
ImVec2 text_pos  = ImVec2(
    bar_min.x + (bar_size.x - text_size.x) * 0.5f,
    bar_min.y + (bar_size.y - text_size.y) * 0.5f
);
draw->AddText(text_pos, IM_COL32(255, 255, 255, 255), buf);

ImGui::Dummy(bar_size);
    if (ImGui::Button("I Understand", ImVec2(-1, 60)))
    {
        exit(0);
    }
    ImGui::End();
}

// Dev debug window removed


        MaxPlayerCount = AimCount;
        
if (DrawIo[79]) {
    ImGui::PushFont(Font2);

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    // 1. Prepare the Count Numbers
    std::string pCountStr = std::to_string(PlayerCount);
    std::string rCountStr = std::to_string(RobotCount);
    
    float fontSize = 50.0f;
    ImVec2 pSize = Font2->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, pCountStr.c_str());
    ImVec2 rSize = Font2->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, rCountStr.c_str());
    
    // 2. Define dimensions for the pill shape
    float paddingX = 40.0f;
    float paddingY = 15.0f;
    float gap = 60.0f; // space between the two numbers
    
    float width = paddingX * 2 + pSize.x + gap + rSize.x;
    float height = paddingY * 2 + std::max(pSize.y, rSize.y);
    
    // Position at top center
    float topY = 60.0f;
    float startX = px - width / 2.0f;
    
    ImVec2 minPos(startX, topY);
    ImVec2 maxPos(startX + width, topY + height);
    
    // 3. Draw Pill Background and Border
    draw_list->AddRectFilled(minPos, maxPos, IM_COL32(0, 0, 0, 255), height / 2.0f);
    draw_list->AddRect(minPos, maxPos, IM_COL32(255, 255, 255, 255), height / 2.0f, ImDrawFlags_None, 4.0f);
    
    // 4. Draw PlayerCount (Red)
    ImVec2 pPos(startX + paddingX, topY + paddingY);
    draw_list->AddText(Font2, fontSize, pPos, IM_COL32(255, 40, 40, 255), pCountStr.c_str());
    
    // 5. Draw RobotCount (Green)
    ImVec2 rPos(startX + paddingX + pSize.x + gap, topY + paddingY);
    draw_list->AddText(Font2, fontSize, rPos, IM_COL32(40, 255, 40, 255), rCountStr.c_str());

    ImGui::PopFont();
    
    // Enemy counter is display-only; clicking it no longer toggles the menu.
    }
}





void ShowExpiryInfo() {
    if (ExpiryTime > 0) {
        // ZEUS KERNEL offline keys are lifetime. The auth layer stores a far-future
        // timestamp only for compatibility with older expiry checks.
        if (ExpiryTime == 4102444799L) {
            ImGui::TextWrapped("Key Expiration: LIFETIME");
            return;
        }

        // Ensure timestamp is in seconds
        std::time_t expireTime = ExpiryTime;
        
        // Check if timestamp is reasonable (between 2000-2100)
        if (expireTime > 946684800 && expireTime < 4102444800) {
            std::tm* tm = std::localtime(&expireTime);
            if (tm) {
                std::stringstream ss;
                ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
                ImGui::TextWrapped("Key Expiration: %s", ss.str().c_str());
            }
        }
    }
}

