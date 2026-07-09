#ifndef UNIFIED_DRIVER_H
#define UNIFIED_DRIVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <regex.h>
#include <sys/uio.h>
#include <iostream>
#include <string>
#include <sys/utsname.h>
#include <sys/system_properties.h>
#include <sys/prctl.h>  // 新增：KPM驱动需要

using namespace std;
pid_t pid;
int 选择值 = 0;
int 隐藏 = 0;


// KPM驱动新增的数据结构
struct mem_operation {
    pid_t target_pid;
    uint64_t addr;
    void *buffer;
    uint64_t size;
};

#define PRCTL_MEM_READ  0x63687501
#define PRCTL_MEM_WRITE 0x63687502

// syscall内存读写系统调用号
#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

class c_driver {
private:
    int has_upper = 0;
    int has_lower = 0;
    int has_symbol = 0;
    int has_digit = 0;
    int fd;
    pid_t pid;
    bool kpm_initialized;  // 新增：KPM初始化状态
    bool kpm_is_root;      // 新增：root权限检查

    typedef struct _COPY_MEMORY {
        pid_t pid;
        uintptr_t addr;
        void* buffer;
        size_t size;
    } COPY_MEMORY, *PCOPY_MEMORY;

    typedef struct _MODULE_BASE {
        pid_t pid;
        char* name;
        uintptr_t base;
    } MODULE_BASE, *PMODULE_BASE;

    struct process {
        pid_t process_pid;
        char process_comm[15];
    };

    // 操作码定义
    enum OPERATIONS {
        // Hook驱动的操作码
        HOOK_OP_READ_MEM = 601,
        HOOK_OP_WRITE_MEM = 602,
        HOOK_OP_MODULE_BASE = 603,
        HOOK_OP_HIDE_PROCESS = 605,
        
        // Dev驱动的操作码
        DEV_OP_INIT_KEY = 0x800,
        DEV_OP_READ_MEM = 0x801,
        DEV_OP_WRITE_MEM = 0x802,
        DEV_OP_MODULE_BASE = 0x803,
        DEV_OP_HIDE_PROCESS = 0x804,
        
        // Proc驱动的操作码
        PROC_OP_INIT_KEY = 0xC00,
        PROC_OP_READ_MEM = 0xC01,
        PROC_OP_WRITE_MEM = 0xC02,
        PROC_OP_MODULE_BASE = 0xC03,
        PROC_OP_HIDE_PROCESS = 0xC04,
    };

    // Syscall内存读写函数
    ssize_t process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                      const struct iovec *__remote_iov, unsigned long __remote_iov_count,
                      unsigned long __flags, bool iswrite) {
        return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid,
                       __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
    }

    bool pvm(void *address, void *buffer, size_t size, bool iswrite) {
        struct iovec local[1];
        struct iovec remote[1];
        local[0].iov_base = buffer;
        local[0].iov_len = size;
        remote[0].iov_base = address;
        remote[0].iov_len = size;
        if (this->pid < 0) {
            return false;
        }
        ssize_t bytes = process_v(this->pid, local, 1, remote, 1, 0, iswrite);
        return bytes == size;
    }

    int symbol_file(const char *filename) {
        int length = strlen(filename);
        for (int i = 0; i < length; i++) {
            if (islower(filename[i])) {
                has_lower = 1;
            } else if (isupper(filename[i])) {
                has_upper = 1;
            } else if (ispunct(filename[i])) {
                has_symbol = 1;
            } else if (isdigit(filename[i])) {
                has_digit = 1;
            }
        }
        return has_lower && !has_upper && !has_symbol && !has_digit;
    }

    // RT DEV驱动路径查找
    char *driver_path() {
        const char *dev_path = "/dev";
        DIR *dir = opendir(dev_path);
        if (dir == NULL){
            printf("Cannot open /dev directory\n");
            return NULL;
        }

        char *files[] = { "wanbai", "CheckMe", "Ckanri", "lanran","video188"};
        struct dirent *entry;
        char *file_path = NULL;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            size_t path_length = strlen(dev_path) + strlen(entry->d_name) + 2;
            file_path = (char *)malloc(path_length);
            snprintf(file_path, path_length, "%s/%s", dev_path, entry->d_name);
            for (int i = 0; i < 5; i++) {
                if (strcmp(entry->d_name, files[i]) == 0) {
                    printf("Driver file: %s\n", file_path);
                    closedir(dir);
                    return file_path;
                }
            }

            struct stat file_info;
            if (stat(file_path, &file_info) < 0) {
                free(file_path);
                file_path = NULL;
                continue;
            }

            if (strstr(entry->d_name, "gpiochip") != NULL) {
                free(file_path);
                file_path = NULL;
                continue;
            }

            if ((S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode))
                && strchr(entry->d_name, '_') == NULL && strchr(entry->d_name, '-') == NULL && strchr(entry->d_name, ':') == NULL) {
                if (strcmp(entry->d_name, "stdin") == 0 || strcmp(entry->d_name, "stdout") == 0
                    || strcmp(entry->d_name, "stderr") == 0) {
                    free(file_path);
                    file_path = NULL;
                    continue;
                }
                
                size_t file_name_length = strlen(entry->d_name);
                time_t current_time;
                time(&current_time);
                int current_year = localtime(&current_time)->tm_year + 1900;
                int file_year = localtime(&file_info.st_ctime)->tm_year + 1900;
                if (file_year <= 1980) {
                    free(file_path);
                    file_path = NULL;
                    continue;
                }
                
                time_t atime = file_info.st_atime;
                time_t ctime = file_info.st_ctime;
                if ((atime == ctime)) {
                    if ((file_info.st_mode & S_IFMT) == 8192 && file_info.st_size == 0
                        && file_info.st_gid == 0 && file_info.st_uid == 0 && file_name_length <= 9) {
                        printf("Driver file: %s\n", file_path);
                        closedir(dir);
                        return file_path;
                    }
                }
            }
            free(file_path);
            file_path = NULL;
        }
        closedir(dir);
        return NULL;
    }

    // DEV驱动路径查找
    char *gtqwq() {
        const char *dev_path = "/dev";
        DIR *dir = opendir(dev_path);
        if (dir == NULL){
            printf("Cannot open /dev directory\n");
            return NULL;
        }

        struct dirent *entry;
        char file_path[256];
        while ((entry = readdir(dir)) != NULL) {
            if (strstr(entry->d_name,"std") != NULL || strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strstr(entry->d_name, "gpiochip") != NULL) {
                continue;
            }
            
            if(strchr(entry->d_name, '_') != NULL && strchr(entry->d_name, '-') != NULL && strchr(entry->d_name, ':') != NULL) {
                continue;
            }
            
            sprintf(file_path, "%s/%s", dev_path, entry->d_name);
            
            struct stat file_info;
            if (stat(file_path, &file_info) < 0)
                continue;
                
            if((localtime(&file_info.st_ctime)->tm_year + 1900) <= 1980)
                continue;
            
            if(strlen(entry->d_name) > 7 || strlen(entry->d_name) < 5)
                continue;
            
            if(file_info.st_gid != 0 || file_info.st_uid != 0)
                continue;

            if (S_ISCHR(file_info.st_mode) || S_ISBLK(file_info.st_mode)){					
                if(file_info.st_gid == 0 && file_info.st_uid == 0){
                    printf("%s\n",file_path);
                    char *devpath = (char *)malloc(32);
                    strcpy(devpath,file_path);
                    closedir(dir);
                    return devpath;
                }
            }
        }
        closedir(dir);
        return NULL;
    }
 char *arlo() {
    DIR *dir = opendir("/dev");
    if (!dir) return NULL;
    
    struct dirent *entry;
    char path[256];
    struct stat st;
    
    while ((entry = readdir(dir))) {
        // 跳过不需要的文件
        if (entry->d_name[0] == '.') continue;
        
        // 检查是否以chu开头
        if (strncmp(entry->d_name, "chu", 3) != 0) continue;
        
        snprintf(path, sizeof(path), "/dev/%s", entry->d_name);
        if (stat(path, &st) != 0) continue;
        
        // 一次性判断所有条件
        if ((S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) &&
            (st.st_mode & 0777) == 0600 &&
            st.st_uid == 0 && st.st_gid == 0) {
            
            char *result = (char *)malloc(strlen(path) + 1);
            if (result) strcpy(result, path);
            closedir(dir);
            return result;
        }
    }
    
    closedir(dir);
    return NULL;
}
    // QX驱动相关函数
    char *execCom(const char *shell) {
        FILE *fp = popen(shell, "r");
        if (fp == NULL) {
            perror("popen failed");
            return NULL;
        }
        char buffer[256];
        char *result = (char *)malloc(1000);
        result[0] = '\0';
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            strcat(result, buffer);
        }
        pclose(fp);
        return result;
    }

    int findFirstMatchingPath(const char *path, regex_t *regex, char *result) {
        DIR *dir;
        struct dirent *entry;

        if ((dir = opendir(path)) != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                char fullpath[1024];
                snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
                if (entry->d_type == DT_LNK) {
                    char linkpath[1024];
                    ssize_t len = readlink(fullpath, linkpath, sizeof(linkpath) - 1);
                    if (len != -1) {
                        linkpath[len] = '\0';
                        if (regexec(regex, linkpath, 0, NULL, 0) == 0) {
                            strcpy(result, fullpath);
                            closedir(dir);
                            return 1;
                        }
                    } else {
                        perror("readlink");
                    }
                }
            }
            closedir(dir);
        } else {
            perror("Unable to open directory");
        }
        return 0;
    }

    void createDriverNode(char *path, int major_number, int minor_number) {
        std::string command = "mknod " + std::string(path) + " c " + std::to_string(major_number) + " " + std::to_string(minor_number);
        system(command.c_str());
        printf("\n");
    }

    void removeDeviceNode(char *path) {
        if (unlink(path) == 0) {
        } else {
        }
    }

public:
    c_driver() {
        printf("\033[1;36m[+] 1. RT Hook ProMax\033[0m\n");
        printf("\033[1;36m[+] 2. RT DEV ProMax\033[0m\n");
        printf("\033[1;36m[+] 3. DEV (Most Compatible)\033[0m\n");
        printf("\033[1;36m[+] 4. Arlo dev Driver\033[0m\n");
        printf("\033[1;36m[-] 5. QX10~11.4\033[0m\n");
        printf("\033[1;36m[-] 6. Syscall Memory R/W\033[0m\n");
        printf("\033[1;36m[+] 7. Arlo KPM Driver\033[0m\n");  // Added: KPM Option
        printf("\033[1;33m[?] Select an option (1-7): \033[0m"); 
        scanf("%d", &选择值);
        
        if (选择值 == 1) {
            printf("\033[41m\033[37mRT Hook ProMax connection success\033[0m\n");    
            fd = 0;
        } else if (选择值 == 2) {
            char *device_name = driver_path();
            fd = open(device_name, O_RDWR);
            
            if (fd == -1) {
                printf("\033[41m\033[37mRT DEV ProMax connection error\033[0m\n");
                free(device_name);
                exit(0);
            }
            free(device_name);
        } else if (选择值 == 3) {        
            char *device_name = gtqwq();
            fd = open(device_name, O_RDWR);
            if (fd == -1) {
                printf("\033[41m\033[37mDEV connection error\033[0m\n");
                free(device_name);
                exit(0);
            }
            free(device_name);
        } else if (选择值 == 4) {
         char *device_name = arlo();
            fd = open(device_name, O_RDWR);
          
        } else if (选择值 == 5) {
            char *output = execCom("ls -l /proc/*/exe 2>/dev/null | grep -E \"/data/[a-z]{6} \\(deleted\\)\"");
            char filePath[256];
            char pid[56];
            if (output != NULL) {
                char *procStart = strstr(output, "/proc/");
                char *pidStart = procStart + 6;
                char *pidEnd = strchr(pidStart, '/');
                strncpy(pid, pidStart, pidEnd - pidStart);
                pid[pidEnd - pidStart] = '\0';

                char *arrowStart = strstr(output, "->");
                char *start = arrowStart + 3;
                char *end = strchr(output, '(') - 1;
                strncpy(filePath, start, end - start + 1);
                filePath[end - start] = '\0';

                char *replacePtr = strstr(filePath, "data");
                if (replacePtr != NULL) {
                    memmove(replacePtr + 2, replacePtr + 3, strlen(replacePtr + 3) + 1);
                    memmove(replacePtr, "dev", strlen("dev"));
                }
            } else {
                printf("Error executing scripts.\n");
            }

            char fdPath[256];
            char pattern[100];
            snprintf(pattern, sizeof(pattern), ".*%s.*", filePath + 5);
            int major_number = 0;
            int minor_number = 0;
            snprintf(fdPath, sizeof(fdPath), "/proc/%s/fd", pid);
            regex_t regex;
            if (regcomp(&regex, pattern, 0) != 0) {
                fprintf(stderr, "Failed to compile regex\n");
            }

            char result[1024];
            if (findFirstMatchingPath(fdPath, &regex, result)) {
                char cmd[256];
                sprintf(cmd, "ls -AL -l  %s | grep -Eo '[0-9]{3},' | grep  -Eo '[0-9]{3}'", result);
                char *fdInfo = execCom(cmd);
                fdInfo[strlen(fdInfo)-1] = '\0';
                major_number = atoi(fdInfo);
                free(fdInfo);
            } else {
                printf("No matching path found.\n");
            }
            regfree(&regex);

            if (filePath[0] != '\0') {
                createDriverNode(filePath, major_number, 0);
                fd = open(filePath, O_RDWR);
                if (fd == -1) {
                    removeDeviceNode(filePath);
                } else {
                    removeDeviceNode(filePath);
                }
            }
        } else if (选择值 == 6) {
            printf("\033[41m\033[37mSyscall memory read/write mode enabled\033[0m\n");
            fd = -1;
        } else if (选择值 == 7) {
            // 新增：KPM驱动模式
            printf("\033[41m\033[37mKPM driver mode enabled\033[0m\n");
            fd = -2;
            kpm_is_root = (getuid() == 0);
            kpm_initialized = false;
            if (!kpm_is_root) {
                printf("\033[41m\033[37mWarning: KPM driver requires root permission!\033[0m\n");
            }
        } else {
            printf("\033[41m\033[37mInvalid selection; switched to Hook connection\033[0m\n");
            fd = 0;
        }
        system("clear");
    }

    ~c_driver() {
        if (fd > 0)
            close(fd);
    }

    void initialize(pid_t pid) {
        this->pid = pid;
        if (选择值 == 7) {
            kpm_initialized = (pid > 0);
        }
    }

    bool mem_addr_virtophy(unsigned long vaddr) {
        int fh = 1;
        int pageSize = getpagesize();

        unsigned long v_pageIndex = vaddr / pageSize;
        unsigned long pfn_item_offset = v_pageIndex * sizeof(uint64_t);
        unsigned long page_offset = vaddr % pageSize;
        uint64_t item = 0;
        char filename[32];
        snprintf(filename, sizeof(filename), "/proc/%d/pagemap", this->pid);
        int fd = open(filename, O_RDONLY);
        if (fd < 0) {
            return false;
        }

        if (lseek(fd, pfn_item_offset, SEEK_SET) < 0) {
            close(fd);
            return false;
        }

        if (read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t)) {
            close(fd);
            return false;
        }

        if (0 == (item & (((uint64_t) 1) << 63))) {
            close(fd);
            return false;
        }
        close(fd);
        return true;
    }

    bool init_key(char* key) {
        if (选择值 == 2 || 选择值 == 3 || 选择值 == 4 || 选择值 == 5) {
            char buf[0x100];
            strcpy(buf,key);
            int opcode;
            
            if (选择值 == 2 || 选择值 == 3 || 选择值 == 5) {
                opcode = DEV_OP_INIT_KEY;
            } else if (选择值 == 4) {
                opcode = PROC_OP_INIT_KEY;
            }
            
            if (ioctl(fd, opcode, buf) != 0) {
                return false;
            }
            return true;
        }
        return false;
    }

    // ==================== 非模板版本（3个参数）====================
    bool read(uintptr_t addr, void *buffer, size_t size) {
        if (选择值 == 6) {
            return pvm(reinterpret_cast<void *>(addr), buffer, size, false);
        } else if (选择值 == 7) {
            // 新增：KPM模式使用prctl
            if (!kpm_initialized || this->pid <= 0) {
                return false;
            }
            
            struct mem_operation op;
            op.target_pid = this->pid;
            op.addr = addr;
            op.buffer = buffer;
            op.size = size;
            
            return prctl(PRCTL_MEM_READ, (unsigned long)&op, 0, 0, 0) == 0;
        } else {
            COPY_MEMORY cm;
            cm.pid = this->pid;
            cm.addr = addr;
            cm.buffer = buffer;
            cm.size = size;

            int opcode;
            if (选择值 == 1) {
                opcode = HOOK_OP_READ_MEM;
            } else if (选择值 == 2 || 选择值 == 3 || 选择值 == 5) {
                opcode = DEV_OP_READ_MEM;
            } else if (选择值 == 4) {
                opcode = PROC_OP_READ_MEM;
            }

            if (ioctl(fd, opcode, &cm) != 0) {
                return false;
            }
            return true;
        }
    }

    bool write(uintptr_t addr, void *buffer, size_t size) {
        if (选择值 == 6) {
            return pvm(reinterpret_cast<void *>(addr), buffer, size, true);
        } else if (选择值 == 7) {
            // 新增：KPM模式使用prctl
            if (!kpm_initialized) return false;
            
            struct mem_operation op;
            op.target_pid = this->pid;
            op.addr = addr;
            op.buffer = buffer;
            op.size = size;
            
            return prctl(PRCTL_MEM_WRITE, (unsigned long)&op, 0, 0, 0) == 0;
        } else {
            COPY_MEMORY cm;
            cm.pid = this->pid;
            cm.addr = addr;
            cm.buffer = buffer;
            cm.size = size;

            int opcode;
            if (选择值 == 1) {
                opcode = HOOK_OP_WRITE_MEM;
            } else if (选择值 == 2 || 选择值 == 3 || 选择值 == 5) {
                opcode = DEV_OP_WRITE_MEM;
            } else if (选择值 == 4) {
                opcode = PROC_OP_WRITE_MEM;
            }

            if (ioctl(fd, opcode, &cm) != 0) {
                return false;
            }
            return true;
        }
    }

    // ==================== 模板版本（1个参数）====================
    template <typename T>
    T read(uintptr_t addr) {
        T res{};
        
        if (选择值 == 7) {
            // KPM模式：与原版kpmdriverTearGame.h完全一致
            if (kpm_initialized && this->pid > 0) {
                // 调用非模板版本
                this->read(addr, &res, sizeof(T));
            }
        } else {
            // 其他模式：检查返回值
            if (this->read(addr, &res, sizeof(T))) {
                return res;
            }
        }
        return res;
    }

    template <typename T>
    bool write(uintptr_t addr, T value) {
        if (选择值 == 7 && !kpm_initialized) return false;
        return this->write(addr, &value, sizeof(T));
    }

    uintptr_t getModuleBase(char* name) {
        if (选择值 == 1 || 选择值 == 6) {
            FILE* fp;
            char cmd[0x100] = "";
            uintptr_t ret = 0;
            
            snprintf(cmd, sizeof(cmd), "ls -l /proc/%d/map_files/ | grep '%s'", pid, name);
            fp = popen(cmd, "r");
            if (!fp) {
                std::cerr << "Failed to run command" << std::endl;
                return 0;
            }
            fscanf(fp,"%*s %*d %*s %*s %*d %*s %*s %lx-%*lx", &ret);
            pclose(fp);
            return ret;
        } else if (选择值 == 2 || 选择值 == 3 || 选择值 == 4 || 选择值 == 5) {
            MODULE_BASE mb;
            char buf[0x100];
            strcpy(buf, name);
            mb.pid = this->pid;
            mb.name = buf;

            int opcode;
            if (选择值 == 2 || 选择值 == 3 || 选择值 == 5) {
                opcode = DEV_OP_MODULE_BASE;
            } else if (选择值 == 4) {
                opcode = PROC_OP_MODULE_BASE;
            }

            if (ioctl(fd, opcode, &mb) != 0) {
                return 0;
            }
            return mb.base;
        } else if (选择值 == 7) {
            // KPM模式：使用和模式1、6一样的方法读取模块基址
            FILE* fp;
            char cmd[0x100] = "";
            uintptr_t ret = 0;
            
            snprintf(cmd, sizeof(cmd), "ls -l /proc/%d/map_files/ | grep '%s'", pid, name);
            fp = popen(cmd, "r");
            if (!fp) {
                std::cerr << "Failed to run command" << std::endl;
                return 0;
            }
            fscanf(fp,"%*s %*d %*s %*s %*d %*s %*s %lx-%*lx", &ret);
            pclose(fp);
            return ret;
        }
        return 0;
    }

    void hide_process() {
        if (选择值 == 1) {
            ioctl(fd, HOOK_OP_HIDE_PROCESS);
        } else if (选择值 == 2 || 选择值 == 3 || 选择值 == 5) {
            ioctl(fd, DEV_OP_HIDE_PROCESS);
        } else if (选择值 == 4) {
            ioctl(fd, PROC_OP_HIDE_PROCESS);
        }
    }
    
    // 新增：简单的状态查询
/*    bool is_kpm_initialized() const {
        return (选择值 == 7) && kpm_initialized;
    }
    
    bool is_kpm_root() const {
        return (选择值 == 7) && kpm_is_root;
    }*/
};

static c_driver *driver = new c_driver();
#endif