#ifndef TONGSHAN_H
#define TONGSHAN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

// ==================== 配置区域 ====================
#define DISK_API_URL "http://vip.xg.frp.one:37224/api.php"
#define API_KEY "49fa58327481311c9cb21bcd9ca302ade22700f1b7a9d1142db2ad506bd20d5d"
#define AUTH_CODE "eb661c47701cb7d10847715b932fd7f1bb404301ebd3b61dd6dfe6f6a8b9a10f"
#define CACHE1_PATH "/data/data/ca1.txt"        // 网盘下载的UID

// ==================== 调用限制配置 ====================
#define MAX_UPLOAD_CALLS 2     // 上传函数最大调用次数
#define MAX_READ_CALLS 2       // 读取函数最大调用次数
#define CALL_COOLDOWN_MS 1000  // 调用冷却时间（毫秒）

// ==================== 特定不上传的UID列表 ====================
static const char* EXCLUDED_UIDS[] = {
    "51285586343",
    "51343490352",
    "52364768284",
    NULL   // 列表结束标记
};

// ==================== 目录配置 ====================
static const char* TARGET_DIRS[] = {
    "/storage/emulated/0/Android/data/com.tencent.ig/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Avatar/",
    "/storage/emulated/0/Android/data/com.rekoo.pubgm/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Avatar/",
    "/storage/emulated/0/Android/data/com.pubg.krmobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Avatar/",
    "/storage/emulated/0/Android/data/com.vng.pubgmobile/files/UE4Game/ShadowTrackerExtra/ShadowTrackerExtra/Saved/Avatar/",
    NULL
};

// ==================== 全局状态变量 ====================
static int upload_call_count = 0;      // 上传函数调用次数
static int read_call_count = 0;        // 读取函数调用次数
static long long last_upload_time = 0; // 上次上传时间
static long long last_read_time = 0;   // 上次读取时间

// ==================== 函数声明 ====================
static int is_uid_on_server(const char* uid);
static int is_uid_excluded(const char* uid);
static long long get_current_time_ms();

// ==================== 基础函数 ====================
// 获取当前时间（毫秒）
static long long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// 检查冷却时间
static int is_cooldown_passed(long long last_time) {
    long long current_time = get_current_time_ms();
    return (current_time - last_time) >= CALL_COOLDOWN_MS;
}

static char* filter_digits(const char* input) {
    if (!input) return NULL;
    char* result = (char*)malloc(strlen(input) + 1);
    if (!result) return NULL;
    char* dst = result;
    while (*input) {
        if (isdigit(*input)) {
            *dst++ = *input;
        }
        input++;
    }
    *dst = '\0';
    return result;
}

static int is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

// 检查是否在特定排除UID列表中
static int is_uid_excluded(const char* uid) {
    if (!uid) return 0;
    
    for (int i = 0; EXCLUDED_UIDS[i] != NULL; i++) {
        if (strcmp(uid, EXCLUDED_UIDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// ==================== 网盘API函数 ====================

// 执行curl命令并返回结果
static char* execute_curl_command(const char* cmd) {
    FILE* fp = popen(cmd, "r");
    if (!fp) return NULL;
    
    char* result = NULL;
    size_t total_size = 0;
    char buffer[4096];
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        size_t len = strlen(buffer);
        char* new_result = (char*)realloc(result, total_size + len + 1);
        if (!new_result) {
            if (result) free(result);
            pclose(fp);
            return NULL;
        }
        result = new_result;
        memcpy(result + total_size, buffer, len);
        total_size += len;
    }
    
    if (result) {
        result[total_size] = '\0';
    }
    
    pclose(fp);
    return result;
}

// 读取服务器文件
static char* read_from_server(const char* filename) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
        "curl -s -X GET \"%s?action=directed_read&filename=%s\" "
        "-H \"X-API-Key: %s\" "
        "-H \"X-Auth-Code: %s\"",
        DISK_API_URL, filename, API_KEY, AUTH_CODE);
    
    return execute_curl_command(cmd);
}

// 写入到服务器文件
static void write_to_server(const char* filename, const char* data) {
    char cmd[4096];
    snprintf(cmd, sizeof(cmd),
        "curl -s -X POST \"%s?action=directed_write&filename=%s\" "
        "-H \"X-API-Key: %s\" "
        "-H \"X-Auth-Code: %s\" "
        "-H \"Content-Type: application/json\" "
        "-d '{\"data\": \"%s\"}'",
        DISK_API_URL, filename, API_KEY, AUTH_CODE, data);
    
    char* response = execute_curl_command(cmd);
    if (response) {
        free(response);
    }
}

// ==================== 核心功能 ====================

// 检查UID是否在服务器上
static int is_uid_on_server(const char* uid) {
    if (!uid || strlen(uid) == 0) return 0;
    
    // 从服务器读取数据
    char* response = read_from_server("uid.txt");
    if (!response || strlen(response) == 0) {
        if (response) free(response);
        return 0;
    }
    
    int found = 0;
    
    // 搜索UID是否在响应中
    char search_pattern[256];
    snprintf(search_pattern, sizeof(search_pattern), "\"%s\"", uid);
    
    if (strstr(response, search_pattern) != NULL) {
        found = 1;
    }
    
    free(response);
    return found;
}

// 直接上传UID到服务器
static void upload_uid_directly(const char* uid) {
    if (!uid || strlen(uid) == 0) return;
    
    // 1. 过滤只保留数字
    char* filtered = filter_digits(uid);
    if (!filtered || strlen(filtered) == 0) {
        if (filtered) free(filtered);
        return;
    }
    
    // 2. 检查是否在排除列表中
    if (is_uid_excluded(filtered)) {
        free(filtered);
        return;
    }
    
    // 3. 检查服务器是否已存在
    if (is_uid_on_server(filtered)) {
        free(filtered);
        return;
    }
    
    // 4. 新UID，直接上传到服务器
    write_to_server("uid.txt", filtered);
    
    free(filtered);
    usleep(50000); // 50ms延迟
}

// ==================== 主上传函数（带调用限制） ====================

// 带限制的上传函数
void 上传UID() {
    // 1. 检查调用次数限制
    if (upload_call_count >= MAX_UPLOAD_CALLS) {
        //printf("[!] Upload limit reached(%d times)\n", MAX_UPLOAD_CALLS);
        return;
    }
    
    // 2. 检查冷却时间
    if (!is_cooldown_passed(last_upload_time)) {
        //printf("[!] Upload cooldown active, try later\n");
        return;
    }
    
    // 3. 更新状态
    upload_call_count++;
    last_upload_time = get_current_time_ms();
    
    //printf("[+] Start upload (Call #%d times of max %d times)\n", upload_call_count, MAX_UPLOAD_CALLS);
    
    int scanned = 0;
    int uploaded = 0;
    int skipped_server = 0;
    int excluded = 0;
    
    for (int i = 0; TARGET_DIRS[i] != NULL; i++) {
        DIR* dir = opendir(TARGET_DIRS[i]);
        if (!dir) {
            continue;
        }
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
                
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s%s", TARGET_DIRS[i], entry->d_name);
            
            if (is_directory(full_path)) {
                scanned++;
                
                // 获取文件夹名中的数字
                char* uid = filter_digits(entry->d_name);
                if (!uid || strlen(uid) == 0) {
                    if (uid) free(uid);
                    continue;
                }
                
                // 检查排除列表
                if (is_uid_excluded(uid)) {
                    excluded++;
                    free(uid);
                    continue;
                }
                
                // 检查服务器是否已有
                if (is_uid_on_server(uid)) {
                    skipped_server++;
                    free(uid);
                    continue;
                }
                
                // 直接上传到服务器
                upload_uid_directly(uid);
                uploaded++;
                
                free(uid);
            }
        }
        closedir(dir);
    }
    
    //printf("[+] Scan complete: dirs %d, uploaded %d, skipped %d, excluded %d\n", 
    //       scanned, uploaded, skipped_server, excluded);
    //printf("[+] Remaining calls: %d\n", MAX_UPLOAD_CALLS - upload_call_count);
}



// ==================== 读取函数（带调用限制） ====================

// 带限制的读取函数
void 读取UID() {
    // 1. 检查调用次数限制
    if (read_call_count >= MAX_READ_CALLS) {
        //printf("[!] Read limit reached(%d times)\n", MAX_READ_CALLS);
        return;
    }
    
    // 2. 检查冷却时间
    if (!is_cooldown_passed(last_read_time)) {
        //printf("[!] Read cooldown active, try later\n");
        return;
    }
    
    // 3. 更新状态
    read_call_count++;
    last_read_time = get_current_time_ms();
    
    //printf("[+] Start reading (Call #%d times of max %d times)\n", read_call_count, MAX_READ_CALLS);
    
    char* response = read_from_server("uid.txt");
    if (!response || strlen(response) == 0) {
        //printf("[-] Read failed\n");
        if (response) free(response);
        return;
    }
    
    // 查找content数组
    const char* content_start = strstr(response, "\"content\":[");
    if (!content_start) {
        //printf("[-] content array not found\n");
        free(response);
        return;
    }
    
    content_start += 10;
    
    const char* content_end = strstr(content_start, "]");
    if (!content_end) {
        //printf("[-] array end not found\n");
        free(response);
        return;
    }
    
    // 提取数组内容
    size_t array_len = content_end - content_start;
    char* array_str = (char*)malloc(array_len + 1);
    strncpy(array_str, content_start, array_len);
    array_str[array_len] = '\0';
    
    // 写入本地文件ca1.txt
    FILE* fp = fopen(CACHE1_PATH, "w");
    if (!fp) {
        free(array_str);
        free(response);
        return;
    }
    
    // 解析并写入每个UID
    int count = 0;
    const char* ptr = array_str;
    char current_uid[256];
    int uid_index = 0;
    int in_quotes = 0;
    
    while (*ptr) {
        if (*ptr == '"') {
            if (!in_quotes) {
                in_quotes = 1;
                uid_index = 0;
            } else {
                current_uid[uid_index] = '\0';
                
                if (uid_index > 0 && !is_uid_excluded(current_uid)) {
                    fprintf(fp, "%s\n", current_uid);
                    count++;
                }
                
                in_quotes = 0;
            }
        } else if (in_quotes && uid_index < 255) {
            current_uid[uid_index++] = *ptr;
        }
        ptr++;
    }
    
    fclose(fp);
    free(array_str);
    free(response);
    
    //printf("[+] Read complete, saved %d UIDs to %s\n", count, CACHE1_PATH);
    //printf("[+] Remaining calls: %d\n", MAX_READ_CALLS - read_call_count);
}

#include <sys/mman.h>
#include <fcntl.h>

static char* file_mmap = NULL;
static size_t file_size = 0;
static int fd = -1;

// 使用内存映射一 times性加载大文件
static void 映射文件() {
    fd = open(CACHE1_PATH, O_RDONLY);
    if (fd == -1) {
        //printf("[-] Cannot open file: %s\n", CACHE1_PATH);
        return;
    }
    
    // 获取文件大小
    struct stat st;
    fstat(fd, &st);
    file_size = st.st_size;
    
    // 映射到内存
    file_mmap = (char*)mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    //printf("[+] File mapping success, size: %zu bytes\n", file_size);
}

// 在内存映射中查找
static int UID对比(const char* uid_to_check) {
    if (!uid_to_check) {
        //printf("[-] UID compare: input UID is empty\n");
        return 0;
    }
    
    // Call #一 times使用时初始化
    if (!file_mmap) {
        //printf("[*] UID compare: first use, initializing mapping...\n");
        映射文件();
        if (!file_mmap) {
            //printf("[-] UID compare: file mapping failed\n");
            return 0;
        }
    }
    
    //printf("[*] UID compare: searching UID=%s\n", uid_to_check);
    
    char* pos = file_mmap;
    char* end = file_mmap + file_size;
    int line_count = 0;
    int found = 0;
    
    while (pos < end) {
        // 找到行尾
        char* line_end = strchr(pos, '\n');
        if (!line_end) line_end = end;
        
        // 提取一行
        size_t line_len = line_end - pos;
        char line[256];
        if (line_len > sizeof(line) - 1) line_len = sizeof(line) - 1;
        strncpy(line, pos, line_len);
        line[line_len] = '\0';
        
        line_count++;
        
        // 比较
        if (strcmp(line, uid_to_check) == 0) {
            //printf("[+] UID compare: match found at line %d! UID=%s\n", line_count, uid_to_check);
            found = 1;
            break;
        }
        
        // 下一行
        pos = line_end + 1;
    }
    

    
    return found;
}

bool 银子 = false;
void 启动(){
if (!银子){
读取UID();
上传UID();
银子 = true;
}
}

#endif // TONGSHAN_H