#ifndef ZEUS_AUTH_H
#define ZEUS_AUTH_H

#include "res/Util.h"
#include "../AES_CBC.h"
#include <iomanip>
#include <chrono>
#include <thread>
#include <signal.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

// Global State
extern bool g_IsLoggedIn;
extern long ExpiryTime;
extern bool IsUpdateAvailable;
extern std::string updateurl;
extern std::string updateshow;

// Shared state with LoginUI
inline bool g_AuthIsLoggedIn = false;
inline long g_ExpiryTime = 0;
inline std::string g_ExpiryDateStr = "NULL";
inline std::string g_Announcement = "";

// ZEUS KERNEL Offline Auth Config
// Active offline keys: ZEUS / zeus
// Expiry policy: Lifetime access. We keep a far-future timestamp for any old expiry checks.
const string g_OfflineKeyPrimary = "ZEUS";
const string g_OfflineKeySecondary = "zeus";
const string g_KmDir = "/storage/emulated/0/SdCard/Keys";
const string g_KmPath = "/storage/emulated/0/SdCard/Keys/zeus_kernel.txt";
const long g_ZeusLifetimeExpiry = 4102444799L; // 2099-12-31 23:59:59, treated/displayed as LIFETIME

inline std::string TrimOfflineKey(const std::string& value) {
    size_t first = 0;
    while (first < value.size() && (value[first] == ' ' || value[first] == '\t' || value[first] == '\r' || value[first] == '\n')) {
        ++first;
    }

    size_t last = value.size();
    while (last > first && (value[last - 1] == ' ' || value[last - 1] == '\t' || value[last - 1] == '\r' || value[last - 1] == '\n')) {
        --last;
    }

    return value.substr(first, last - first);
}

inline bool IsValidOfflineZeusKey(const std::string& key) {
    const std::string normalizedKey = TrimOfflineKey(key);
    return normalizedKey == g_OfflineKeyPrimary || normalizedKey == g_OfflineKeySecondary;
}

inline void ActivateZeusLifetimeAccess(const std::string& key) {
    g_IsLoggedIn = true;
    g_AuthIsLoggedIn = true;
    g_ExpiryTime = g_ZeusLifetimeExpiry;
    ExpiryTime = g_ZeusLifetimeExpiry;
    g_ExpiryDateStr = "LIFETIME";
    g_Announcement = "ZEUS KERNEL Offline Lifetime Access";
    IsUpdateAvailable = false;
    updateurl = "";
    updateshow = "";

    // Save only the offline key locally. No network request is performed.
    createDirectory(g_KmDir);
    ofstream file(g_KmPath);
    if (file.is_open()) {
        file << TrimOfflineKey(key) << endl;
        file.close();
    }
}

inline bool LoadSavedSession(std::string& outKey) {
    ifstream file(g_KmPath);
    if (file.is_open()) {
        getline(file, outKey);
        file.close();
        outKey = TrimOfflineKey(outKey);
        if (IsValidOfflineZeusKey(outKey)) {
            ActivateZeusLifetimeAccess(outKey);
            return true;
        }
    }
    return false;
}

inline bool PerformNetworkLogin(const std::string& key, std::string& outMessage) {
    const std::string normalizedKey = TrimOfflineKey(key);

    if (normalizedKey.empty()) {
        outMessage = "Offline key cannot be empty";
        return false;
    }

    if (!IsValidOfflineZeusKey(normalizedKey)) {
        outMessage = "Invalid offline key";
        return false;
    }

    ActivateZeusLifetimeAccess(normalizedKey);
    outMessage = "ZEUS KERNEL offline lifetime access enabled";
    return true;
}

// -----------------------------------------------------------------------------
// ONLINE ZPANEL AUTH SYSTEM IS DISABLED FOR NOW.
// It is intentionally kept below with #if 0 / #endif so it can be restored later.
// To restore the online system later, move/merge this block back and remove the
// offline PerformNetworkLogin() above.
// -----------------------------------------------------------------------------
#if 0
// ZPanel Config (Mirrored from app's Login.h)
const string g_ZPanelUrl = "https://zpanel.space/connect";
const string g_AES_Key = "b591nKHFOnXpwu0W";
const string g_AES_Iv = "zcN3ZOg4uhjw6H9S";
const string g_GameName = "PUBG";
const string g_KmPath = "/storage/emulated/0/SdCard/Keys/com.spy.txt";

// Utility to run shell command and get output
inline std::string ExecShell(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

inline bool LoadSavedSession(std::string& outKey) {
    ifstream file(g_KmPath);
    if (file.is_open()) {
        getline(file, outKey);
        file.close();
        return !outKey.empty();
    }
    return false;
}

inline bool PerformNetworkLogin(const std::string& key, std::string& outMessage) {
    if (key.empty()) {
        outMessage = "Key cannot be empty";
        return false;
    }

    // 1. Prepare Request Body
    string serial = y23e98d6a5239eaa6c6626ed7d8a9458b(key); // MD5 of key
    string postData = "game=" + g_GameName + "&user_key=" + key + "&serial=" + serial;

    // 2. Perform Request (Try multiple tools for robustness)
    string response = "";
    
    // List of commands to try: {command, POST_argument_format}
    std::vector<std::string> cmds = {
        "curl -k -L -s -X POST -d \"%s\" \"%s\" 2>/dev/null",
        "/system/bin/curl -k -L -s -X POST -d \"%s\" \"%s\" 2>/dev/null",
        "wget --no-check-certificate -q -O- --post-data=\"%s\" \"%s\" 2>/dev/null",
        "/system/bin/toybox wget --no-check-certificate -q -O- --post-data=\"%s\" \"%s\" 2>/dev/null"
    };

    for (const auto& cmdTmpl : cmds) {
        char cmd[2048];
        snprintf(cmd, sizeof(cmd), cmdTmpl.c_str(), postData.c_str(), g_ZPanelUrl.c_str());
        response = ExecShell(cmd);
        if (!response.empty()) break;
    }

    // Attempt 4: "Proper" Fix - Android Broadcast Bridge
    if (response.empty()) {
        const char* resPath = "/data/data/com.spy/files/esp_res.txt";
        remove(resPath);
        
        printf("\033[1;36m[BRIDGE]\033[0m Requesting network access via App...\n");
        char broadcastCmd[2048];
        // --user 0 ensures it goes to the primary user's app instance
        snprintf(broadcastCmd, sizeof(broadcastCmd), "am broadcast --user 0 -a com.spy.NET_REQ --es url \"%s\" --es body \"%s\" > /dev/null", 
                g_ZPanelUrl.c_str(), postData.c_str());
        system(broadcastCmd);
        
        // Wait for result file (max 10s)
        for (int i = 0; i < 50; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            std::ifstream f(resPath);
            if (f.is_open()) {
                std::stringstream ss;
                ss << f.rdbuf();
                response = ss.str();
                f.close();
                remove(resPath);
                printf("\033[1;32m[BRIDGE]\033[0m Response received from App.\n");
                break;
            }
        }
    }

    if (response.empty()) {
        outMessage = "Networking error: All methods failed (Broadcast/Curl/Wget).\nCheck if app has internet permission!";
        return false;
    }
    
    // Check for Bridge Error
    if (response.find("ERR:") == 0) {
        outMessage = "Bridge Error: " + response.substr(4);
        return false;
    }
    
    // Decrypt Response
    // ... continues below as before

    // 3. Decrypt Response
    // Step A: Base64 Decode
    string b1 = c696cf413dc7a0e92419fa5626dea9bab(response);
    if (b1.empty()) {
        outMessage = "Invalid server response";
        return false;
    }

    // Step B: AES-128-CBC Decrypt
    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, (uint8_t*)g_AES_Key.c_str(), (uint8_t*)g_AES_Iv.c_str());
    
    std::vector<uint8_t> buffer(b1.begin(), b1.end());
    // Length must be multiple of 16 for CBC
    if (buffer.size() % 16 != 0) {
        outMessage = "Decryption error (Length)";
        return false;
    }

    AES_CBC_decrypt_buffer(&ctx, buffer.data(), buffer.size());
    
    // Remove padding
    int decryptedLen = aes_pkcs7_unpad(buffer.data(), buffer.size());
    if (decryptedLen <= 0) {
        outMessage = "Padding error";
        return false;
    }
    string b2((char*)buffer.data(), decryptedLen);

    // Step C: Decoding the Base64 result of AES
    string jsonStr = c696cf413dc7a0e92419fa5626dea9bab(b2);
    if (jsonStr.empty()) {
        // Fallback: sometimes it might not be double encoded depending on server version
        jsonStr = b2;
    }

    // 4. Parse JSON (Safe parse without exceptions)
    json res = json::parse(jsonStr, nullptr, false);
    if (res.is_discarded()) {
        outMessage = "JSON Parse Error";
        return false;
    }
    
    bool success = false;
    if (res.contains("status")) {
        if (res["status"].is_boolean()) success = res["status"].get<bool>();
        else if (res["status"].is_string()) success = (res["status"].get<string>() == "true");
    }

    if (success) {
        json data = res["data"];
        string expiry = data.value("EXP", data.value("Exp", "NULL"));
        g_ExpiryDateStr = expiry;
        
        // Parse expiry date to timestamp
        if (expiry != "NULL") {
            struct tm t = {0};
            if (sscanf(expiry.c_str(), "%d-%d-%d %d:%d:%d", 
                       &t.tm_year, &t.tm_mon, &t.tm_mday, 
                       &t.tm_hour, &t.tm_min, &t.tm_sec) == 6) {
                t.tm_year -= 1900;
                t.tm_mon -= 1;
                g_ExpiryTime = mktime(&t);
                ExpiryTime = g_ExpiryTime;
            }
        }

        g_Announcement = res.value("announcement", res.value("msg", "Welcome!"));
        
        // Save Session
        ofstream file(g_KmPath);
        if (file.is_open()) {
            file << key << endl;
            file.close();
        }

        g_AuthIsLoggedIn = true;
        outMessage = "Success";
        return true;
    } else {
        outMessage = res.value("reason", "Invalid Key");
        return false;
    }
}

#endif // ONLINE ZPANEL AUTH SYSTEM DISABLED

// Deprecated
inline void 卡密() {}

#include "ItemsId.h"

#endif // ZEUS_AUTH_H