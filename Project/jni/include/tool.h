#ifndef ZEUS_KERNEL_MEMREAD_H
#define ZEUS_KERNEL_MEMREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "driver.h"
using namespace std;
std::vector<std::string> lines;
	// 定义圆周率
#define PI 3.141592653589793238
#define PI_180_INV (180.0f / PI)
typedef unsigned short UTF16;
typedef char UTF8;

float px,py;

string exec_native_surface(string command) {
    char buffer[128];
    string result = "";
    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    // read till end of process:
    while (!feof(pipe)) {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}
// 获取进程

// 算法 结构体 //
struct Vector2A
{
	float X;
	float Y;

	  Vector2A()
	{
		this->X = 0;
		this->Y = 0;
	}

	Vector2A(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
};
struct Vector3A
{
	float X;
	float Y;
	float Z;

	  Vector3A()
	{
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3A(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

};
struct D2DVector
{
	float X;
	float Y;
};

struct D3DVector
{
	float X;
	float Y;
	float Z;
};
struct D4DVector
{
	float X;
	float Y;
	float Z;
	float W;
};


struct FMatrix
{
	float M[4][4];
};





struct FTransform
{
	D4DVector Rotation;
	Vector3A Translation;
	float chunk;
	Vector3A Scale3D;
};





inline float getMinAngle(float target, float self) {
    float delta = target - self;
    // 性能优化：使用条件表达式替代while循环
    if (delta > 180.0f) delta -= 360.0f;
    else if (delta < -180.0f) delta += 360.0f;
    return delta;
}
struct alignas(8) Rotator {
    float Pitch;  // 绕X轴旋转（抬头/低头）
    float Yaw;    // 绕Y轴旋转（左/右转向）
    float Roll;   // 绕Z轴旋转（倾斜）
    
    // 性能优化：使用初始化列表
    Rotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
    Rotator(float _P, float _Y, float _R) : Pitch(_P), Yaw(_Y), Roll(_R) {}
};
inline Vector2A FastAtan2(const Vector3A& Target, const Vector3A& Self) {
    Vector2A AimCoordinates;
    float dx = Target.X - Self.X;
    float dy = Target.Y - Self.Y;
    float dz = Target.Z - Self.Z;
    float hyp = sqrt(dx * dx + dy * dy);
    
    // 性能优化：使用预计算的PI常量
    AimCoordinates.X = atan2(dz, hyp) * PI_180_INV;
    AimCoordinates.Y = atan2(dy, dx) * PI_180_INV;
    return AimCoordinates;
}

Vector3A MarixToVector(FMatrix matrix)//
{
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)//
{
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)//
{
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}

FTransform getBone(unsigned long addr)
{
    FTransform transform;
    driver->read(addr, &transform, 4 * 11);
    return transform;
}

// 读取字符信息
void getUTF8(char *buf, unsigned long namepy)
{
	unsigned short buf16[32] = {0};
	driver->read(namepy, buf16, 28);
	unsigned short *pTempUTF16 = buf16;
	char *pTempUTF8 = buf;
	char *pUTF8End = pTempUTF8 + 32;
	while (pTempUTF16 < pTempUTF16 + 28)
	{
		if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End)
		{
			*pTempUTF8++ = (char)*pTempUTF16;
		}
		else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
			*pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
}





class FRotator {
   public:
    FRotator()
        : Pitch(0.f), Yaw(0.f), Roll(0.f) {
    }
    FRotator(float _Pitch, float _Yaw, float _Roll)
        : Pitch(_Pitch), Yaw(_Yaw), Roll(_Roll) {
    }
    ~FRotator() {
    }
    float Pitch;
    float Yaw;
    float Roll;
    inline FRotator Clamp() {
        if (Pitch > 180) {
            Pitch -= 360;
        } else {
            if (Pitch < -180) {
                Pitch += 360;
            }
        }
        if (Yaw > 180) {
            Yaw -= 360;
        } else {
            if (Yaw < -180) {
                Yaw += 360;
            }
        }
        if (Pitch > 89) {
            Pitch = 89;
        }
        if (Pitch < -89) {
            Pitch = -89;
        }
        while (Yaw < 180) {
            Yaw += 360;
        }
        while (Yaw > 180) {
            Yaw -= 360;
        }
        Roll = 0;
        return FRotator(Pitch, Yaw, Roll);
    }
    inline float Length() {
        return sqrtf(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
    }
    FRotator operator+(FRotator v) {
        return FRotator(Pitch + v.Pitch, Yaw + v.Yaw, Roll + v.Roll);
    }
    FRotator operator-(FRotator v) {
        return FRotator(Pitch - v.Pitch, Yaw - v.Yaw, Roll - v.Roll);
    }
};
static int getLength(const char* str) {
    int len = 0;
    while (*str) {
        if ((*str & 0xC0) != 0x80) {
            len++;
        }
        ++str;
    }
    return len;
}
Vector2A getAngle2D(const Vector3A& Enemy, const Vector3A& RealPerson)
{
    float dx = RealPerson.X - Enemy.X;
    float dy = RealPerson.Y - Enemy.Y;
    float dz = RealPerson.Z - Enemy.Z;

    Vector2A out;
    out.X = atan2(dy, dx) * 180.0f / 3.1415926f;      // 水平角
    float hor = sqrt(dx*dx + dy*dy);
    out.Y = atan2(dz, hor) * 180.0f / 3.1415926f;     // 俯仰角
    return out;
}


// 计算旋转坐标
Vector2A rotateCoord(float angle, float objRadar_x, float objRadar_y)
{
	Vector2A radarCoordinate;
	float s = sin(angle * PI / 180);
	float c = cos(angle * PI / 180);
	radarCoordinate.X = objRadar_x * c + objRadar_y * s;
	radarCoordinate.Y = -objRadar_x * s + objRadar_y * c;
	return radarCoordinate;
}
//世界坐标转屏幕坐标
Vector2A WorldToScreen(Vector3A obj, float matrix[16], float ViewW)//
{
	float x =
		px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y =
		py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;

	return Vector2A(x, y);
}






static float tm1=127/255.f;
static ImVec4 arr[] = {
        {144/255.f, 238/255.f, 144/255.f, tm1},
        {135/255.f, 206/255.f, 255/255.f, tm1},
        {255/255.f, 0/255.f,   0/255.f,   tm1},
        {0/255.f,   255/255.f, 0/255.f,   tm1},
        {0/255.f,   255/255.f, 127/255.f, tm1},
        {255/255.f, 182/255.f, 193/255.f, tm1},
        {218/255.f, 112/255.f, 214/255.f, tm1},
        {248/255.f, 248/255.f, 255/255.f, tm1},
        {0/255.f,   255/255.f, 255/255.f, tm1},
        {255/255.f, 165/255.f, 0/255.f,   tm1},
        {153/255.f, 204/255.f, 255/255.f, tm1},
        {204/255.f, 255/255.f, 153/255.f, tm1},
        {255/255.f, 255/255.f, 153/255.f, tm1},
        {255/255.f, 153/255.f, 153/255.f, tm1},
        {153/255.f, 153/255.f, 204/255.f, tm1},
        {204/255.f, 204/255.f, 204/255.f, tm1},
        {102/255.f, 204/255.f, 153/255.f, tm1},
        {255/255.f, 102/255.f, 0/255.f,   tm1},
        {102/255.f, 204/255.f, 204/255.f, tm1},
        {153/255.f, 204/255.f, 255/255.f, tm1}
};
static int length=sizeof(arr)/16;
float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y,
					  float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	// 求平方根
	return (float)(sqrt(x * x + y * y + z * z));
}


bool 获取载具(int id, char* name)
{
	//;
	// memset(name, '\0', sizeof(name));
	
	if (id >= 3306001 && id<= 336010)//不知道什么b原因读取不到
	{
		strcpy(name, "Motorcycle Cart");
		return true;
	}
	else if (id >= 3306001 && id <= 3306010)//另写一行
	{
		strcpy(name, "Dacia");
		return true;
	}
	else if (id >= 3301001 && id <= 3303010)
	{
		strcpy(name, "Buggy");
		return true;
    }
	else if (id >= 3334001 && id <= 3334010)
	{
		strcpy(name, "Motor Glider");
		return true;
	}
	
	else if (id >= 3309001 && id <= 3309001)
	{
		strcpy(name, "Motorcycle Cart");
		return true;
	}
	else if (id >= 3325001 && id <= 3325010)
	{
		strcpy(name, "Boat");
		return true;
    }
	else if (id >= 3313001 && id <= 3313010)
	{
		strcpy(name, "Convertible");
		return true;
    }
	else if (id >= 3336001 && id <= 3336001)
	{
		strcpy(name, "Coupe");
		return true;
	}
    else if (id >= 3349001 && id <= 3349001)
	{
		strcpy(name, "Coupe");
		return true;
	}
	else if (id >= 3307001 && id <= 3307010)
	{
		strcpy(name,"Bus");
		return true;
    }
	else if (id >= 3302001 && id <= 3302010)
	{
		strcpy(name,"UAZ Closed");
		return true;
    }
	else if (id >= 3303001 && id <= 3303010)
	{
		strcpy(name,"UAZ Open");
		return true;
	}
	else if (id >= 3304001 && id <= 3304010)
	{
		strcpy(name,"UAZ");
		return true;
	}	
	else if (id >= 3314001 && id <= 3314010)
	{
		strcpy(name,"Pickup");
		return true;
	}	
	else if (id >= 3322001 && id <= 3322010)
	{
		strcpy(name,"Snow UAZ");
		return true;
	}	
	else if (id >= 3324001 && id <= 3324010)
	{
	
		strcpy(name,"Snowmobile");
		return true;
	}	
	else if (id >= 3321001 && id <= 3321010)
	{
		strcpy(name,"Snowmobile");
		return true;
	}
	else if (id >= 3308001 && id <= 3308010)
	{
		strcpy(name,"Motorcycle");
		return true;
	}
	else if (id >= 3310001 && id <= 3310010)
	{
		strcpy(name,"Pickup Truck");
		return true;
	}
	else if (id >= 3315001 && id <= 3315010)
	{
		strcpy(name,"Scooter");
		return true;
	}
	else if (id >= 3326001 && id <= 3326010)
	{
		strcpy(name,"Aqua Rail");
		return true;
	}
	else if (id >= 3329001 && id <= 3329010)
	{
		strcpy(name,"BRDM");
		return true;
	}
	else if (id >= 3312001 && id <= 3312010)
	{
		strcpy(name,"Coupe");
		return true;
	}
	else if (id >= 3335001 && id <= 3335010)
	{
		strcpy(name,"Monster Truck");
		return true;
	}
	else if (id >= 3323001 && id <= 3323010)
	{
		strcpy(name,"Tuk");
		return true;
	}
	else if (id >= 3348001 && id <= 3348010)
	{
		strcpy(name,"Classic Car");
	    return true;
	}
	
	    return false;
}
bool 获取投掷物(int id, char* name)
{
if (id >= 602004 && id<= 602004)
	{
		
    strcpy(name, "Frag Grenade");
		
      return true;
      
	 }
	 else if (id >= 602003 && id <= 602003)//另写一行
	{
		strcpy(name, "Molotov");
		return true;
	}
	else if (id >= 602002 && id <= 602002)//另写一行
	{
		strcpy(name, "Smoke Grenade");
		return true;
	}
	else if (id >= 602001 && id <= 602001)//另写一行
	{
		strcpy(name, "Thermite");
		return true;
	}
        return false;
}
 char *状态(int dz)
{
   // int dz;
	static char name[30];
memset(name, '\0', sizeof(name));
      if(dz == 524296)   
        strcpy(name, "Driving");
 else if (dz== 4194307)
	    strcpy(name, "Swimming"); 	
 else if (dz== 520)
	    strcpy(name, "Scoped"); 	
 else if (dz == 648)
        strcpy(name, "Firing");
 else if (dz== 1032)
	    strcpy(name, "Leaning");
 else if (dz == 11)
	    strcpy(name, "Running");
 else if (dz == 72)
	    strcpy(name, "Jumping");	
 else if (dz == 75)
	    strcpy(name, "Jumping");	
 else if (dz == 16392)
		strcpy(name, "Cooking Grenade");
 else if (dz == 32)
		strcpy(name, "Prone");		
 else if (dz == 16)
		strcpy(name, "Crouch");		
 else if (dz == 1040)
		strcpy(name, "Crouch Leaning");		
 else if (dz == 264)
		strcpy(name, "Reloading");		
 else if (dz == 160)
		strcpy(name, "Prone Firing");		
 else if (dz == 272)
		strcpy(name, "Crouch Reload");		
 else if (dz == 288)
		strcpy(name, "Prone Reload");					
 else if (dz == 16777226)
		strcpy(name, "Vaulting");		
 else if (dz == 137)
		strcpy(name, "Walking Firing");			
 else if (dz == 1048584)
		strcpy(name, "Passenger");	
 else if (dz == 9)
		strcpy(name, "Walking");			
  else if (dz == 145)
		strcpy(name, "Crouch Firing");		
return name;

}
char *GetHolGunItem(int BoxId)
{
    switch (BoxId)
    {
       
//投掷爆炸物
    

  case 0:
 return "";
 break;
  case 9807001:
 return "Machete";
 break;
 case 9807002:
 return "Crowbar";
 break;
 case 9807003:
 return "Sickle";
 break;
 case 9807004:
 return "Pan";
 break;
 case 9807005:
 return "Flame Blade";
 break;
case 101001:
return "AKM";
break;
case 101901:
return "AKM-XT";
break;
case 1010011:
return "AKM-Damaged";
break;
case 1010012:
return "AKM-Repaired";
break;
case 1010013:
return "AKM-Intact";
break;
case 1010014:
return "AKM-Improved";
break;
case 1010015:
return "AKM-Refined";
break;
case 1010016:
return "AKM-Cobra";
break;
case 1010017:
return "AKM-Steel Front";
break;
case 1010019:
return "AKM";
break;
case 101002:
return "M16A4";
break;
case 1010021:
return "M16A4-Damaged";
break;
case 1010022:
return "M16A4-Repaired";
break;
case 1010023:
return "M16A4-Intact";
break;
case 1010024:
return "M16A4-Improved";
break;
case 1010025:
return "M16A4-Refined";
break;
case 101003:
return "SCAR-L";
break;
case 101903:
return "SCAR-XT";
break;
case 1010031:
return "SCAR-L-Damaged";
break;
case 1010032:
return "SCAR-L-Repaired";
break;
case 1010033:
return "SCAR-L-Intact";
break;
case 1010034:
return "SCAR-L-Improved";
break;
case 1010035:
return "SCAR-L-Refined";
break;
case 1010036:
return "SCAR-L-Cobra";
break;
case 1010037:
return "SCAR-L-Steel Front";
break;
case 1010039:
return "SCAR-L";
break;
case 101004:
return "M416";
break;
case 1010041:
return "M416-Damaged";
break;
case 1010042:
return "M416-Repaired";
break;
case 1010043:
return "M416-Intact";
break;
case 1010044:
return "M416-Improved";
break;
case 1010045:
return "M416-Refined";
break;
case 1010046:
return "M416-Cobra";
break;
case 1010047:
return "M416-Steel Front";
break;
case 1010049:
return "M416";
break;
case 101005:
return "GROZA";
break;
case 1010051:
return "GROZA-Damaged";
break;
case 1010052:
return "GROZA-Repaired";
break;
case 1010053:
return "GROZA-Intact";
break;
case 1010054:
return "GROZA-Improved";
break;
case 1010055:
return "GROZA-Refined";
break;
case 1010056:
return "GROZA-Cobra";
break;
case 1010057:
return "GROZA-Steel Front";
break;
case 1010059:
return "GROZA";
break;
case 101006:
return "AUG";
break;
case 1010061:
return "AUG-Damaged";
break;
case 1010062:
return "AUG-Repaired";
break;
case 1010063:
return "AUG-Intact";
break;
case 1010064:
return "AUG-Improved";
break;
case 1010065:
return "AUG-Refined";
break;
case 1010066:
return "AUG-Cobra";
break;
case 1010067:
return "AUG-Steel Front";
break;
case 101007:
return "QBZ";
break;
case 1010071:
return "QBZ-Damaged";
break;
case 1010072:
return "QBZ-Repaired";
break;
case 1010073:
return "QBZ-Intact";
break;
case 1010074:
return "QBZ-Improved";
break;
case 1010075:
return "QBZ-Refined";
break;
case 1010076:
return "QBZ-Cobra";
break;
case 1010077:
return "QBZ-Steel Front";
break;
case 101008:
return "M762";
break;
case 101908:
return "M762-XT";
break;
case 1010081:
return "M762-Damaged";
break;
case 1010082:
return "M762-Repaired";
break;
case 1010083:
return "M762-Intact";
break;
case 1010084:
return "M762-Improved";
break;
case 1010085:
return "M762-Refined";
break;
case 1010086:
return "M762-Cobra";
break;
case 1010087:
return "M762-Steel Front";
break;
case 1010089:
return "M762";
break;
case 101009:
return "Mk47";
break;
case 1010091:
return "Mk47-Damaged";
break;
case 1010092:
return "Mk47-Repaired";
break;
case 1010093:
return "Mk47-Intact";
break;
case 1010094:
return "Mk47-Improved";
break;
case 1010095:
return "Mk47-Refined";
break;
case 101010:
return "G36C";
break;
case 1010101:
return "G36C-Damaged";
break;
case 1010102:
return "G36C-Repaired";
break;
case 1010103:
return "G36C-Intact";
break;
case 1010104:
return "G36C-Improved";
break;
case 1010105:
return "G36C-Refined";
break;
case 1010106:
return "G36C-Cobra";
break;
case 1010107:
return "G36C-Steel Front";
break;
case 101012:
return "Honey Badger";
break;
case 1010121:
return "Honey Badger-Damaged";
break;
case 1010122:
return "Honey Badger-Repaired";
break;
case 1010123:
return "Honey Badger-Intact";
break;
case 1010124:
return "Honey Badger-Improved";
break;
case 1010125:
return "Honey Badger-Refined";
break;
case 1010126:
return "Honey Badger-Cobra";
break;
case 1010127:
return "Honey Badger-Steel Front";
break;
case 101100:
return "FAMAS";
break;
case 1011001:
return "FAMAS-Damaged";
break;
case 1011002:
return "FAMAS-Repaired";
break;
case 1011003:
return "FAMAS-Intact";
break;
case 1011004:
return "FAMAS-Improved";
break;
case 1011005:
return "FAMAS-Refined";
break;
case 1011006:
return "FAMAS-Cobra";
break;
case 1011007:
return "FAMAS-Steel Front";
break;
case 101102:
return "ACE32";
break;
case 1011021:
return "ACE32-Damaged";
break;
case 1011022:
return "ACE32-Repaired";
break;
case 1011023:
return "ACE32-Intact";
break;
case 1011024:
return "ACE32-Improved";
break;
case 1011025:
return "ACE32-Refined";
break;
case 1011026:
return "ACE32-Cobra";
break;
case 1011027:
return "ACE32-Steel Front";
break;
case 102001:
return "UZI";
break;
case 102901:
return "UZI-XT";
break;
case 1020011:
return "UZI-Damaged";
break;
case 1020012:
return "UZI-Repaired";
break;
case 1020013:
return "UZI-Intact";
break;
case 1020014:
return "UZI-Improved";
break;
case 1020015:
return "UZI-Refined";
break;
case 102002:
return "UMP45";
break;
case 1020021:
return "UMP45-Damaged";
break;
case 1020022:
return "UMP45-Repaired";
break;
case 1020023:
return "UMP45-Intact";
break;
case 1020024:
return "UMP45-Improved";
break;
case 1020025:
return "UMP45-Refined";
break;
case 1020029:
return "UMP45";
break;
case 102003:
return "Vector";
break;
case 102903:
return "Vector-XT";
break;
case 1020031:
return "Vector-Damaged";
break;
case 1020032:
return "Vector-Repaired";
break;
case 1020033:
return "Vector-Intact";
break;
case 1020034:
return "Vector-Improved";
break;
case 1020035:
return "Vector-Refined";
break;
case 1020036:
return "Vector-Cobra";
break;
case 1020037:
return "Vector-Steel Front";
break;
case 102004:
return "Thompson";
break;
case 1020041:
return "Thompson-Damaged";
break;
case 1020042:
return "Thompson-Repaired";
break;
case 1020043:
return "Thompson-Intact";
break;
case 1020044:
return "Thompson-Improved";
break;
case 1020045:
return "Thompson-Refined";
break;
case 102005:
return "Bizon";
break;
case 1020051:
return "Bizon-Damaged";
break;
case 1020052:
return "Bizon-Repaired";
break;
case 1020053:
return "Bizon-Intact";
break;
case 1020054:
return "Bizon-Improved";
break;
case 1020055:
return "Bizon-Refined";
break;
case 1020059:
return "Bizon";
break;
case 102007:
return "MP5K";
break;
case 1020071:
return "MP5K-Damaged";
break;
case 1020072:
return "MP5K-Repaired";
break;
case 1020073:
return "MP5K-Intact";
break;
case 1020074:
return "MP5K-Improved";
break;
case 1020075:
return "MP5K-Refined";
break;
case 1020076:
return "MP5K-Cobra";
break;
case 1020077:
return "MP5K-Steel Front";
break;
case 102105:
return "P90";
break;
case 1021051:
return "P90-Damaged";
break;
case 1021052:
return "P90-Repaired";
break;
case 1021053:
return "P90-Intact";
break;
case 1021054:
return "P90-Improved";
break;
case 1021055:
return "P90-Refined";
break;
case 103001:
return "Kar98K";
break;
case 103901:
return "Kar98K-XT";
break;
case 1030011:
return "Kar98K-Damaged";
break;
case 1030012:
return "Kar98K-Repaired";
break;
case 1030013:
return "Kar98K-Intact";
break;
case 1030014:
return "Kar98K-Improved";
break;
case 1030015:
return "Kar98K-Refined";
break;
case 1030019:
return "Kar98K";
break;
case 103002:
return "M24";
break;
case 103902:
return "M24-XT";
break;
case 1030021:
return "M24-Damaged";
break;
case 1030022:
return "M24-Repaired";
break;
case 1030023:
return "M24-Intact";
break;
case 1030024:
return "M24-Improved";
break;
case 1030025:
return "M24-Refined";
break;
case 1030026:
return "M24-Cobra";
break;
case 1030027:
return "M24-Steel Front";
break;
case 103003:
return "AWM";
break;
case 103903:
return "AWM";
break;
case 1030031:
return "AWM-Damaged";
break;
case 1030032:
return "AWM-Repaired";
break;
case 1030033:
return "AWM-Intact";
break;
case 1030034:
return "AWM-Improved";
break;
case 1030035:
return "AWM-Refined";
break;
case 1030036:
return "AWM-Cobra";
break;
case 1030037:
return "AWM-Steel Front";
break;
case 1030039:
return "AWM";
break;
case 103004:
return "SKS";
break;
case 1030041:
return "SKS-Damaged";
break;
case 1030042:
return "SKS-Repaired";
break;
case 1030043:
return "SKS-Intact";
break;
case 1030044:
return "SKS-Improved";
break;
case 1030045:
return "SKS-Refined";
break;
case 1030046:
return "SKS-Cobra";
break;
case 1030047:
return "SKS-Steel Front";
break;
case 103005:
return "VSS";
break;
case 1030051:
return "VSS-Damaged";
break;
case 1030052:
return "VSS-Repaired";
break;
case 1030053:
return "VSS-Intact";
break;
case 1030054:
return "VSS-Improved";
break;
case 1030055:
return "VSS-Refined";
break;
case 1030059:
return "VSS";
break;
case 103006:
return "Mini14";
break;
case 1030061:
return "Mini14-Damaged";
break;
case 1030062:
return "Mini14-Repaired";
break;
case 1030063:
return "Mini14-Intact";
break;
case 1030064:
return "Mini14-Improved";
break;
case 1030065:
return "Mini14-Refined";
break;
case 1030069:
return "Mini14";
break;
case 1031011:
return "Mosin-Damaged";
break;
case 1031012:
return "Mosin-Repaired";
break;
case 1031013:
return "Mosin-Intact";
break;
case 1031014:
return "Mosin-Improved";
break;
case 1031015:
return "Mosin-Refined";
break;
case 103007:
return "Mk14";
break;
case 1030071:
return "Mk14-Damaged";
break;
case 1030072:
return "Mk14-Repaired";
break;
case 1030073:
return "Mk14-Intact";
break;
case 1030074:
return "Mk14-Improved";
break;
case 1030075:
return "Mk14-Refined";
break;
case 1030076:
return "Mk14-Cobra";
break;
case 1030077:
return "Mk14-Steel Front";
break;
case 103008:
return "Win94";
break;
case 1030081:
return "Win94-Damaged";
break;
case 1030082:
return "Win94-Repaired";
break;
case 1030083:
return "Win94-Intact";
break;
case 1030084:
return "Win94-Improved";
break;
case 1030085:
return "Win94-Refined";
break;
case 103009:
return "SLR";
break;
case 1030091:
return "SLR-Damaged";
break;
case 1030092:
return "SLR-Repaired";
break;
case 1030093:
return "SLR-Intact";
break;
case 1030094:
return "SLR-Improved";
break;
case 1030095:
return "SLR-Refined";
break;
case 1030096:
return "SLR-Cobra";
break;
case 1030097:
return "SLR-Steel Front";
break;
case 1030099:
return "SLR";
break;
case 103010:
return "QBU";
break;
case 1030101:
return "QBU-Damaged";
break;
case 1030102:
return "QBU-Repaired";
break;
case 1030103:
return "QBU-Intact";
break;
case 1030104:
return "QBU-Improved";
break;
case 1030105:
return "QBU-Refined";
break;
case 103012:
return "AMR";
break;
case 1030121:
return "AMR-Damaged";
break;
case 1030122:
return "AMR-Repaired";
break;
case 1030123:
return "AMR-Intact";
break;
case 1030124:
return "AMR-Improved";
break;
case 1030125:
return "AMR-Refined";
break;
case 1030126:
return "AMR-Cobra";
break;
case 1030127:
return "AMR-Steel Front";
break;
case 103100:
return "Mk12";
break;
case 1031001:
return "Mk12-Damaged";
break;
case 1031002:
return "Mk12-Repaired";
break;
case 1031003:
return "Mk12-Intact";
break;
case 1031004:
return "Mk12-Improved";
break;
case 1031005:
return "Mk12-Refined";
break;
case 1031006:
return "Mk12-Cobra";
break;
case 1031007:
return "Mk12-Steel Front";
break;
case 103101:
return "Mosin";
break;
case 104001:
return "S686";
break;
case 1040011:
return "S686-Damaged";
break;
case 1040012:
return "S686-Repaired";
break;
case 1040013:
return "S686-Intact";
break;
case 1040014:
return "S686-Improved";
break;
case 1040015:
return "S686-Refined";
break;
case 1040022:
return "S1897-Repaired";
break;
case 1040023:
return "S1897-Intact";
break;
case 1040024:
return "S1897-Improved";
break;
case 1040025:
return "S1897-Refined";
break;
case 104003:
return "S12K";
break;
case 1040031:
return "S12K-Damaged";
break;
case 1040032:
return "S12K-Repaired";
break;
case 1040033:
return "S12K-Intact";
break;
case 1040034:
return "S12K-Improved";
break;
case 1040035:
return "S12K-Refined";
break;
case 1040039:
return "S12K";
break;
case 104004:
return "DBS";
break;
case 1040041:
return "DBS-Damaged";
break;
case 1040042:
return "DBS-Repaired";
break;
case 1040043:
return "DBS-Intact";
break;
case 1040044:
return "DBS-Improved";
break;
case 1040045:
return "DBS-Refined";
break;
case 1040049:
return "DBS";
break;
case 104102:
return "NS2000";
break;
case 1041021:
return "NS2000-Damaged";
break;
case 1041022:
return "NS2000-Repaired";
break;
case 1041023:
return "NS2000-Intact";
break;
case 1041024:
return "NS2000-Improved";
break;
case 1041025:
return "NS2000-Refined";
break;
case 105001:
return "M249";
break;
case 1050011:
return "M249-Damaged";
break;
case 1050012:
return "M249-Repaired";
break;
case 1050013:
return "M249-Intact";
break;
case 1050014:
return "M249-Improved";
break;
case 1050015:
return "M249-Refined";
break;
case 1050016:
return "M249-Cobra";
break;
case 1050017:
return "M249-Steel Front";
break;
case 105002:
return "DP-28";
break;
case 1050021:
return "DP-28-Damaged";
break;
case 1050022:
return "DP-28-Repaired";
break;
case 1050023:
return "DP-28-Intact";
break;
case 1050024:
return "DP-28-Improved";
break;
case 1050025:
return "DP-28-Refined";
break;
case 1050029:
return "DP-28";
break;
case 105010:
return "MG3";
break;
case 1050101:
return "MG3-Damaged";
break;
case 1050102:
return "MG3-Repaired";
break;
case 1050103:
return "MG3-Intact";
break;
case 1050104:
return "MG3-Improved";
break;
case 1050105:
return "MG3-Refined";
break;
case 1050106:
return "MG3-Cobra";
break;
case 1050107:
return "MG3-Steel Front";
break;
case 1050109:
return "MG3";
break;
case 106001:
return "P92";
break;
case 1060011:
return "P92-Damaged";
break;
case 1060012:
return "P92-Repaired";
break;
case 1060013:
return "P92-Intact";
break;
case 1060014:
return "P92-Improved";
break;
case 1060015:
return "P92-Refined";
break;
case 1060019:
return "P92";
break;
case 106002:
return "P1911";
break;
case 1060021:
return "P1911-Damaged";
break;
case 1060022:
return "P1911-Repaired";
break;
case 1060023:
return "P1911-Intact";
break;
case 1060024:
return "P1911-Improved";
break;
case 1060025:
return "P1911-Refined";
break;
case 106003:
return "R1895";
break;
case 1060031:
return "R1895-Damaged";
break;
case 1060032:
return "R1895-Repaired";
break;
case 1060033:
return "R1895-Intact";
break;
case 1060034:
return "R1895-Improved";
break;
case 1060035:
return "R1895-Refined";
break;
case 106004:
return "P18C";
break;
case 1060041:
return "P18C-Damaged";
break;
case 1060042:
return "P18C-Repaired";
break;
case 1060043:
return "P18C-Intact";
break;
case 1060044:
return "P18C-Improved";
break;
case 1060045:
return "P18C-Refined";
break;
case 1060049:
return "P18C";
break;
case 106005:
return "R45";
break;
case 1060051:
return "R45-Damaged";
break;
case 1060052:
return "R45-Repaired";
break;
case 1060053:
return "R45-Intact";
break;
case 1060054:
return "R45-Improved";
break;
case 1060055:
return "R45-Refined";
break;
case 106006:
return "Sawed-off";
break;
case 1060061:
return "Sawed-off-Damaged";
break;
case 1060062:
return "Sawed-off-Repaired";
break;
case 1060063:
return "Sawed-off-Intact";
break;
case 1060064:
return "Sawed-off-Improved";
break;
case 1060065:
return "Sawed-off-Refined";
break;
case 1060069:
return "Sawed-off";
break;
case 106008:
return "Skorpion";
break;
case 1060081:
return "Skorpion-Damaged";
break;
case 1060082:
return "Skorpion-Repaired";
break;
case 1060083:
return "Skorpion-Intact";
break;
case 1060084:
return "Skorpion-Improved";
break;
case 1060085:
return "Skorpion-Refined";
break;case 1060089:
return "Skorpion";
break;
case 106010:
return "Desert Eagle";
break;
case 1060101:
return "Desert Eagle-Damaged";
break;
case 1060102:
return "Desert Eagle-Repaired";
break;
case 1060103:
return "Desert Eagle-Intact";
break;
case 1060104:
return "Desert Eagle-Improved";
break;
case 1060105:
return "Desert Eagle-Refined";
break;
case 1060109:
return "Desert Eagle-Commander";
break;
case 107001:
return "Crossbow";
break;
case 1070011:
return "Crossbow-Damaged";
break;
case 1070012:
return "Crossbow-Repaired";
break;
case 1070013:
return "Crossbow-Intact";
break;
case 1070014:
return "Crossbow-Improved";
break;
case 1070015:
return "Crossbow-Refined";
break;
case 106103:
return "Flare Gun";
break;
case 106106:
return "Flare Gun-Empty";
break;
case 106007:
return "Flare Gun";
break;
case 1060071:
return "Evac Flare Gun";
break;
break;
case 101011:
return "AC-VAL";
break;
case 101101:
return "ASMAbakan";
break;
case 103011:
return "Mosin Nagant";
break;
case 104100:
return "SPAS-12";
break;
case 104101:
return "M1014";
break;
case 106908:
return "Vz61-XT";
break;
case 105004:
return "Flamethrower";
break;
break;
case 107003:
return "Riot Shield";
break;
case 107007:
return "Tactical Crossbow";
break;
case 107008:
return "Explosive Bow";
break;
case 107009:
return "Explosive Bow";
break;
case 107020:
return "Explosive Crossbow";
break;
case 602001:
return "Stun Grenade";
break;
case 602002:
return "Smoke Grenade";
break;
case 602003:
return "Molotov";
break;
case 602004:
return "Frag Grenade";
break;
case 108001:
return "Machete";
break;
case 1080011:
return "Machete";
break;
case 108002:
return "Crowbar";
break;
case 108003:
return "Sickle";
break;
case 108004:
return "Pan";
break;
case 1080041:
return "Pan";
break;
case 108005:
return "Military Knife";
break;
case 1080051:
return "Military Knife";
break;
case 602005:
return "Apple";
      break;

             
    default:
      return "Official Channel @MinLOD";
      break;
  }
    return nullptr;
}

ImColor ColorArr[100];
ImColor RandomColor()
{
    int R, G, B, A = 150;
    R = (random() % 255);
    G = (random() % 255);
    B = (random() % 255);
    return ImColor(R, G, B, A);
}
void ColorInitialization()
{
    for(int i = 0; i < 100; i++) {
        ColorArr[i] = RandomColor();
    }
}

ImColor TeamColor(int Num)
{
    if(Num < 99 && Num > 0) {
        return ColorArr[Num];
    } else {
        return ImColor(67, 205, 128, 150);
    }
}
#endif
