#include <chrono>
#include <cstring>
#include "Other/CustomClassNameReader.h" // 确保包含自定义类的头文件
// 需要一个全局变量或静态变量来保存开始时间
std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::time_point::min();
bool GetHandInfo(char* gname,char** name);
bool GetItemInfo(char* gname,char** name,ImVec4* color);
bool GetGrenadeInfo(char* gname,char** name,ImVec4* color);
bool GetVehicleInfo(char* gname,char** name,ImVec4* color);    
bool showItem, showShotgun, showPistol, show556, show762, showRifle, showMirror, showExpansion, showDrug, showArmor, showSubmachine, showSniper, showOtherParts, showChest, showDrop,狗,宝箱,武器,自定义,钞票,信号枪,自救器;
struct Config {
bool showItem;
bool showShotgun;
bool showPistol;
bool show556;
bool show762;
bool showRifle;
bool showMirror;
bool showExpansion;
bool showDrug;
bool showArmor;
bool showSubmachine;
bool showSniper;
bool showOtherParts;
bool showChest;
bool showDrop;
bool 狗;
bool 宝箱;
bool 武器;
bool 自定义;
bool 钞票;
bool 信号枪;
bool 自救器;
}Config;

// ZEUS KERNEL Item ESP profile. The UI and renderer share this compact list so
// each item can be toggled independently without changing DrawIo/NumIo indexes.
struct ZeusItemESPDef {
    int item_id;
    const char* name;
    bool enabled;
    ImVec4 color;
    const char* category;
    int priority;
};

static ZeusItemESPDef g_ZeusItemESP[] = {
    {101001, "AKM",          false, ImVec4(1.00f, 0.08f, 0.08f, 1.0f), "Rifle",  90},
    {101004, "M416",         false, ImVec4(1.00f, 0.22f, 0.78f, 1.0f), "Rifle",  90},
    {103003, "AWM",          false, ImVec4(1.00f, 0.78f, 0.20f, 1.0f), "Sniper", 100},
    {203015, "8x",           false, ImVec4(0.22f, 0.70f, 1.00f, 1.0f), "Scope",  80},
    {601005, "Med Kit",      false, ImVec4(0.20f, 1.00f, 0.44f, 1.0f), "Heal",   75},
    {502003, "Helmet Lv3",   false, ImVec4(1.00f, 0.66f, 0.12f, 1.0f), "Armor",  85},
    {503003, "Armor Lv3",    false, ImVec4(1.00f, 0.50f, 0.08f, 1.0f), "Armor",  85},
};
static const int ZEUS_ITEM_ESP_COUNT = sizeof(g_ZeusItemESP) / sizeof(g_ZeusItemESP[0]);

static int ZeusFindItemESPIndex(const char* itemName) {
    if (!itemName || itemName[0] == '\0') return -1;
    for (int i = 0; i < ZEUS_ITEM_ESP_COUNT; ++i) {
        if (strcmp(itemName, g_ZeusItemESP[i].name) == 0) return i;
    }
    // Compatibility aliases used by some item-name tables.
    if (strcmp(itemName, "Level 3 Helmet") == 0) return ZeusFindItemESPIndex("Helmet Lv3");
    if (strcmp(itemName, "Level 3 Vest") == 0 || strcmp(itemName, "Vest Lv3") == 0) return ZeusFindItemESPIndex("Armor Lv3");
    if (strcmp(itemName, "8x Scope") == 0) return ZeusFindItemESPIndex("8x");
    if (strcmp(itemName, "MedKit") == 0) return ZeusFindItemESPIndex("Med Kit");
    return -1;
}

static bool ZeusAnyItemESPEnabled() {
    for (int i = 0; i < ZEUS_ITEM_ESP_COUNT; ++i) {
        if (g_ZeusItemESP[i].enabled) return true;
    }
    return false;
}

static void ZeusSyncItemESPConfig() {
    bool rifle = false, sniper = false, scope = false, heal = false, armor = false;
    for (int i = 0; i < ZEUS_ITEM_ESP_COUNT; ++i) {
        if (!g_ZeusItemESP[i].enabled) continue;
        const char* cat = g_ZeusItemESP[i].category;
        if (strcmp(cat, "Rifle") == 0) rifle = true;
        else if (strcmp(cat, "Sniper") == 0) sniper = true;
        else if (strcmp(cat, "Scope") == 0) scope = true;
        else if (strcmp(cat, "Heal") == 0) heal = true;
        else if (strcmp(cat, "Armor") == 0) armor = true;
    }
    Config.showRifle = rifle;
    Config.showSniper = sniper;
    Config.showMirror = scope;
    Config.showDrug = heal;
    Config.showArmor = armor;
    Config.showItem = ZeusAnyItemESPEnabled();
}

ImVec4 esp_color = ImVec4(255/255.f,0/255.f,0/255.f,255/255.f);
ImVec4 bot_color = ImVec4(1.f,1.f,1.f,1.f);
ImVec4 ray_color = ImVec4(1.f,1.f,1.f,1.f);
ImVec4 bone_color = ImVec4(0/255.f,255/255.f,0/255.f,255/255.f);
ImVec4 vehicle_color = ImVec4(0/255.f,255/255.f,255/255.f,255/255.f);
ImVec4 rifle_color = ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f);
ImVec4 submachine_color = ImVec4(255/255.f,110/255.f,180/255.f,255.f/255.f);
ImVec4 snipe_color = ImVec4(144/255.f,238/255.f,0/255.f,255.f/255.f);
ImVec4 box_color = ImVec4(255/255.f,192/255.f,203/255.f,255.f/255.f);
ImVec4 drop_color = ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f);
ImVec4 drug_color = ImVec4(255/255.f,105/255.f,180/255.f,255.f/255.f);
ImVec4 armor_color = ImVec4(255/255.f,215/255.f,0/255.f,255.f/255.f);
ImVec4 beibao_color = ImVec4(128.f/255.f, 0.f/255.f, 128.f/255.f, 255.f/255.f);
ImVec4 color_556 = ImVec4(0/255.f,255/255.f,0/255.f,255.f/255.f);
ImVec4 color_762 = ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f);
ImVec4 mirror_color = ImVec4{255.f/255.f,144.f/255.f,0.f/255.f,255.f/255.f};
ImVec4 otherparts_color = ImVec4{245.f/255.f,245.f/255.f,245.f/255.f,255.f/255.f};
ImVec4 expansion_color = ImVec4{208.f/255.f,128.f/255.f,114.f/255.f,255.f/255.f};
ImVec4 aim_color = ImVec4(255/255.f,0/255.f,0/255.f,255/255.f);

bool GetItemInfo(char* gname,char** name,ImVec4* color)
{
    if(Config.show556)
    {
        if(strcmp(gname,"BP_Ammo_556mm_Pickup_C")== 0)
        {
            *name="5.56MM";
            *color=color_556;
            return true;
        }
        if(strcmp(gname,"BP_Ammo_762mm_Pickup_C")== 0) 
        {
            *name = "7.62MM";
            *color = color_762;
            return true;
        }
    }
    if (Config.钞票){
        if (strstr(gname, "GoldenTokenWrapper_C") != 0)
        {
            *name="Cash";
          //  *color=armor_color;
            return true;           
        }
    }
 
    //显示步枪img
    if(Config.showRifle)
    {
        if(strcmp(gname,"BP_Rifle_AKM_Wrapper_C")==0)
        {
            *name="AKM";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_VAL_Wrapper_C")==0)
        {
            *name="VAL";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M416_Wrapper_C")==0)
        {
            *name="M416";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M16A4_Wrapper_C")==0)
        {
            *name="M16A4";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_SCAR_Wrapper_C")==0)
        {
            *name="SCAR";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_QBZ_Wrapper_C")==0)
        {
            *name="QBZ";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_G36_Wrapper_C")==0)
        {
            *name="G36C";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M762_Wrapper_C")==0)
        {
            *name="M762";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_Groza_Wrapper_C")==0)
        {
            *name="Groza";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_AUG_Wrapper_C")==0)
        {
            *name="AUG";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_DP28_Wrapper_C")==0)
        {
            *name="DP28";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_M249_Wrapper_C")==0)
        {
            *name="M249";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_MG3_Wrapper_C")==0)
        {
            *name="MG3";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_CrossBow_Wrapper_C")==0)
        {
            *name="CrossBow";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_QBU_Wrapper_C")==0)
        {
            *name="QBU";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_VSS_Wrapper_C")==0)
        {
            *name="VSS";
            *color=snipe_color;
            return true;
        }
    }
    //显示冲锋枪
    if(Config.showSubmachine)
    {
        if(strcmp(gname,"BP_MachineGun_UMP9_Wrapper_C")==0)
        {
            *name="UMP45";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_P90CG17_Wrapper_C")==0)
        {
            *name="P90CG17";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_TommyGun_Wrapper_C")==0)
        {
            *name="Thompson";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_PP19_Wrapper_C")==0)
        {
            *name="PP19";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Uzi_Wrapper_C")==0)
        {
            *name="Uzi";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Vector_Wrapper_C")==0)
        {
            *name="Vector";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_MP5K_Wrapper_C")==0)
        {
            *name="MP5K";
            *color=snipe_color;
            return true;
        }
        
    }
    //显示狙击枪
    if(Config.showSniper)
    {
        if(strcmp(gname,"BP_Sniper_Kar98k_Wrapper_C")==0)
        {
            *name="Kar98k";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Win94_Wrapper_C")==0)
        {
            *name="WIN94";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mini14_Wrapper_C")==0)
        {
            *name="Mini14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_SKS_Wrapper_C")==0)
        {
            *name="SKS";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_M24_Wrapper_C")==0)
        {
            *name="M24";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_rifle_Mk47_Wrapper_C")==0)
        {
            *name="Mk47";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_WEP_Mk14_Pickup_C")==0)
        {
            *name="MK14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AWM_Wrapper_C")==0)
        {
            *name="AWM";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mosin_Wrapper_C")==0)
        {
            *name="Mosin";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AMR_Wrapper_C")==0)
        {
            *name="AMR";
            *color=snipe_color;
            return true;
        }
        
        if(strcmp(gname,"BP_Sniper_SLR_Wrapper_C")==0)
        {
            *name="SLR";
            *color=snipe_color;
            return true;
        }
    }
    //霰弹枪
    if(Config.showShotgun)
    {
        if(strcmp(gname,"BP_ShotGun_DP12_Wrapper_C")==0)
        {
            *name="DP12";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S686_Wrapper_C")==0)
        {
            *name="S686";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SawedOff_Wrapper_C")==0)
        {
            *name="SawedOff";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S1897_Wrapper_C")==0)
        {
            *name="S1897";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S12K_Wrapper_C")==0)
        {
            *name="S12K";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SPAS-12_Wrapper_C")==0)
        {
            *name="SPAS-12";
            *color=snipe_color;
            return true;
        }
    }
    //手枪
    if(Config.showPistol)
    {
        if(strcmp(gname,"BP_Pistol_P18C_Wrapper_C")==0)
        {
            *name="P18C";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R1895_Wrapper_C")==0)
        {
            *name="R1895";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P1911_Wrapper_C")==0)
        {
            *name="P1911";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P92_Wrapper_C")==0)
        {
            *name="P92";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R45_Wrapper_C")==0)
        {
            *name="R45";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_Vz61_Wrapper_C")==0)
        {
            *name="Vz61";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_DesertEagle_Wrapper_C")==0)
        {
            *name="DesertEagle";
            *color=snipe_color;
            return true;
        }
    }
    //倍镜
    if(Config.showMirror)
    {
        if(strcmp(gname,"BP_MZJ_8X_Pickup_C")==0)
        {
            *name="8x";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_6X_Pickup_C")==0)
        {
            *name="6x";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_4X_Pickup_C")==0)
        {
            *name="4x";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_3X_Pickup_C")==0)
        {
            *name="3x";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_2X_Pickup_C")==0)
        {
            *name="2x";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_HD_Pickup_C")==0)
        {
            *name="Red Dot";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_QX_Pickup_C")==0)
        {
            *name="Holographic";
            *color=otherparts_color;
            return true;
        }
    }
    //扩容
    if(Config.showExpansion)
    {
        if(strcmp(gname,"BP_DJ_Large_EQ_Pickup_C")==0)
        {
            *name="(Rifle) Quick Extended Magazine";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Large_E_Pickup_C")==0)
        {
            *name="(Rifle) Extended Magnitude";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_EQ_Pickup_C")==0)
        {
            *name="SR Quickdraw Ext. Mag";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_E_Pickup_C")==0)
        {
            *name="SR Ext. Mag";
            *color=mirror_color;
            return true;
        }
    }
    //其他配件
    if(Config.showOtherParts)
    {
        if(strcmp(gname,"BP_QK_Large_Suppressor_Pickup_C")==0)
        {
            *name="AR Suppressor";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Suppressor_Pickup_C")==0)
        {
            *name="SR Suppressor";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_Sniper_Pickup_C")==0)
        {
            *name="Cheek Pad";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_ZDD_Sniper_Pickup_C")==0)
        {
            *name="SR Bullet Loop";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Large_Compensator_Pickup_C")==0)
        {
            *name="AR Compensator";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Compensator_Pickup_C")==0)
        {
            *name="SR Compensator";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Vertical_Pickup_C")==0)
        {
            *name="Vertical Grip";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_A_Pickup_C")==0)
        {
            *name="Stock";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Angled_Pickup_C")==0)
        {
            *name="Angled Grip";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_ThumbGrip_Pickup_C")==0)
        {
            *name="Thumb Grip";
            *color=otherparts_color;
            return true;
        }
    }
    //药品显示
    if(Config.showDrug)
    {
        if(strcmp(gname,"Injection_Pickup_C")==0)
        {
            *name="Adrenaline Syringe";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Firstaid_Pickup_C")==0)
        {
            *name="First Aid Kit";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"FirstAidbox_Pickup_C")==0)
        {
            *name="Med Kit";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Pills_Pickup_C")==0)
        {
            *name="Painkiller";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Drink_Pickup_C")==0)
        {
            *name="Energy Drink";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Bandage_Pickup_C")==0)
        {
            *name="Bandage";
            *color=drug_color;
            return true;
        }
    }
    //甲和头盔
    if(Config.showArmor)
    {
        if(strcmp(gname,"PickUp_BP_Helmet_Lv3_C")==0)
        {
            *name="Helmet Lv3";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv3_C")==0)
        {
            *name="Armor Lv3";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv3_B_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_A_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_C")==0 )
        {
            *name="Backpack Lv3";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Helmet_Lv2_C")==0)
        {
            *name="Helmet Lv2";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv2_C")==0)
        {
            *name="Armor Lv2";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv2_C")==0)
        {
            *name="Backpack Lv2";
            *color=armor_color;
            return true;
        }
    }  
    if (Config.showChest){
        if (strstr(gname, "PickUpListWrapperActor") != 0)
        {
            *name="Crate";
            *color=beibao_color;
            return true;           // 盒子
        }
        if (strstr(gname, "PlayerDeadBox") != 0)
        {
            *name="Crate";
            *color=beibao_color;
            return true;           // 盒子
        }
       if (strstr(gname, "XTPlayerDeadInventoryBox_C") != 0)
        {
            *name="Crate";
            *color=beibao_color;
            return true;           // 盒子
        }
        if (strstr(gname, "AirDropListWrapperActor") != 0)
        {
            *name="Air Drop";
            *color=beibao_color;
            return true;           
        }
        
    }
    if(Config.信号枪){
    if (strstr(gname, "BP_Pistol_Flaregun_Wrapper_C") != 0)
        {
            *name="Flare Gun";
          //  *color=armor_color;
            return true;           
        }
        if (strstr(gname, "BP_Ammo_Flare_Pickup_C") != 0)
        {
            *name="Flare Ammo";
           // *color=armor_color;
            return true;           
        }
        if (strstr(gname, "BP_Pistol_Flaregun_C") != 0)
        {
            *name="Flare Gun";
           // *color=armor_color;
            return true;           
        }
    }
    if(Config.自救器)
    {
    if (strstr(gname, "BP_revivalAED_Pickup_C") != 0)
        {
            *name="Self AED";
          //  *color=armor_color;
            return true;           
        }
    }
    if (Config.宝箱){



    
        
     //ALOGD("Class: %s",ClassName);        
    if (strstr(gname, "ClassicStore") != nullptr)//敌人盒子
    {
        *name = "Black Market";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv1_C") != nullptr)//敌人盒子
    {
        *name = "1 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv2_C") != nullptr)//敌人盒子
    {
        *name = "2 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BP_DC_HighLevel_TE_ItemBox_Lua_Lv3_2_C") != nullptr)//敌人盒子
    {
        *name = "4 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BP_3705andChest_C") != nullptr)//敌人盒子
    {
        *name = "Sky Island Supply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BP_3705andChest_TplanD_High_C") != nullptr)//敌人盒子
    {
        *name = "Sky Island Supply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv3_C") != nullptr)//敌人盒子
    {
        *name = "3 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv4_C") != nullptr)//敌人盒子
    {
        *name = "4 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv5_C") != nullptr)//敌人盒子
    {
        *name = "5 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ItemBox_Lua_Lv6_C") != nullptr)//敌人盒子
    {
        *name = "6 LvSupply Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "AIVH_Leopard_1A3_C") != nullptr)//敌人盒子
    {
        *name = "Low Level Tank Boss";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "ClassicStore") != nullptr)//敌人盒子
    {
        *name = "High Level Tank Boss";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "TreasureChestPickUpListWrapper_C"))
        {
         *name = "                 Enabled";
         return true;
        }
    }
    if (Config.自定义){
    #include "Other/CustomClassNameReader.h" // 确保包含自定义类的头文件

CustomClassNameReader customReader("ClassNames.txt");

// 在你的逻辑中调用
if (customReader.GetCustomClassName(gname, name, color)) {
    // 自定义类名匹配成功，使用返回的 name 和 color
    return true;
} else {
    // 没有匹配到自定义类名，继续其他逻辑
}
    }
    if (Config.武器){
    if (strstr(gname, "WeaponBox_Lua_Lv1_C") != nullptr)//敌人盒子
    {
        *name = "1 LvWeapon Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "WeaponBox_Lua_Lv2_C") != nullptr)//敌人盒子
    {
        *name = "2 LvWeapon Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "WeaponBox_Lua_Lv3_C") != nullptr)//敌人盒子
    {
        *name = "3 LvWeapon Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "WeaponBox_Lua_Lv4_C") != nullptr)//敌人盒子
    {
        *name = "4 LvWeapon Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "WeaponBox_Lua_Lv5_C") != nullptr)//敌人盒子
    {
        *name = "5 LvWeapon Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBox_Lua_Lv1_C") != nullptr)//敌人盒子
    {
        *name = "1 LvLetter Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBox_Lua_Lv2_C") != nullptr)//敌人盒子
    {
        *name = "2 LvLetter Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBox_Lua_Lv3_C") != nullptr)//敌人盒子
    {
        *name = "3 LvLetter Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBox_Lua_Lv4_C") != nullptr)//敌人盒子
    {
        *name = "4 LvLetter Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBox_Lua_Lv5_C") != nullptr)//敌人盒子
    {
        *name = "5 LvLetter Crate";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBag_Lua_Lv1_C") != nullptr)//敌人盒子
    {
        *name = "Handbag";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBag_Lua_Lv2_C") != nullptr)//敌人盒子
    {
        *name = "Handbag";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBag_Lua_Lv3_C") != nullptr)//敌人盒子
    {
        *name = "Handbag";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBag_Lua_Lv4_C") != nullptr)//敌人盒子
    {
        *name = "Handbag";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "FileBag_Lua_Lv5_C") != nullptr)//敌人盒子
    {
        *name = "Handbag";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "EggBox_Lua_Lv1_C") != nullptr)//敌人盒子
    {
        *name = "Hidden Spot";
        //*color = redcolor;
        return true;
    }
    }
    //ALOGD("Class: %s",ClassName);        
     if (Config.狗){
     if(strstr(gname,"TrackVehicle_BP_C")!= nullptr)
    {
        *name="Metro Vehicle";
        *color=vehicle_color;
        return true;
    }
    if (strstr(gname, "AIMob_PatrolDog_C") != nullptr)//敌人盒子
    {
        *name = "Robot Dog";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_HungerB_C") != nullptr)//敌人盒子
    {
        *name = "Dog";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_Library_C") != nullptr)//敌人盒子
    {
        *name = "Admin";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_HungerH_C") != nullptr)//敌人盒子
    {
        *name = "Dog";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_Watcher_C") != nullptr)//敌人盒子
    {
        *name = "Dog";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_VenomVariant_C") != nullptr)//敌人盒子
    {
        *name = "Venom Mutant";
        //*color = redcolor;
        return true;
    }
    if (strstr(gname, "BPPawn_BurningVariant_C") != nullptr)//敌人盒子
    {
        *name = "Burning Mutant";
        //*color = redcolor;
        return true;
    }
    }
    return false;
}




// 手雷预警
bool GetGrenadeInfo(char* gname,char** name,ImVec4* color)
{
if(strcmp(gname,"BP_Grenade_Smoke_C")==0)
    {
        *name="Smoke Grenade!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"BP_Projectile_SmokeBomb_C")==0)
    {
        *name="Smoke Grenade!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"BP_Grenade_Burn_C")==0)
    {
        *name="Molotov!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"BP_Projectile_BurnGrenade_C")==0)
    {
        *name="Molotov!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
if(strcmp(gname,"BP_Grenade_Shoulei_C")==0)
    {
        *name="Frag Grenade!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"BP_Projectile_FragGrenade_C")==0)
    {
        *name="Frag Grenade!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    return false;
}
// 获取载具
bool GetVehicleInfo(char* gname,char** name,ImVec4* color)
{
    if(strcmp(gname,"VH_BRDM_C")==0)
    {
        *name="BRDM";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Horse_C")==0||strcmp(gname,"VH_Horse_1_C")==0||strcmp(gname,"VH_Horse_2_C")==0||strcmp(gname,"VH_Horse_3_C")==0||strcmp(gname,"VH_Horse_4_C")==0)
    {
        *name="Horse";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Scooter_C")==0)
    {
        *name="Scooter";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Motorcycle_C")==0||strcmp(gname,"VH_Motorcycle_1_C")==0)
    {
        *name="Motorcycle";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_MotorcycleCart_1_C")==0||strcmp(gname,"VH_MotorcycleCart_C")==0||strstr(gname,"VH_MotorcycleCart_C_1_Pooled")!= nullptr)
    {
        *name="Motorcycle Cart";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Snowmobile_C")==0)
    {
        *name="Snowmobile";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"BP_VH_Tuk_C")==0||strcmp(gname,"BP_VH_Tuk_1_C")==0)
    {
        *name="Motorcycle Cart";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Buggy")!= nullptr||strstr(gname,"BP_VH_Buggy_C")!= nullptr||strstr(gname,"BP_VH_Buggy_2_C")!= nullptr||strstr(gname,"BP_VH_Buggy_3_C")!= nullptr||strstr(gname,"BP_VH_Buggy_C_1_Pooled")!= nullptr)
    {
        *name="Buggy";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Dacia")!= nullptr||strstr(gname,"VH_Dacia_3_New_C")!= nullptr||strstr(gname,"VH_Dacia_C")!= nullptr||strstr(gname,"VH_Dacia_C_1_Pooled")!= nullptr||strstr(gname,"VH_Dacia_2_C")!= nullptr||strstr(gname,"VH_Dacia_2_C_1_Pooled")!= nullptr||strstr(gname,"VH_Dacia_3_C")!= nullptr||strstr(gname,"VH_Dacia_4_C")!= nullptr||strstr(gname,"VH_Dacia_4_C_1_Pooled")!= nullptr||strstr(gname,"VH_Dacia_3_C_1_Pooled")!= nullptr)
    {
        *name="Dacia";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"UAZ")!= nullptr||strcmp(gname,"VH_UAZ01_C")==0||strcmp(gname,"VH_UAZ01_C_1_Pooled")==0||strcmp(gname,"VH_UAZ03_C_1_Pooled")==0||strcmp(gname,"VH_UAZ03_C")==0)
    {
        *name="UAZ";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"_PickUp")!= nullptr)
    {
        *name="Pickup";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"PickUp_07_C")!= nullptr)
    {
        *name="Pickup Offroad";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_Motorglider_C")!= nullptr)
    {
        *name="Motor Glider";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_UTV_C")!= nullptr)
    {
        *name="UTV";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"BP_VH_Bigfoot_C")!= nullptr)
    {
        *name="Monster Truck";
        *color=vehicle_color;
        return true;
    }

    if(strstr(gname,"BP_CoupeRB_Base_C")!= nullptr||strstr(gname,"VH_CoupeRB_1_C")!= nullptr||strstr(gname,"VH_CoupeRB_1_C_1_Pooled")!= nullptr||strstr(gname,"BP_CoupeRB_Base_C")!= nullptr)
    {
        *name="Coupe RB";
        *color=vehicle_color;
        return true;
    }
        
    if(strstr(gname,"VH_ATV1_C")!= nullptr)
    {
        *name="ATV";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_Snowbike_C")!= nullptr)
    {
        *name="Snowbike";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Rony")!= nullptr||strstr(gname,"Rony_01_C")!= nullptr)
    {
        *name="Pickup";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Mirado")!= nullptr||strstr(gname,"Mirado_open_4_C")!= nullptr||strstr(gname,"Mirado_open_1_C")!= nullptr||strstr(gname,"Mirado_open_2_C")!= nullptr||strstr(gname,"Mirado_open_3_C")!= nullptr)
    {
        *name="Mirado";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"MiniBus")!= nullptr||strstr(gname,"VH_MiniBus_01_C")!= nullptr)
    {
        *name="Mini Bus";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"PG117")!= nullptr||strstr(gname,"VH_PG117_C")!= nullptr||strstr(gname,"VH_PG117_Sl_C")!= nullptr||strstr(gname,"VH_PG117_C_1_Pooled")!= nullptr||strstr(gname,"VH_AmphibiousBoat_C")!= nullptr||strstr(gname,"AquaRail_1_C")!= nullptr)
    {
        *name="Boat";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"AquaRail")!= nullptr)
    {
        *name="Aqua Rail";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"PickUp_07_C")!= nullptr)
    {
        *name="Pickup Offroad";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_Drift_001_New_C")!= nullptr)
    {
        *name="Rally Car";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_StoreBus_C")!= nullptr||strstr(gname,"VH_StoreBus_C_1_Pooled")!= nullptr)
    {
        *name="Store Bus";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_StationWagon_New_C")!= nullptr)
    {
        *name="Station Wagon";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_Picobus_C")!= nullptr)
    {
        *name="Pico Bus";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_Blanc_C")!= nullptr)
    {
        *name="Blanc SUV";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"VH_PenguinSledge_C")!= nullptr)
    {
        *name="Theme Vehicle";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"MonsterTruck")!= nullptr || strstr(gname,"Bigfoot")!= nullptr)
    {
        *name="Monster Truck";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Snowmobile")!= nullptr)
    {
        *name="Snowmobile";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Snowbike")!= nullptr)
    {
        *name="Snowbike";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Coupe")!= nullptr || strstr(gname,"CoupeRB")!= nullptr)
    {
        *name="Coupe RB";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Boat")!= nullptr || strstr(gname,"AquaRail")!= nullptr)
    {
        *name="Boat";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Motor")!= nullptr)
    {
        *name="Motorcycle";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname, "VH_") != nullptr || strstr(gname, "BP_VH_") != nullptr || strstr(gname, "Vehicle") != nullptr || strstr(gname, "Buggy") != nullptr || strstr(gname, "Dacia") != nullptr || strstr(gname, "UAZ") != nullptr)
    {
        *name="Vehicle";
        *color=vehicle_color;
        return true;
    }
    
    return false;
}