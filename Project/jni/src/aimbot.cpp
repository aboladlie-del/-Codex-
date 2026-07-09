#include "main.h"
#include "offsets.h"
#include <random>
#include "include.h"
#include "Event.h"
// 遍历自瞄对象
std::mt19937 generator(static_cast<unsigned int>(time(0))); // 使用当前时间作为种子
std::uniform_int_distribution<int> distribution_offset(-60, 60); // 生成-80到80之间的随
static bool isBursting = false;      // 是否正在连点
static int lastAmmoCount = -1;
static bool isAmmoChanging = false;
int findminat()
// 初始化随机数生成器

{
 float min = NumIo[3];
 float DistanceMin = 400.0;
 int minAt = 999;
 for (int i = 0; i < MaxPlayerCount; i++)
 {
  switch ((int)NumIo[18])//瞄准模式
  {
case 0:
if (IsAimLongAim)
{
 if (strcmp(Aim[i].Name, AimName) == 0)
 {
  minAt = i;
 }
}
else
{
 if (Aim[i].ScreenDistance < min)
 {
  if (DrawIo[25])
  {
//持续锁定
strcpy(AimName, Aim[i].Name);
  }
  min = Aim[i].ScreenDistance;
  minAt = i;
 }
}
break;
case 1:
if (IsAimLongAim)
{
 if (strcmp(Aim[i].Name, AimName) == 0)
 {
  minAt = i;
 }
}
else
{
 if (Aim[i].WodDistance < DistanceMin)
 {
  if (DrawIo[25])
  {
strcpy(AimName, Aim[i].Name);
  }
  DistanceMin = Aim[i].WodDistance;
  minAt = i;
 }
}
break;
  }
 }
 if (minAt == 999)
 {
  Gmin = -1;
  IsAimLongAim = false;
  return -1;
 }
 Gmin = minAt;
 Aim[minAt].WodDistance;
 if (DrawIo[25])
 {
  IsAimLongAim = true;
 }
 return minAt;
}
float GetPitch(float Fov) 
{ 
 
 if (Fov > 75 && Fov <= 130) // 不开镜
 {
  
  return (0.7f/30)*g_WeaponPressureDB[MyWeapon];
 }
 else if (Fov == 70 || Fov == 75) // 机瞄 ，侧瞄
 {  
  
  return (1.8f/30)*g_WeaponPressureDB[MyWeapon]; 
 }
 else if (Fov == 55 || Fov == 60) // 红点 ，全息
 {
  
  return (1.8f/30)*g_WeaponPressureDB[MyWeapon];
 }
 else if ((int)Fov == 44) // 2倍
 {
  
  return (5.3f/30)*g_WeaponPressureDB[MyWeapon];
 }
 else if ((int)Fov == 26) // 3倍
 {
  
  return (7.2f/30)*g_WeaponPressureDB[MyWeapon];
 }
 else if ((int)Fov == 20) // 4倍
 {
  
  return (8.6f/30)*g_WeaponPressureDB[MyWeapon];
 }
 else if ((int)Fov == 13) // 6倍
 { 
  
  return (14.1f/30)*g_WeaponPressureDB[MyWeapon];
 }
 
	return (2.5f/30)*g_WeaponPressureDB[MyWeapon];// 8倍
}
float GetWeaponId(int WeaponId)
{
 switch (WeaponId)
 {
  //突击步枪
  case 101008:
  return 1.2;// M762
  break;
  case 101001:
  return 1.1;// AKM
  break;
  case 101004:
  return 0.8;// M416
  break;
  case 101003:
  return 0.8;// SCAR-L
  break;
  case 101002:
  return 0.85;// M16A4
  break;
  case 101009:
  return 1.05;// Mk47
  break;
  case 101006:
  return 0.75;// AUG
  break;
  case 101005:
  return 1.05;// Groza
  break;
  case 101010:
  return 1.15;// G36C
  break;
  case 101007:
  return 0.95;// QBZ
  break;
  case 101011:
  return 1.0;// AC-VAL
  break;
  case 101012:
  return 0.90;// 蜜獾突击步枪
  break;
  //机关枪
  case 105001:
  return 1.3;// M249
  break;
  case 105002:
  return 1.3;// DP-28
  case 105010:
  return 1.05;// MG3
  break;
  //冲锋枪
  case 102001:
  return 0.6;// UZI
  break;
  case 102003:
  return 0.6;// Vector
  break;
  case 100103:
  return 0.65;// PP-19
  break;
  case 102007:
  return 0.6;// MP5K
  break;
  case 102002:
  return 0.6;// UMP 5
  break;
  case 102004:
  return 0.6;// 汤姆逊
  break;
  case 102105:
  return 0.5;// P90
  break;
  case 102005:
  return 0.6;// 野牛
  break;
  default:
  return 0.95;// 未收录
  break;
 }
 return 1.0;// 未获取
}

float getScopeAcc(int Scope)
{
 switch (Scope)
 {
  case 0:
  return 1.0f;
  break;
  case 1:
  return .98f;
  break;
  case 2:
  return .95f;
  break;
  case 3:
  return .94f;
  break;
  case 4:
  return .92f;
  break;
  case 5:
  return .9f;
  break;
  case 6:
  return .88f;
  break;
  case 7:
  return .86f;
  break;
  case 8:
  return .85f;
  break;
  default:
  return .9f;
  break;
 }
}

float 狙击(int WeaponId)
{
 switch (WeaponId)
 {
 //狙击枪
 case 103001:
  return 1.0;
  break;
 case 103002:
  return 1.0;
  break;
 case 103003:
  return 1.0;
  break;
 case 103008:
  return 1.0;
  break;
 case 103011:
  return 1.0;
  break;
 case 103012:
  return 1.0;
  break;
 case 103015:
  return 1.0;
  break;
 case 103901:
  return 1.0;
  break;
 case 103902:
  return 1.0;
  break;
 case 103903:
  return 1.0;
  break;
 case 103904:
  return 1.0;
  break;
 case 103905:
  return 1.0;
  break;
 case 107986:
  return 1.0;
  break;
 case 107910:
  return 1.0;
  break;

 //适配地铁武器
 case 1030021:
  return 1.0;
  break;
 case 1030022:
  return 1.0;
  break;
 case 1030023:
  return 1.0;
  break;
 case 1030024:
  return 1.0;
  break;
 case 1030025:
  return 1.0;
  break;
 case 1030026:
  return 1.0;
  break;
 case 1030027:
  return 1.0;
  break;
 case 1030121:
  return 1.0;
  break;
 case 1030122:
  return 1.0;
  break;
 case 1030123:
  return 1.0;
  break;
 case 1030124:
  return 1.0;
  break;
 case 1030125:
  return 1.0;
  break;
 case 1030126:
  return 1.0;
  break;
 case 1030127:
  return 1.0;
  break;
 case 1030011:
  return 1.0;
  break;
 case 1030012:
  return 1.0;
  break;
 case 1030013:
  return 1.0;
  break;
 case 1030014:
  return 1.0;
  break;
 case 1030015:
  return 1.0;
  break;
 case 1031011:
  return 1.0;
  break;
 case 1031012:
  return 1.0;
  break;
 case 1031013:
  return 1.0;
  break;
 case 1031014:
  return 1.0;
  break;
 case 1031015:
  return 1.0;
  break;
 case 9814038:
  return 1.0;
  break;
 case 9814039:
  return 1.0;
  break;
 case 9814040:
  return 1.0;
  break;
 case 9814041:
  return 1.0;
  break;
 case 9814043:
  return 1.0;
  break;
 case 9814044:
  return 1.0;
  break;
 case 9814045:
  return 1.0;
  break;
 case 9814046:
  return 1.0;
  break;
 case 9814047:
  return 1.0;
  break;
 case 9814048:
  return 1.0;
  break;
 case 9814049:
  return 1.0;
  break;
 case 9814050:
  return 1.0;
  break;
 case 9814051:
  return 1.0;
  break;
 case 9814052:
  return 1.0;
  break;
  case 1030039:
  return 1.0;
  break;
  case 1030037:
  return 1.0;
  break;
  case 1030036:
  return 1.0;
  break;
  case 1030035:
  return 1.0;
  break;
  case 1030034:
  return 1.0;
  break;
  case 1030033:
  return 1.0;
  break;
  case 1030032:
  return 1.0;
  break;
 case 1030031:
  return 1.0;
  break;
 case 9814053:
  return 1.0;
  break;
 
 }
 return 0.0; // 不是狙击枪
}
//枪械类型分类
float 散弹枪(int WeaponId)
{
 switch (WeaponId)
 {
 case 104005:
  return 1.0;
  break;
 case 104001:
  return 1.0;
  break;
  case 103006:
  return 1.0;
  break;
 case 104002:
  return 1.0;
  break;
 case 104003:
  return 1.0;
  break;
 case 104004:
  return 1.0;
  break;
 case 104100:
  return 1.0;
  break;
 case 104101:
  return 1.0;
  break;
 case 104102:
  return 1.0;
  break;
 //适配地铁武器
 case 1040011:
  return 1.0;
  break;
 case 1040012:
  return 1.0;
  break;
 case 1040013:
  return 1.0;
  break;
 case 1040014:
  return 1.0;
  break;
 case 1040015:
  return 1.0;
  break;
 case 1040022:
  return 1.0;
  break;
 case 1040023:
  return 1.0;
  break;
 case 1040024:
  return 1.0;
  break;
 case 1040025:
  return 1.0;
  break;
 case 1040031:
  return 1.0;
  break;
 case 1040032:
  return 1.0;
  break;
 case 1040033:
  return 1.0;
  break;
 case 1040034:
  return 1.0;
  break;
 case 1040035:
  return 1.0;
  break;
 case 1040041:
  return 1.0;
  break;
 case 1040042:
  return 1.0;
  break;
 case 1040043:
  return 1.0;
  break;
 case 1040044:
  return 1.0;
  break;
 case 1040045:
  return 1.0;
  break;
 case 1041022:
  return 1.0;
  break;
 case 1041024:
  return 1.0;
  break;
 case 1041025:
  return 1.0;
  break;
 }
 return 0.0; // 不是喷子
}
#include <unordered_set>
#include <fstream>
#include <string>

float 连点配置(int WeaponId)
{
    static const std::unordered_set<int> shotgun_ids = []() -> std::unordered_set<int> {
        std::unordered_set<int> ids;
        std::ifstream file("burst_weapon_config.cfg");
        if (!file) return ids;

        std::string line;
        while (std::getline(file, line)) {
            // 去掉尾部 \r\n
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
                line.pop_back();

            // 跳过空行或 // 注释
            if (line.empty() || line.rfind("//", 0) == 0) continue;

            // 简单手动解析整型
            int id = 0;
            bool valid = true;
            for (char c : line) {
                if (std::isspace(static_cast<unsigned char>(c))) continue;
                if (c < '0' || c > '9') { valid = false; break; }
                id = id * 10 + (c - '0');
            }
            if (valid) ids.insert(id);
        }
        return ids;
    }();

    return shotgun_ids.count(WeaponId) ? 1.0f : 0.0f;
}
static Gyro *gyro = nullptr;
void AimBotAuto(ImDrawList *Draw)
// 自瞄线程
{
 bool isDown = false;
 // 是否按下触摸
 
 
 double tx = NumIo[5] + distribution_offset(generator);
 double ty = NumIo[6] + distribution_offset(generator);
 
 bool isDown9 = false;                    // 连点状态
    double tx9 = NumIo[48] + distribution_offset(generator); // 第二指X坐标
    double ty9 = NumIo[49] + distribution_offset(generator); // 第二指Y坐标

    double ScreenX = displayInfo.width, ScreenY = displayInfo.height;
    if(displayInfo.width<displayInfo.height){
        ScreenX = displayInfo.width, ScreenY = displayInfo.height;
    }else{
        ScreenX = displayInfo.height, ScreenY = displayInfo.width;
    }

 double ScrXH = ScreenX / 2.0f;
 // 一半屏幕X
 
 double ScrYH = ScreenY / 2.0f;
 // 一半屏幕X
 
 static float TargetX = 0;
 static float TargetY = 0;
 // 触摸目标位置
 Vector3A obj;
 float PushX;
 /* 压枪X */
 float PushY;
 /* 压枪Y */
 int 目标血量 = 100;
    string 目标名字;
    int 数率 = 0;
 float NowCoor[3];
 //Touch_Init();
 
 while (1)
 {
 // usleep(1000000 / 120);
  
  	ImGuiIO& iooi = ImGui::GetIO();
		
		if (DrawIo[21] && iooi.MouseDown[0] && iooi.MousePos.x <= NumIo[6] + NumIo[7] && iooi.MousePos.y <= py*2 - NumIo[5] + NumIo[7] && iooi.MousePos.x >= NumIo[6] - NumIo[7] && iooi.MousePos.y >= py*2 - NumIo[5] - NumIo[7])
        {           
        	usleep(30000);      
            if (DrawIo[21] && iooi.MouseDown[0] && iooi.MousePos.x <= NumIo[6] + NumIo[7] && iooi.MousePos.y <= py*2 - NumIo[5] + NumIo[7] && iooi.MousePos.x >= NumIo[6] - NumIo[7] && iooi.MousePos.y >= py*2 - NumIo[5] - NumIo[7])
        	{
           		while (DrawIo[21] && iooi.MouseDown[0] && iooi.MousePos.x <= NumIo[6] + NumIo[7] && iooi.MousePos.y <= py*2 - NumIo[5] + NumIo[7] && iooi.MousePos.x >= NumIo[6] - NumIo[7] && iooi.MousePos.y >= py*2 - NumIo[5] - NumIo[7])
                {			
    				NumIo[6] = iooi.MousePos.x;
                    NumIo[5] = py*2 - iooi.MousePos.y;
					TouchingColor = ImColor(0, 220, 0, 150);
    				usleep(500);           				
                }		
				TouchingColor = ImColor(255, 0, 0, 150);
            }
        }
if (DrawIo[56] /*连点*/&& iooi.MouseDown[0] &&
                iooi.MousePos.x <= NumIo[49]  + NumIo[50] && iooi.MousePos.y <= py*2 - NumIo[48] + NumIo[50] &&
                iooi.MousePos.x >= NumIo[49]  - NumIo[50] && iooi.MousePos.y >= py*2 - NumIo[48] - NumIo[50])
            {
                usleep(30000);
                while (DrawIo[56] && iooi.MouseDown[0] &&
                       iooi.MousePos.x <= NumIo[49]  + NumIo[50] && iooi.MousePos.y <= py*2 - NumIo[48] + NumIo[50] &&
                       iooi.MousePos.x >= NumIo[49]  - NumIo[50] && iooi.MousePos.y >= py*2 - NumIo[48] - NumIo[50])
                {
                    NumIo[49]  = iooi.MousePos.x;
                    NumIo[48] = py*2 - iooi.MousePos.y;
                    TouchingColor = ImColor(0, 220, 0, 150);
                    usleep(500);
                }
                TouchingColor = ImColor(255, 0, 0, 150);
            }
  
  if (!DrawIo[20])
  {
IsAimLongAim = false;
if (isDown == true)
{
 usleep(1000);
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 // 恢复变量
 Touch_Up(8);
 // 抬起
 isDown = false;
}
                    if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
usleep(NumIo[9] * 1500);
continue;
  }
  
  findminat();
  // 获取目标
  if (Gmin == -1)
  {
IsAimLongAim = false;
if (isDown == true)
{
 usleep(1000);
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 // 恢复变量
 Touch_Up(8);
 // 抬起
 isDown = false;
}
                    if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
usleep(NumIo[9] * 1500);
continue;
  }
  float ToReticleDistance = Aim[Gmin].ScreenDistance;
float FlyTime = Aim[Gmin].WodDistance / NumIo[11];
  float DropM = 540.0f * FlyTime * FlyTime;
  

  
  //函数留空
  int 开火判断;
  if (fov>75&& Aim[Gmin].WodDistance > NumIo[39]){
  开火判断 =  Firing;
  } else if(fov < 75){
  开火判断 =  Firing;
  }else{
  开火判断 = 0;
  }
NowCoor[0] = Aim[Gmin].ObjAim.X;
  NowCoor[1] = Aim[Gmin].ObjAim.Y;
  NowCoor[2] = Aim[Gmin].ObjAim.Z;
  obj.X = NowCoor[0] + (Aim[Gmin].AimMovement.X * FlyTime*NumIo[36]);
  obj.Y = NowCoor[1] + (Aim[Gmin].AimMovement.Y * FlyTime*NumIo[36]);
  obj.Z = NowCoor[2] + (Aim[Gmin].AimMovement.Z * FlyTime*NumIo[36] + DropM);

  if (自身动作 == 16 || 自身动作 == 1040 || 自身动作 == 1552 || 自身动作 == 1680 || 自身动作 == 19 || 自身动作 == 17 || 自身动作 == 657 || 自身动作 == 1681 || 自身动作 == 144 || 自身动作 == 5153 || 自身动作 == 35 || 自身动作 == 272 || 自身动作 == 656 || 自身动作 == 145 || 自身动作 == 1313)
            {
 obj.Z -= Aim[Gmin].WodDistance * (g_WeaponPressureDB[MyWeapon] * g_WeaponPressureDX[MyWeapon]) * GetWeaponId(MyWeapon);

}
else if (自身动作 == 672 || 自身动作 == 32 || 自身动作 == 544 || 自身动作 == 35|| 自身动作 == 33 || 自身动作 == 160 || 自身动作 == 288)
{
 obj.Z -= Aim[Gmin].WodDistance * (g_WeaponPressureDB[MyWeapon]* g_WeaponPressurePX[MyWeapon]) * GetWeaponId(MyWeapon);
}
else
{
 obj.Z -= Aim[Gmin].WodDistance * g_WeaponPressureDB[MyWeapon] * GetWeaponId(MyWeapon);//*0.1;
}
  
  
  
  float cameras = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
  
  Vector2A vpvp = WorldToScreen(obj,matrix,cameras);
  瞄准x = vpvp.X;
  瞄准y = vpvp.Y;
  zm_y = vpvp.X;
  zm_x = ScreenX - vpvp.Y;
  float AimDs = sqrt(pow(px - vpvp.X, 2) + pow(py - vpvp.Y, 2));
  
if (DrawIo[31] && DrawIo[20] && Aiming == 1 || Firing == 1 && Aiming == 257 || Firing == 1)
{

    fwjl = AimDs; // 如果条件满足，将 AimDs 赋值给 NumIo[3]
}
else
{

    fwjl = NumIo[3];
}
if (DrawIo[55] && 连点配置(MyWeapon) == 1.0) {
int 自身子弹 = driver->read<int>(driver->read<uintptr_t>(driver->read<uintptr_t>(oneself + OFFSET_ACTOR_WEAPON_MANAGER)+ OFFSET_WEAPON_CURRENT_REPLICATED)+OFFSET_WEAPON_AMMO_CURRENT);
    // 获取当前子弹数量
    if (lastAmmoCount == -1) {
        lastAmmoCount = 自身子弹; // 初始化
    }
    
    if (自身子弹 != lastAmmoCount) {
        // 子弹数量发生变化，开始连点
        if (!isAmmoChanging && 自身子弹 < lastAmmoCount) {
            // 只有子弹减少时才触发连点（开火时）
            isAmmoChanging = true;
        }
        lastAmmoCount = 自身子弹;
    }
    if (isAmmoChanging && 自身子弹 > 0) {
        auto now = std::chrono::steady_clock::now();
        
        tx9 = NumIo[48] + distribution_offset(generator);
        ty9 = NumIo[49] + distribution_offset(generator);

        if (!isDown9) {
            if (NumIo[10] == 0.0f)
                Touch_Down(9, (int)tx9, (int)ty9);
            else
                Touch_Down(9, displayInfo.height - (int)tx9, displayInfo.width - (int)ty9);
            isDown9 = true;
            usleep(NumIo[76] * 10);
        } else {
            Touch_Up(9);
            isDown9 = false;
            usleep(NumIo[76] * 10);
        }
        
    } 
    else if (isAmmoChanging && 自身子弹 == 0) {
        isAmmoChanging = false;
        lastAmmoCount = -1; // 重置以便下次重新检测
        if (isDown9) {
            Touch_Up(9);
            isDown9 = false;
        }
    }
} else {
    // 关闭 DrawIo[55] 或武器不匹配时重置所有状态
    isAmmoChanging = false;
    lastAmmoCount = -1;
    if (isDown9) {
        Touch_Up(9);
        isDown9 = false;
    }
}
  if (zm_x <= 0 || zm_x >= ScreenX || zm_y <= 0 || zm_y >= ScreenY)
  {
IsAimLongAim = false;
if (isDown == true)
{
 usleep(1000);
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 // 恢复变量
 Touch_Up(8);
 // 抬起
 isDown = false;
}
                    if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
usleep(NumIo[9] * 1500);
continue;
  }
  
  if ((DrawIo[26] && (px >= Aim[Gmin].boxLeft && px <= Aim[Gmin].boxRight && py >= Aim[Gmin].boxTop && py <= Aim[Gmin].boxBottom)) ||
    (!DrawIo[26] && (ToReticleDistance <= NumIo[3] || AimDs <= NumIo[3])))
{
switch ((int)NumIo[0])
{


  case 0://开火
 if (DrawIo[33] && 狙击(MyWeapon) == 1.0) { // 如果启用狙击自瞄
  if (Aiming == 0 || Aiming == 256) { // 如果开镜状态为0 
IsAimLongAim = false;
 if (isDown == true) {
 usleep(1000);
 // 应用随机偏移量
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);
// 恢复变量 
Touch_Up(8);
 // 抬起
 isDown = false;
  }
                      if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
  usleep(NumIo[9] * 1500);
  continue;
 }
 break;
 
  }else
   if (DrawIo[38] && 散弹枪(MyWeapon) == 1.0 && Aim[Gmin].WodDistance <= NumIo[68]){
IsAimLongAim = false;
break;

}else if (开火判断 != 1) {// 如果未开火
 
  IsAimLongAim = false;
if (isDown == true) {
usleep(1000);

 // 应用随机偏移量
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 
 // 恢复变量 
 Touch_Up(8);
 // 抬起
 isDown = false;
}
                    if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
usleep(NumIo[9] * 1500);
  continue;

}else{
 break;
  }
  
  
  
  
 


 case 1:
 if (Aiming != 1)
 {
  IsAimLongAim = false;
  if (isDown == true)
  {
usleep(1000);
tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

// 恢复变量
Touch_Up(8);
isDown = false;
  }
                      if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
  usleep(NumIo[9] * 1500);
  continue;
 }
 break;
 case 2:
 if (开火判断 == 0 && Aiming == 256 || 开火判断 == 0 && Aiming == 0)
 {
  IsAimLongAim = false;
  if (isDown == true)
  {
usleep(1000);
tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

// 恢复变量
Touch_Up(8);
isDown = false;
  }
                      if (isDown9) {
                        Touch_Up(9);
                        isDown9 = false;
                    }
  usleep(NumIo[9] * 1500);
  continue;
 }
 break;
 case 3:
 IsAimLongAim = true;  // 设置为锁定状态
 if (isDown == true)
 {
  usleep(1000);
  tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

  // 恢复变量
  Touch_Up(8);
  isDown = false;
 }
 usleep(NumIo[9] * 1500);
 break;
}
           if (DrawIo[37]) {
          if (数率 > 0) {
            数率++;
            if (数率 == (int)NumIo[21]) {
              数率 = 0;
            }
          }else {
            if (Aim[Gmin].名字 == 目标名字) {
              if (Aim[Gmin].血量 >= 目标血量) {
                目标血量 = Aim[Gmin].血量;
                continue;
              }else {
                数率++;
                目标血量 = Aim[Gmin].血量;
              }
            }else {
              目标名字 = Aim[Gmin].名字;
              目标血量 = Aim[Gmin].血量;
              continue;
            }
          }
        }

float Acc = getScopeAcc((int)(90 / fov));

if (isDown == false)
{
 usleep(1000);
 if (NumIo[10] == 0.0f)
 Touch_Down(8,(int)tx, (int)ty);
 else
 Touch_Down(8,displayInfo.height - (int)tx, displayInfo.width-(int)ty);
 
 isDown = true;
 usleep(1000);
}
if (zm_x > ScrXH)
{
 TargetX = -(ScrXH - zm_x) / NumIo[16] * Acc;
 if (TargetX + ScrXH > ScrXH * 2)
 {
  TargetX = 0;
 }
}
else if (zm_x < ScrXH)
{
 TargetX = (zm_x - ScrXH) / NumIo[16] * Acc;
 if (TargetX + ScrXH < 0)
 {
  TargetX = 0;
 }
}

if (zm_y > ScrYH)
{
 TargetY = -(ScrYH - zm_y) / NumIo[16] * Acc;
 if (TargetY + ScrYH > ScrYH * 2)
 {
  TargetY = 0;
 }
}
else if (zm_y < ScrYH)
{
 TargetY = (zm_y - ScrYH) / NumIo[16] * Acc;
 if (TargetY + ScrYH < 0)
 {
  TargetY = 0;
 }
}
static float smoothAccumulateX = 0, smoothAccumulateY = 0;  // 亚像素累积

// 累积小数部分
smoothAccumulateX += TargetX;
smoothAccumulateY += TargetY;

// 当累积超过0.5像素时才实际移动
int moveX = 0, moveY = 0;
if (fabs(smoothAccumulateX) >= 0.5f) {
    moveX = (int)smoothAccumulateX;  // 取整数部分
    smoothAccumulateX -= moveX;      // 减去整数部分，保留小数
}

if (fabs(smoothAccumulateY) >= 0.5f) {
    moveY = (int)smoothAccumulateY;
    smoothAccumulateY -= moveY;
}

// 4. 应用移动（只有在有实际像素移动时才更新）
if (moveX != 0 || moveY != 0) {
    tx += moveX;
    ty += moveY;
}
		
		
if (tx >= NumIo[5] + NumIo[7] || tx <= NumIo[5] - NumIo[7]
|| ty >= NumIo[6] + NumIo[7] || ty <= NumIo[6] - NumIo[7])
{
 usleep(1000);
 // 只要滑屏达到了边界，直接还原至中心
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 // 恢复变量
 Touch_Up(8);
 // 抬起
 usleep(3000);
 // 延迟
 if (!NumIo[10])
 Touch_Down(8,(int)tx, (int)ty);
 else
 Touch_Down(8,displayInfo.height - (int)tx, displayInfo.width - (int)ty);
 // 按下
 smoothAccumulateX = smoothAccumulateY = 0;
 
 usleep(1000);
}

if (!NumIo[10])
Touch_Move(8,(int)tx, (int)ty);
else
Touch_Move(8,displayInfo.height - (int)tx, displayInfo.width - (int)ty);

isDown = true;

usleep(NumIo[9] * 1500);
  }
  else
  {
IsAimLongAim = false;
if (isDown)
{
 tx = NumIo[5] + distribution_offset(generator);
 ty = NumIo[6] + distribution_offset(generator);

 // 恢复变量
 Touch_Up(8);
 // 抬起
 isDown = false;
 // 延时
 usleep(NumIo[9] * 1500);
}
            if (isDown9) {
                Touch_Up(9);
                isDown9 = false;
            }
  }

 }
}

void AimBot(ImDrawList *Draw)
// 自瞄线程
{
    bool isDown = false;
    // 是否按下触摸
    gyro = new Gyro;
   
  

    double ScreenX = displayInfo.width > displayInfo.height ? displayInfo.height : displayInfo.width;
    double ScreenY = displayInfo.width > displayInfo.height ? displayInfo.width : displayInfo.height;
    // 分辨率(竖屏)PS:滑屏用的坐标是竖屏状态下的

    double ScrXH = ScreenX / 2.0f;
    // 一半屏幕X

    double ScrYH = ScreenY / 2.0f;
    // 一半屏幕X

    static float TargetX = 0;
    static float TargetY = 0;
    // 触摸目标位置
    Vector3A obj;
    float PushX;
    /* 压枪X */
    float PushY;
    /* 压枪Y */
int 目标血量 = 100;
    string 目标名字;
    int 数率 = 0;
    float NowCoor[3];
    //   Touch_Init();
    while (1)
    {
        if (!DrawIo[20])
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }
        findminat();
        // 获取目标
        if (Gmin == -1)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        float ToReticleDistance = Aim[Gmin].ScreenDistance;
        float FlyTime = Aim[Gmin].WodDistance / NumIo[11];
        float DropM = 540.0f * FlyTime * FlyTime;
int 开火判断;
  if (fov>75&& Aim[Gmin].WodDistance > NumIo[39]){
  开火判断 =  Firing;
  } else if(fov < 75){
  开火判断 =  Firing;
  }else{
  开火判断 = 0;
  }
  

       NowCoor[0] = Aim[Gmin].ObjAim.X;
        NowCoor[1] = Aim[Gmin].ObjAim.Y;
        NowCoor[2] = Aim[Gmin].ObjAim.Z;
obj.X = NowCoor[0] + (Aim[Gmin].AimMovement.X * FlyTime*NumIo[36]);
  obj.Y = NowCoor[1] + (Aim[Gmin].AimMovement.Y * FlyTime*NumIo[36]);
  obj.Z = NowCoor[2] + (Aim[Gmin].AimMovement.Z * FlyTime*NumIo[36] + DropM);

  if (自身动作 == 16 || 自身动作 == 1040 || 自身动作 == 1552 || 自身动作 == 1680 || 自身动作 == 19 || 自身动作 == 17 || 自身动作 == 657 || 自身动作 == 1681 || 自身动作 == 144 || 自身动作 == 5153 || 自身动作 == 35 || 自身动作 == 272 || 自身动作 == 656 || 自身动作 == 145 || 自身动作 == 1313)
            {
 obj.Z -= Aim[Gmin].WodDistance * (g_WeaponPressureDB[MyWeapon] * g_WeaponPressureDX[MyWeapon]) * GetWeaponId(MyWeapon);

}
else if (自身动作 == 672 || 自身动作 == 32 || 自身动作 == 544 || 自身动作 == 35|| 自身动作 == 33 || 自身动作 == 160 || 自身动作 == 288)
{
 obj.Z -= Aim[Gmin].WodDistance * (g_WeaponPressureDB[MyWeapon]* g_WeaponPressurePX[MyWeapon]) * GetWeaponId(MyWeapon);
}
else
{
 obj.Z -= Aim[Gmin].WodDistance * g_WeaponPressureDB[MyWeapon] * GetWeaponId(MyWeapon);//*0.1;
}
  
        float cameras = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];

        Vector2A vpvp = WorldToScreen(obj, matrix, cameras);
        瞄准x = vpvp.X;
        瞄准y = vpvp.Y;
        zm_y = vpvp.X;
        zm_x = ScreenX - vpvp.Y;
        float AimDs = sqrt(pow(px - vpvp.X, 2) + pow(py - vpvp.Y, 2));
        

        if (zm_x <= 0 || zm_x >= ScreenX || zm_y <= 0 || zm_y >= ScreenY)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        if (ToReticleDistance <= NumIo[3] || AimDs <= NumIo[3])
        {
            switch ((int)NumIo[0])
            {
  case 0://开火
 if (DrawIo[33] && 狙击(MyWeapon) == 1.0) { // 如果启用狙击自瞄
  if (Aiming == 0 || Aiming == 256) { // 如果开镜状态为0 
IsAimLongAim = false;
  usleep(NumIo[9] * 1500);
  continue;
 }
 break;
  }else if (DrawIo[38] && 散弹枪(MyWeapon) == 1.0 && Aim[Gmin].WodDistance <= NumIo[68]){
IsAimLongAim = false;
break;

}else if (开火判断 != 1)
{
    IsAimLongAim = false;
    usleep(NumIo[9] * 1000);
    continue;
}else{
 break;
  }
  
            case 1:
                if (Aiming != 1)
                {
                    IsAimLongAim = false;
                    if (isDown == true)
                    {
                        usleep(1000);
                        usleep(NumIo[9] * 1000);
                        continue;
                    }
                    break;
                }
            case 2:
                if (开火判断 == 0 && Aiming == 256 || 开火判断 == 0 && Aiming == 0)
                {
                    IsAimLongAim = false;
                    usleep(NumIo[9] * 1000);
                    continue;
                }
                break;
            }
            if (DrawIo[37]) {
          if (数率 > 0) {
            数率++;
            if (数率 == (int)NumIo[21]) {
              数率 = 0;
            }
          }else {
            if (Aim[Gmin].名字 == 目标名字) {
              if (Aim[Gmin].血量 >= 目标血量) {
                目标血量 = Aim[Gmin].血量;
                continue;
              }else {
                数率++;
                目标血量 = Aim[Gmin].血量;
              }
            }else {
              目标名字 = Aim[Gmin].名字;
              目标血量 = Aim[Gmin].血量;
              continue;
            }
          }
        }
            float Acc = getScopeAcc((int)(90 / fov));
            if (zm_x > ScrXH)
            {
                TargetX = -(ScrXH - zm_x) / NumIo[16] * Acc;
                if (TargetX + ScrXH > ScrXH * 2)
                {
                    TargetX = 0;
                }
            }
            else if (zm_x < ScrXH)
            {
                TargetX = (zm_x - ScrXH) / NumIo[16] * Acc;
                if (TargetX + ScrXH < 0)
                {
                    TargetX = 0;
                }
            }
            if (zm_y > ScrYH)
            {
                TargetY = -(ScrYH - zm_y) / NumIo[16] * Acc;
                if (TargetY + ScrYH > ScrYH * 2)
                {
                    TargetY = 0;
                }
            }
            else if (zm_y < ScrYH)
            {
                TargetY = (zm_y - ScrYH) / NumIo[16] * Acc;
                if (TargetY + ScrYH < 0)
                {
                    TargetY = 0;
                }
            }

            if (TargetY >= 35 || TargetX >= 35 || TargetY <= -35 || TargetX <= -35)
            {
                usleep(NumIo[9] * 1000);
                continue;
            }

            float xxx = vpvp.X - (ScreenY / 2);
            float yyy = vpvp.Y - (ScreenX / 2);
            xxx *= Acc;
            yyy *= Acc;
            xxx *= NumIo[9];
            yyy *= NumIo[9];
            if (NumIo[10] == 1.0f)
            {
                xxx = -xxx;
                yyy = -yyy;
            }else {
            }
            if (gyro)
            {
                gyro->update(-(xxx * 0.002), (yyy * 0.002));
            }

            usleep(NumIo[9] * 1000);
        }
        else
        {
            IsAimLongAim = false;
        }
    }
}