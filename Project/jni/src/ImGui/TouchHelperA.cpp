#include <TouchHelperA.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <string>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <time.h>
#include <thread>
#include <imgui.h>

using namespace std;

ImVec2 Pos;
int winWidth, winHeith;
#define maxE 5
#define maxF 10
#define UNGRAB 0
#define GRAB 1
static uint32_t orientation = 0;
static float screenHeight = 0, screenWidth = 0;
Vector2 point, point2;

struct touchObj
{
    bool occupy = false;
    bool isTmpDown = false;
    bool isDown = false;
    bool isUp = false;
    int x = 0;
    int y = 0;
    int id = 0;
    int size1 = 0;
    int size2 = 0;
    int size3 = 0;
};

struct targ
{
    int fdNum;
    float S2TX;
    float S2TY;
    bool createVirtualDevice;
};

string exec(string command)
{
    char buffer[128];
    string result = "";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

Vector2 touch_screen_size;
float touch_s;
 
bool isFirstDown = true;
static pthread_t touchloop;
int fdNum = 0, origfd[maxE], nowfd;
struct touchObj Finger[maxE][maxF];
static struct input_event event[128];
static struct input_event downEvent[2];

// 添加缺失的全局变量

bool g_createVirtualDevice = false;
static targ g_threadArg;

char *_genRandomString(int length)
{
    int flag, i;
    srand((unsigned)time(NULL));
    char *tmpString = (char *)malloc(length * sizeof(char));

    for (i = 0; i < length - 1; i++)
    {
        flag = rand() % 3;
        switch (flag)
        {
        case 0:
            tmpString[i] = 'A' + rand() % 26;
            break;
        case 1:
            tmpString[i] = 'a' + rand() % 26;
            break;
        case 2:
            tmpString[i] = '0' + rand() % 10;
            break;
        default:
            tmpString[i] = 'x';
            break;
        }
    }
    tmpString[length - 1] = '\0';
    return tmpString;
}

Vector2 rotatePointx(int orientation, float x, float y, int32_t displayWidth,int32_t displayHeight) {
    if (orientation == 0) {
        return Vector2(x, y);
    }
    Vector2 xy(x, y);
    if (orientation == 3) {
        xy.x = (float)displayHeight - y;
        xy.y = x;
    } else if (orientation == 2) {
        xy.x = (float)displayWidth - x;
        xy.y =(float) displayHeight - y;
    } else if (orientation == 1) {
        xy.x = y;
        xy.y =(float) displayWidth - x;
    }
    return xy;
}

void Upload(bool createVirtualDevice)
{
    if (!createVirtualDevice) {
        return;
    }
    
    int tmpCnt = 0, tmpCnt2 = 0, i, j;
    for (i = 0; i < fdNum; i++)
    {
        for (j = 0; j < maxF; j++)
        {
            if (Finger[i][j].isDown)
            {
                tmpCnt2++;
                if (tmpCnt2 > 10)
                    break;
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_TRACKING_ID;
                event[tmpCnt].value = Finger[i][j].id;
                tmpCnt++;
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_X;
                event[tmpCnt].value = Finger[i][j].x;
                tmpCnt++;
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_Y;
                event[tmpCnt].value = Finger[i][j].y;
                tmpCnt++;
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_POSITION_X;
                event[tmpCnt].value = Finger[i][j].x;
                tmpCnt++;
                event[tmpCnt].type = EV_ABS;
                event[tmpCnt].code = ABS_MT_POSITION_Y;
                event[tmpCnt].value = Finger[i][j].y;
                tmpCnt++;
                if (Finger[i][j].size1)
                {
                    event[tmpCnt].type = EV_ABS;
                    event[tmpCnt].code = ABS_MT_TOUCH_MAJOR;
                    event[tmpCnt].value = Finger[i][j].size1;
                    tmpCnt++;
                }
                if (Finger[i][j].size2)
                {
                    event[tmpCnt].type = EV_ABS;
                    event[tmpCnt].code = ABS_MT_WIDTH_MAJOR;
                    event[tmpCnt].value = Finger[i][j].size2;
                    tmpCnt++;
                }
                if (Finger[i][j].size3)
                {
                    event[tmpCnt].type = EV_ABS;
                    event[tmpCnt].code = ABS_MT_TOUCH_MINOR;
                    event[tmpCnt].value = Finger[i][j].size3;
                    tmpCnt++;
                }
                event[tmpCnt].type = EV_SYN;
                event[tmpCnt].code = SYN_MT_REPORT;
                event[tmpCnt].value = 0;
                tmpCnt++;
            }
        }
    }
    if (tmpCnt == 0)
    {
        event[tmpCnt].type = EV_SYN;
        event[tmpCnt].code = SYN_MT_REPORT;
        event[tmpCnt].value = 0;
        tmpCnt++;
        if (!isFirstDown)
        {
            isFirstDown = true;
            event[tmpCnt].type = EV_KEY;
            event[tmpCnt].code = BTN_TOUCH;
            event[tmpCnt].value = 0;
            tmpCnt++;
            event[tmpCnt].type = EV_KEY;
            event[tmpCnt].code = BTN_TOOL_FINGER;
            event[tmpCnt].value = 0;
            tmpCnt++;
        }
    }
    else
    {
        if (isFirstDown)
        {
            isFirstDown = false;
            write(nowfd, downEvent, sizeof(downEvent));
        }
    }
    event[tmpCnt].type = EV_SYN;
    event[tmpCnt].code = SYN_REPORT;
    event[tmpCnt].value = 0;
    tmpCnt++;
    write(nowfd, event, sizeof(struct input_event) * tmpCnt);
}

int Orientation;
void *TypeA(void *arg)
{
    targ* tmp = (targ*)arg;
    int i = tmp->fdNum;
    float S2TX = tmp->S2TX;
    float S2TY = tmp->S2TY;
    bool createVirtualDevice = tmp->createVirtualDevice;
    struct input_event ie;
    int latest = 0;
    
    for (;;)
    {
        if (read(origfd[i], &ie, sizeof(struct input_event)) > 0)
        {
            if (ie.type == EV_ABS)
            {
                if (ie.code == ABS_MT_SLOT)
                {
                    latest = ie.value;
                    continue;
                }
                if (ie.code == ABS_MT_TRACKING_ID)
                {
                    if (ie.value == -1)
                    {
                        Finger[i][latest].isUp = true;
                        Finger[i][latest].isDown = false;
                    }
                    else
                    {                   
                        Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
                        Finger[i][latest].isDown = true;
                    }
                    // 关键修复：无论什么模式都更新ImGui状态
                    if (g_Initialized) {
                        ImGui::GetIO().MouseDown[0] = Finger[i][latest].isDown;
                    }
                    continue;
                }           
                if (ie.code == ABS_MT_POSITION_X)
                {
                    Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
                    Finger[i][latest].x = (int)(ie.value * S2TX);
                    Finger[i][latest].isTmpDown = true;
                    
                    // 关键修复：无论什么模式都更新ImGui坐标
                    if (g_Initialized) {
                        point = rotatePointx(Orientation, (float)Finger[i][latest].x, 
                                            (float)Finger[i][latest].y,
                                            touch_screen_size.x, touch_screen_size.y);
                        ImGui::GetIO().MousePos = ImVec2(point.x / touch_s, point.y / touch_s);
                    }
                    continue;
                }
                if (ie.code == ABS_MT_POSITION_Y)
                {
                    Finger[i][latest].id = (i * 2 + 1) * maxF + latest;
                    Finger[i][latest].y = (int)(ie.value * S2TY);
                    Finger[i][latest].isTmpDown = true;
                    
                    // 关键修复：无论什么模式都更新ImGui坐标
                    if (g_Initialized) {
                        point = rotatePointx(Orientation, (float)Finger[i][latest].x, 
                                            (float)Finger[i][latest].y,
                                            touch_screen_size.x, touch_screen_size.y);
                        ImGui::GetIO().MousePos = ImVec2(point.x / touch_s, point.y / touch_s);
                    }
                    continue;
                }
                // 虚拟设备模式下才处理额外属性
                if (createVirtualDevice) {
                    if (ie.code == ABS_MT_TOUCH_MAJOR)
                    {
                        Finger[i][latest].size1 = ie.value;
                        Finger[i][latest].isTmpDown = true;
                        continue;
                    }
                    if (ie.code == ABS_MT_WIDTH_MAJOR)
                    {
                        Finger[i][latest].size2 = ie.value;
                        Finger[i][latest].isTmpDown = true;
                        continue;
                    }
                    if (ie.code == ABS_MT_TOUCH_MINOR)
                    {
                        Finger[i][latest].size3 = ie.value;
                        Finger[i][latest].isTmpDown = true;
                        continue;
                    }
                }
            }
            
            if (ie.type == EV_SYN)
            {
                if (ie.code == SYN_REPORT)
                {
                    if (Finger[i][latest].isTmpDown) {
                        // 关键修复：无论什么模式都上传触摸数据
                        Upload(createVirtualDevice);
                    }
                    continue;
                }
                continue;
            }
        }
        usleep(1000);
    }
    return 0;
}

Vector2 getTouchScreenDimension1(int fd){
    int abs_x[6], abs_y[6] = {0};
    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), abs_x);
    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), abs_y);
    return Vector2(abs_x[2], abs_y[2]);
}

struct Screen {
    float ScreenX;
    float ScreenY;
};

Screen full_screen;
int screen_x, screen_y;
bool Isinit=false;

void Touch_Init(int *retX, int *retY)
{
    Touch_Init(retX, retY, true);
}

void Touch_Init(int *retX, int *retY, bool createVirtualDevice)
{
    g_createVirtualDevice = createVirtualDevice;
    g_Initialized = true;
    
    if (!Isinit) {
        std::string window_size = exec("wm size");
        sscanf(window_size.c_str(), "Physical size: %dx%d", &screen_x, &screen_y);
        full_screen.ScreenX = screen_x;
        full_screen.ScreenY = screen_y;
        
        // 方向检测线程
        std::thread([=] {
            int last = -1;
            while (true) {
                int now = atoi(exec("dumpsys display | grep 'mCurrentOrientation' | cut -d'=' -f2").c_str());
                if (now != last) {
                    last = now;
                    Orientation = now;
                    if (now == 0 || now == 2) {
                        screen_x = full_screen.ScreenX;
                        screen_y = full_screen.ScreenY;
                    } else {
                        screen_x = full_screen.ScreenY;
                        screen_y = full_screen.ScreenX;
                    }
                }
                sleep(2);
            }
        }).detach();
        
        Isinit = true;
    }

    if (createVirtualDevice) {
        downEvent[0].type = EV_KEY;
        downEvent[0].code = BTN_TOUCH;
        downEvent[0].value = 1;
        downEvent[1].type = EV_KEY;
        downEvent[1].code = BTN_TOOL_FINGER;
        downEvent[1].value = 1;
    }

    char temp[128];
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int eventCount = 0;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, "event"))
            eventCount++;
    }
    closedir(dir);

    struct input_absinfo absX[maxE], absY[maxE];
    int screenX = 0, screenY = 0, minCnt = eventCount + 1;
    fdNum = 0;

    if (createVirtualDevice) {
        /* ****** 只抢一个设备 ****** */
        bool grabbed = false;
        for (int i = 0; i <= eventCount && !grabbed; ++i) {
            sprintf(temp, "/dev/input/event%d", i);
            int fd = open(temp, O_RDWR);
            if (fd < 0) continue;

            uint8_t *bits = NULL;
            ssize_t bits_size = 0;
            int res, j, k;
            bool hasSlot = false, hasX = false, hasY = false;

            while (1) {
                res = ioctl(fd, EVIOCGBIT(EV_ABS, bits_size), bits);
                if (res < bits_size) break;
                bits_size = res + 16;
                bits = (uint8_t *)realloc(bits, bits_size * 2);
                if (!bits) exit(0);
            }
            for (j = 0; j < res; ++j) {
                for (k = 0; k < 8; ++k) {
                    if (bits[j] & (1 << k)) {
                        int code = j * 8 + k;
                        if (code == ABS_MT_SLOT)       hasSlot = true;
                        if (code == ABS_MT_POSITION_X) hasX = true;
                        if (code == ABS_MT_POSITION_Y) hasY = true;
                    }
                }
            }
            free(bits);

            if (hasSlot && hasX && hasY) {
                if (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &absX[fdNum]) == 0 &&
                    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &absY[fdNum]) == 0) {
                    if (ioctl(fd, EVIOCGRAB, GRAB) != 0) {
                        close(fd);
                        continue;
                    }
                    printf("[+] Touch device grabbed: %s\n", temp);
                    origfd[fdNum] = fd;
                    if (i < minCnt) {
                        screenX = absX[fdNum].maximum;
                        screenY = absY[fdNum].maximum;
                        minCnt  = i;
                    }
                    fdNum++;
                    grabbed = true;
                } else {
                    close(fd);
                }
            } else {
                close(fd);
            }
        }
    } else {
        /* ****** 仅触摸模式：不抢占设备 ****** */
        for (int i = 0; i <= eventCount && fdNum < maxE; ++i) {
            sprintf(temp, "/dev/input/event%d", i);
            int fd = open(temp, O_RDWR);
            if (fd < 0) continue;

            uint8_t* bits = NULL;
            ssize_t bits_size = 0;
            int res, j, k;
            bool hasSlot = false, hasX = false, hasY = false;

            while (1) {
                res = ioctl(fd, EVIOCGBIT(EV_ABS, bits_size), bits);
                if (res < bits_size) break;
                bits_size = res + 16;
                bits = (uint8_t*)realloc(bits, bits_size * 2);
                if (!bits) exit(0);
            }
            for (j = 0; j < res; ++j) {
                for (k = 0; k < 8; ++k) {
                    if (bits[j] & (1 << k)) {
                        int code = j * 8 + k;
                        if (code == ABS_MT_SLOT) hasSlot = true;
                        if (code == ABS_MT_POSITION_X) hasX = true;
                        if (code == ABS_MT_POSITION_Y) hasY = true;
                    }
                }
            }
            free(bits);

            if (hasSlot && hasX && hasY) {
                if (ioctl(fd, EVIOCGABS(ABS_MT_POSITION_X), &absX[fdNum]) == 0 &&
                    ioctl(fd, EVIOCGABS(ABS_MT_POSITION_Y), &absY[fdNum]) == 0) {
                    printf("[+] Touch device found: %s\n", temp);
                    origfd[fdNum] = fd;
                    if (i < minCnt) {
                        screenX = absX[fdNum].maximum;
                        screenY = absY[fdNum].maximum;
                        minCnt = i;
                    }
                    fdNum++;
                } else {
                    close(fd);
                }
            } else {
                close(fd);
            }
        }
    }

    if (fdNum == 0) exit(0);

    if (createVirtualDevice) {
        /* 创建 uinput 虚拟设备 */
        struct uinput_user_dev ui_dev;
        nowfd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (nowfd <= 0) exit(0);

        memset(&ui_dev, 0, sizeof(ui_dev));
        strncpy(ui_dev.name, _genRandomString(rand() % 10 + 5), UINPUT_MAX_NAME_SIZE);
        ui_dev.id.bustype = 0;
        ui_dev.id.vendor  = rand() % 10 + 5;
        ui_dev.id.product = rand() % 10 + 5;
        ui_dev.id.version = rand() % 10 + 5;

        ioctl(nowfd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);
        ioctl(nowfd, UI_SET_EVBIT, EV_ABS);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_X);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_Y);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_WIDTH_MAJOR);
        ioctl(nowfd, UI_SET_ABSBIT, ABS_MT_TOUCH_MINOR);
        ioctl(nowfd, UI_SET_EVBIT, EV_SYN);
        ioctl(nowfd, UI_SET_EVBIT, EV_KEY);
        ioctl(nowfd, UI_SET_KEYBIT, BTN_TOUCH);
        ioctl(nowfd, UI_SET_KEYBIT, BTN_TOOL_FINGER);
        ioctl(nowfd, UI_SET_PHYS, _genRandomString(rand() % 10 + 5));

        sprintf(temp, "/dev/input/event%d", minCnt);
        int fd_id = open(temp, O_RDWR);
        if (fd_id > 0) {
            struct input_id id;
            if (!ioctl(fd_id, EVIOCGID, &id)) {
                ui_dev.id.bustype = id.bustype;
                ui_dev.id.vendor  = id.vendor;
                ui_dev.id.product = id.product;
                ui_dev.id.version = id.version;
            }
            close(fd_id);
        }

        ui_dev.absmin[ABS_X]                = 0;
        ui_dev.absmax[ABS_X]                = screenX;
        ui_dev.absmin[ABS_Y]                = 0;
        ui_dev.absmax[ABS_Y]                = screenY;
        ui_dev.absmin[ABS_MT_POSITION_X]    = 0;
        ui_dev.absmax[ABS_MT_POSITION_X]    = screenX;
        ui_dev.absmin[ABS_MT_POSITION_Y]    = 0;
        ui_dev.absmax[ABS_MT_POSITION_Y]    = screenY;
        ui_dev.absmin[ABS_MT_TRACKING_ID]   = 0;
        ui_dev.absmax[ABS_MT_TRACKING_ID]   = 65535;
        ui_dev.absmin[ABS_MT_TOUCH_MAJOR]   = 0;
        ui_dev.absmax[ABS_MT_TOUCH_MAJOR]   = 255;
        ui_dev.absmin[ABS_MT_WIDTH_MAJOR]   = 0;
        ui_dev.absmax[ABS_MT_WIDTH_MAJOR]   = 255;
        ui_dev.absmin[ABS_MT_TOUCH_MINOR]   = 0;
        ui_dev.absmax[ABS_MT_TOUCH_MINOR]   = 255;

        *retX = screenX;
        *retY = screenY;

        write(nowfd, &ui_dev, sizeof(ui_dev));
        if (ioctl(nowfd, UI_DEV_CREATE)) exit(0);
    } else {
        *retX = screenX;
        *retY = screenY;
    }

    // 使用全局变量避免内存问题
    g_threadArg.fdNum = 0;
    g_threadArg.S2TX  = (float)absX[0].maximum / (float)screenX;
    g_threadArg.S2TY  = (float)absY[0].maximum / (float)screenY;
    g_threadArg.createVirtualDevice = createVirtualDevice;
    
    pthread_create(&touchloop, NULL, TypeA, &g_threadArg);

    touch_screen_size = getTouchScreenDimension1(origfd[0]);
    touch_s = (touch_screen_size.x + touch_screen_size.y) /
              (full_screen.ScreenX + full_screen.ScreenY);
}

void UpdateScreenData(int w, int h, uint32_t orientation_) {
    ::screenWidth = w;
    ::screenHeight = h; 
    ::orientation = orientation_;
}

void Touch_Down(int id, int x, int y)
{
    if (!g_createVirtualDevice) return;
    
    int num = fdNum - 1;
    Finger[num][id].id = (num * 2 + 1) * maxF + id;
    Finger[num][id].x = x*touch_s;
    Finger[num][id].y = y*touch_s;
    Finger[num][id].size1 = 8;
    Finger[num][id].size2 = 8;
    Finger[num][id].size3 = 8;
    Finger[num][id].isDown = true;
    
    if (g_Initialized) {
        ImGui::GetIO().MouseDown[0] = true;
        point = rotatePointx(Orientation, (float)Finger[num][id].x, 
                           (float)Finger[num][id].y,
                           touch_screen_size.x, touch_screen_size.y);
        point2.x = point.x / touch_s;
        point2.y = point.y / touch_s;
        ImGui::GetIO().MousePos = ImVec2(point.x / touch_s, point.y / touch_s);
    }
    
    Upload(true);
}

void Touch_Move(int id, int x, int y)
{
    if (!g_createVirtualDevice) return;
    
    int num = fdNum - 1;
    Finger[num][id].id = (num * 2 + 1) * maxF + id;
    Finger[num][id].x = x*touch_s;
    Finger[num][id].y = y*touch_s;
    Finger[num][id].size1 = 8;
    Finger[num][id].size2 = 8;
    Finger[num][id].size3 = 8;
    Finger[num][id].isDown = true;
    
    if (g_Initialized) {
        point = rotatePointx(Orientation, (float)Finger[num][id].x, 
                           (float)Finger[num][id].y,
                           touch_screen_size.x, touch_screen_size.y);
        point2.x = point.x / touch_s;
        point2.y = point.y / touch_s;
        ImGui::GetIO().MousePos = ImVec2(point.x / touch_s, point.y / touch_s);
    }
    
    Upload(true);
}

void Touch_Up(int id)
{
    if (!g_createVirtualDevice) return;
    
    int num = fdNum - 1;
    Finger[num][id].isDown = false;
    Finger[num][id].isUp = true;
    
    if (g_Initialized) {
        ImGui::GetIO().MouseDown[0] = false;
    }
    
    Upload(true);
}