#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <fstream>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <iostream>
#include <fstream>
#include<iostream>
#include<ctime>
using namespace std;
#include "Other/Auth.h"
#include "Draw.h"
#include "VulkanUtils.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#define MAX_LENGTH 1024
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <main.h>
#include "main.h"
#include "draw.cpp"
int fbl; bool IsMenuOpen = true; bool IsShortcutEnabled = false;
bool g_IsLoggedIn = true;
long ExpiryTime = 0;
bool IsUpdateAvailable = false;
std::string updateurl = "";
std::string updateshow = "";
typedef struct {
    const void* data; // 图片数据的地址
    size_t size;      // 图片数据的大小
    void* id;         // 纹理ID的指针，类型改为 void*
} TextureInfo;

TextureInfo textures[] = {
   
};




// Keep original function name unchanged
void LoadMemoryImages() {
    // Can keep pre-loaded or change to on-demand loading
    for (size_t i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
       // LoadTextureFromMemory(textures[i].data, textures[i].size, (MyTextureData*)textures[i].id);
    }
}


int 按键次数 = 0;
int 数据() {
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    return count ? count : -1;
}

int 音量() {
    int EventCount = 数据();
    if (EventCount < 0) {
        printf("Input device not found\n");
        return -1;
    }

    int *fdArray = (int *)malloc(EventCount * sizeof(int));
    if (!fdArray) {
        perror("malloc");
        return -1;
    }

    for (int i = 0; i < EventCount; i++) {
        char temp[128];
        snprintf(temp, sizeof(temp), "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDWR | O_NONBLOCK);
        if (fdArray[i] < 0) {
            perror("open");
            free(fdArray);
            return -1;
        }
    }

    struct pollfd polls[EventCount];
    int count;

    for (int i = 0; i < EventCount; i++) {
        polls[i].fd = fdArray[i];
        polls[i].events = POLLIN;
    }

    const int LONG_PRESS_THRESHOLD = 2000; // 2秒
    int volumeUpPressed = 0;
    struct timespec pressTime;
    int isBall = 0; // 控制悬浮球的显示和隐藏

    while (1) {
        count = poll(polls, EventCount, 1000);
        if (count < 0) {
            perror("poll");
            break;
        }

        for (int i = 0; i < EventCount; i++) {
            if (polls[i].revents & POLLIN) {
                input_event ev;
                memset(&ev, 0, sizeof(ev));
                read(fdArray[i], &ev, sizeof(ev));
                if (ev.type == EV_KEY) {
                    if (ev.code == KEY_VOLUMEUP) {
                        if (ev.value == 1) { // 按下
                            volumeUpPressed = 1;
                            clock_gettime(CLOCK_MONOTONIC, &pressTime);
                        } else if (ev.value == 0) { // 释放
                            volumeUpPressed = 0;
                            struct timespec releaseTime;
                            clock_gettime(CLOCK_MONOTONIC, &releaseTime);
                            long long elapsed = (releaseTime.tv_sec - pressTime.tv_sec) * 1000LL + (releaseTime.tv_nsec - pressTime.tv_nsec) / 1000000LL;
                            if (elapsed < LONG_PRESS_THRESHOLD) {
                                if (!DrawIo[57]) {
                                    IsMenuOpen = true;
                                    IsBall = true; // Show ball
                                }
                            }
                        }
                    } else if (ev.code == KEY_VOLUMEDOWN && ev.value == 1) {
                        IsMenuOpen = false;
                        IsBall = false; // Hide ball
                        按键次数++;
                        IsShortcutEnabled = (按键次数 % 2 == 0);
                    }
                }
            }
        }

        if (volumeUpPressed) {
            struct timespec currentTime;
            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            long long elapsed = (currentTime.tv_sec - pressTime.tv_sec) * 1000LL + (currentTime.tv_nsec - pressTime.tv_nsec) / 1000000LL;
            if (elapsed >= LONG_PRESS_THRESHOLD) {
                
                for (int i = 0; i < EventCount; i++) {
                    close(fdArray[i]);
                }
                free(fdArray);
                exit(0);
            }
        }

        usleep(1000);
    }

    for (int i = 0; i < EventCount; i++) {
        close(fdArray[i]);
    }
    free(fdArray);
    return 0;
}





bool main_thread_flag = true;
bool IsAntiRecordEnabled = false;
MyTextureData AimbotOffTexture;
MyTextureData FloatingAimbotSwitch;

int abs_ScreenX = 0;
int abs_ScreenY = 0;


void getRoot(int argc, char **argv)
{
    if (getuid() != 0)
    {
        std::string cmd = "su -c ";
        for (int i = 0; i < argc; i++) {
            cmd += argv[i];
            if (i < argc - 1) cmd += " ";
        }
        system(cmd.c_str());
        exit(0); // Success re-exec
    }
}
void config()
{
    NumIo[83] = 1.0f;//触摸Y位置
    NumIo[49] = 660.0f;//触摸Y位置
    NumIo[48] = 900.0f;//触摸X位置
    NumIo[50] = 300.0f;//触摸范围    
    NumIo[96] = 500.0f;
	NumIo[36] = 1.4f;
	NumIo[98] = 50.0f;
	NumIo[68] = 50.0f;
    NumIo[45] = 100.0f;
    NumIos[13] = 0;
    NumIo[77] = 1.0f;
    NumIo[78] =1.0f;
    NumIo[1] = 180.0f; // Radar width compatibility
    NumIo[2] = 180.0f; // Radar height compatibility
    NumIo[53] = 180.0f; // Radar center X
    NumIo[54] = 260.0f; // Radar center Y
    NumIo[3] = 150.0f;//自瞄范围
    NumIo[4] = 1.4f;//压枪力度
    NumIo[5] = 670.0f;//触摸Y位置
    NumIo[6] = 1400.0f;//触摸X位置
    NumIo[7] = 300.0f;//触摸范围
    NumIo[8] = 2.0f;//自瞄部位
    NumIo[9] = 9.0f;//平滑力度
    NumIo[10] = 0.0f;//充电口位置
    NumIo[11] = 600.0f;//子弹速度
    NumIo[12] = 60;//帧率
    NumIo[14] = 800;//
    NumIo[15] = 200;//
    NumIo[16] = 15.0f;//自瞄速度
    NumIo[17] = 150.0f;//雷达大小
    NumIo[18] = 0.0f;//瞄准方式
    NumIo[30] = 300.0f;//自瞄速度
    NumIo[32] = 1.0f;//骨骼粗细
    NumIo[62] = 1.0f;
	NumIo[34] = 0.0f;
NumIo[41] = 0.0f;
NumIo[42] = 0.0f;
    NumIo[33] = 180.0f;//Item ESP max distance
NumIo[43] = 0.0f;
    DrawIo[90] = false;
    DrawIo[91] = false;
    DrawIo[104] = false;
    NumIo[46] = 18.0f;
	NumIo[35] = 150.0f;//骨骼最大显示距离

	
	
}
void NumIoSave(const char *name) // 保存配置
{
    if (numSave == nullptr)
    {
        string SaveFile = "/data";
        SaveFile += "/";
        SaveFile += name;
        numSave = fopen(SaveFile.c_str(), "wb+");
    }
    fseek(numSave, 0, SEEK_SET);

    // 保存现有数据
    fwrite(NumIo, sizeof(float) * 300, 1, numSave);
    fwrite(DrawIo, sizeof(bool) * 300, 1, numSave);
    fwrite(NumIos, sizeof(float) * 300, 1, numSave);
    fwrite(intIo, sizeof(int) * 10, 1, numSave);

    // 保存额外的布尔变量
    fwrite(&Config.showRifle, sizeof(bool), 1, numSave);
    fwrite(&Config.showSubmachine, sizeof(bool), 1, numSave);
    fwrite(&Config.showSniper, sizeof(bool), 1, numSave);
    fwrite(&Config.showShotgun, sizeof(bool), 1, numSave);
    fwrite(&Config.showPistol, sizeof(bool), 1, numSave);
    fwrite(&Config.宝箱, sizeof(bool), 1, numSave);
    fwrite(&Config.狗, sizeof(bool), 1, numSave);
    fwrite(&Config.武器, sizeof(bool), 1, numSave);
    fwrite(&Config.信号枪, sizeof(bool), 1, numSave);
    fwrite(&Config.自救器, sizeof(bool), 1, numSave);
    fwrite(&Config.钞票, sizeof(bool), 1, numSave);
    fwrite(&Config.showChest, sizeof(bool), 1, numSave);
    fwrite(&Config.showDrop, sizeof(bool), 1, numSave);
    fwrite(&Config.showDrug, sizeof(bool), 1, numSave);
    fwrite(&Config.showArmor, sizeof(bool), 1, numSave);
    fwrite(&Config.showMirror, sizeof(bool), 1, numSave);
    fwrite(&Config.showExpansion, sizeof(bool), 1, numSave);
    fwrite(&Config.showOtherParts, sizeof(bool), 1, numSave);
    fwrite(&Config.show556, sizeof(bool), 1, numSave);
    fwrite(&Config.show762, sizeof(bool), 1, numSave);
    fwrite(&IsAntiRecordEnabled, sizeof(bool), 1, numSave);


    // 保存颜色变量
    fwrite(&真人射线, sizeof(ImColor), 1, numSave); // 真人射线颜色
    fwrite(&人机射线, sizeof(ImColor), 1, numSave); // 人机射线颜色
    fwrite(&自瞄圈, sizeof(ImColor), 1, numSave); // 自瞄圈颜色
    fwrite(&十字架, sizeof(ImColor), 1, numSave); // 十字架颜色

    fflush(numSave);
    fsync(fileno(numSave));
}

void NumIoLoad(const char *name)//加载配置
{
    ColorInitialization(); // 队伍颜色初始化
    if (numSave == nullptr)
    {
        string SaveFile = "/data";
        SaveFile += "/";
        SaveFile += name;
        numSave = fopen(SaveFile.c_str(), "rb+");
    }
    if (numSave != nullptr)
    {
        fseek(numSave, 0, SEEK_SET);
        fread(NumIo, sizeof(float) * 300, 1, numSave);
        fread(DrawIo, sizeof(bool) * 300, 1, numSave);
        fread(NumIos, sizeof(float) * 300, 1, numSave);
        fread(intIo, sizeof(int) * 10, 1, numSave);

        // 加载额外的布尔变量
    fread(&Config.showRifle, sizeof(bool), 1, numSave);
    fread(&Config.showSubmachine, sizeof(bool), 1, numSave);
    fread(&Config.showSniper, sizeof(bool), 1, numSave);
    fread(&Config.showShotgun, sizeof(bool), 1, numSave);
    fread(&Config.showPistol, sizeof(bool), 1, numSave);
    fread(&Config.宝箱, sizeof(bool), 1, numSave);
    fread(&Config.狗, sizeof(bool), 1, numSave);
    fread(&Config.武器, sizeof(bool), 1, numSave);
    fread(&Config.信号枪, sizeof(bool), 1, numSave);
    fread(&Config.自救器, sizeof(bool), 1, numSave);
    fread(&Config.钞票, sizeof(bool), 1, numSave);
    fread(&Config.showChest, sizeof(bool), 1, numSave);
    fread(&Config.showDrop, sizeof(bool), 1, numSave);
    fread(&Config.showDrug, sizeof(bool), 1, numSave);
    fread(&Config.showArmor, sizeof(bool), 1, numSave);
    fread(&Config.showMirror, sizeof(bool), 1, numSave);
    fread(&Config.showExpansion, sizeof(bool), 1, numSave);
    fread(&Config.showOtherParts, sizeof(bool), 1, numSave);
    fread(&Config.show556, sizeof(bool), 1, numSave);
    fread(&Config.show762, sizeof(bool), 1, numSave);
        fread(&IsAntiRecordEnabled, sizeof(bool), 1, numSave);
        // 加载颜色变量
        fread(&真人射线, sizeof(ImColor), 1, numSave); // 真人射线颜色
        fread(&人机射线, sizeof(ImColor), 1, numSave); // 人机射线颜色
        fread(&自瞄圈, sizeof(ImColor), 1, numSave); // 自瞄圈颜色
        fread(&十字架, sizeof(ImColor), 1, numSave); // 十字架颜色
    }
    else
    {
        config();
    }
}


   








static const char* ZEUS_CRASH_LOG_PATH = "/storage/emulated/0/ZEUS_KERNEL_CRASH.log";
static const char* ZEUS_CRASH_LOG_FALLBACK = "/data/local/tmp/ZEUS_KERNEL_CRASH.log";

static void ZeusWriteCrashLine(const char* text) {
    int fd = open(ZEUS_CRASH_LOG_PATH, O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fd < 0) {
        fd = open(ZEUS_CRASH_LOG_FALLBACK, O_CREAT | O_WRONLY | O_APPEND, 0666);
    }
    if (fd >= 0) {
        write(fd, text, strlen(text));
        close(fd);
    }
}

static void ZeusCrashHandler(int sig, siginfo_t* info, void* ctx) {
    char buf[1024];
    time_t now = time(nullptr);
    snprintf(buf, sizeof(buf),
             "\n========== ZEUS KERNEL CRASH =========="
             "\nTime: %ld"
             "\nSignal: %d"
             "\nFaultAddress: %p"
             "\nPID: %d"
             "\nlibUE4: 0x%lx"
             "\nUWorld: 0x%lx"
             "\nGName: 0x%lx"
             "\nLastFPS: %d"
             "\n=======================================\n",
             (long)now, sig, info ? info->si_addr : nullptr, getpid(),
             (unsigned long)libUE4, (unsigned long)Uworld, (unsigned long)Gname, FPS);
    ZeusWriteCrashLine(buf);
    signal(sig, SIG_DFL);
    raise(sig);
}

static void InstallZeusCrashLogger() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = ZeusCrashHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGABRT, &sa, nullptr);
    sigaction(SIGBUS,  &sa, nullptr);
    sigaction(SIGILL,  &sa, nullptr);
    sigaction(SIGFPE,  &sa, nullptr);
    ZeusWriteCrashLine("ZEUS KERNEL crash logger installed.\n");
}

int main(int argc, char *argv[])
{
InstallZeusCrashLogger();
//启动();
   
// Using 48 characters width - the "Sweet Spot" for MT Manager
printf("\033[1;36m+----------------------------------------------+\033[0m\n");
printf("\033[1;36m| \033[1;37m             ZEUS KERNEL             \033[1;36m |\033[0m\n");
printf("\033[1;36m+----------------------------------------------+\033[0m\n");
printf("\033[1;32m[SYSTEM]\033[0m Initializing ZEUS KERNEL Driver...\n");
printf("\033[1;36m------------------------------------------------\033[0m\n");

// Your functionality follows here



    std::string targetPackage = (argc > 1) ? argv[1] : "";
    if (!targetPackage.empty()) {
        printf("\033[1;36m[SYSTEM]\033[0m Target Package: %s\n", targetPackage.c_str());
    }

const char *liandian = "burst_weapon_config.cfg";
        FILE *file666 = fopen(liandian, "r");
        if (file666 == NULL) {
            FILE *fp = fopen(liandian, "w");
            if (fp != NULL) {
                fclose(fp);
            }
        } else {
            fclose(file666);
        }
DrawInt();
std::string filePath = "no_background.txt";  // 移除了固定路径
std::ifstream file(filePath);
if (file.is_open()) {
    pid_t pid = fork();
    if (pid > 0) {
        exit(0);
        file.close();
    } else if (pid == 0) {
        // 子进程代码
    } else {
        fprintf(stderr, "[-] Failed to create background process\n");
        exit(1);
    }
} else {
    // 文件不存在时的处理
}


    pid_t nowpid = getpid();
    char command[100];
    string strbool;
std::string filePaths = "anti_record.txt";  // Removed fixed path
std::ifstream file2(filePaths);
if (file2.is_open()) {
    IsAntiRecordEnabled = true;
    file2.close();
} else {
    IsAntiRecordEnabled = false;
}

RenderWeaponSlider();
getRoot(argc, argv);    
    screen_config(); 
    ::abs_ScreenX = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    ::abs_ScreenY = (displayInfo.height < displayInfo.width ? displayInfo.height : displayInfo.width);
    
    ::native_window_screen_x = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    ::native_window_screen_y = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
    // 初始化imgui
    if (!initGUI_draw(native_window_screen_x, native_window_screen_y, false)) {
        return -1;
    }
      
    
    
    
    
    NumIoLoad("ZEUS_CONFIG");
    std::thread volumeThread(音量);
    timer FPSTimer;
    FPSTimer.AotuFPS_init();
    FPSTimer.setAffinity();

std::string filePathss = "gyroscope.txt";
std::ifstream file3(filePathss);
bool TuoLuoYi = file3.good();
file3.close();

if (TuoLuoYi) {
Touch_Init(&abs_ScreenY,&abs_ScreenX,false);
new std::thread(AimBot, ImGui::GetForegroundDrawList());
} else {
Touch_Init(&abs_ScreenY,&abs_ScreenX,true);
new std::thread(AimBotAuto, ImGui::GetForegroundDrawList());
}

static bool texturesLoaded = false;
if (!texturesLoaded) {
    LoadMemoryImages();
    texturesLoaded = true;
}
//   LoadTextureFromMemory((const void *)&没人, sizeof(没人), &没人图);
 //  LoadTextureFromMemory((const void *)&真人, sizeof(真人), &真人图);
   // LoadTextureFromMemory((const void *)&人机, sizeof(人机), &人机图);
	//LoadTextureFromMemory((const void *)&自瞄关, sizeof(自瞄关), &FloatingAimbotSwitch); 
    while (main_thread_flag)
    {
        FPSTimer.SetFps(FPS);
        FPSTimer.AotuFPS();
        drawBegin();// imgui draw begin
     if (IsAntiRecordEnabled)
{
android::ANativeWindowCreator::ProcessMirrorDisplay();
}

        if (!IsUpdateAvailable){
            static bool gameLaunched = false;
            if (!gameLaunched && !targetPackage.empty()) {
                gameLaunched = true;
                printf("\033[1;32m[SYSTEM]\033[0m Menu opened. Auto-launching: %s\n", targetPackage.c_str());
                char launchCmd[256];
                snprintf(launchCmd, sizeof(launchCmd), "su -c \"monkey -p %s 1\" > /dev/null 2>&1", targetPackage.c_str());
                system(launchCmd);
                // No need to re-focus loader yet, user probably wants to see the game first
            }
            win1();
        }
UpdateAndRenderToasts();
        if (DrawIo[40]) {
            FPS = 24; // Force lower FPS to reduce CPU usage and boost game FPS
        } else {
            FPS = NumIo[12];
        }
        if(初始化)
        DrawPlayer(ImGui::GetForegroundDrawList());
        
        drawEnd();// imgui画图结束
     //   std::this_thread::sleep_for(1ms);
    }
    shutdown();
    return 1;
}
float Numlo[300];
#include "UI.h"
