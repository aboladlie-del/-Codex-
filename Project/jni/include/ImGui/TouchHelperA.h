#ifndef ZEUS_KERNEL_TOUCH_H
#define ZEUS_KERNEL_TOUCH_H

#include <cstdio>
#include <cstdlib>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <linux/input.h>
#include <linux/uinput.h>
#include <ctime>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Draw.h"

extern ImVec2 Pos;
extern int winWidth;
extern int winHeith;

struct Vector2 {
    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    Vector2() {}
    float x;
    float y;
    bool operator == (const Vector2 &t) const {
        if ( this->x == t.x && this->y == t.y ) return true;
        return false;
    }
    bool operator != (const Vector2 &t) const {
        if ( this->x != t.x || this->y != t.y ) return true;
        return false;
    }
};

extern Vector2 point;
extern Vector2 point2;
extern std::mutex touch_mutex;
extern std::condition_variable touch_cv;

void UpdateScreenData(int w, int h, uint32_t orientation_);
void Touch_Init(int *retX, int *retY, bool createVirtualDevice); // 新的重载版本
void Touch_Down(int id,int x, int y);
void Touch_Move(int id,int x, int y);
void Touch_Up(int id);

#endif