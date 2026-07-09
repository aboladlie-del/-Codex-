#pragma once
#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include "imgui.h"
#include <string>
#include <functional>
#include <thread>
#include <chrono>
#include "Other/Auth.h"
#include <thread>
#include <chrono>

enum class LoginStatus {
    IDLE,
    CHECKING,
    SUCCESS,
    FAILED
};

struct LoginState {
    char userKey[128] = "";
    LoginStatus status = LoginStatus::IDLE;
    std::string message = "";
    bool rememberMe = true;
};

static LoginState g_LoginState;

// Forward declaration from Auth.h or similar
extern bool PerformNetworkLogin(const std::string& key, std::string& outMessage);
extern bool LoadSavedSession(std::string& outKey);

namespace LoginUI {

    static ImVec4 MixColor(ImVec4 a, ImVec4 b, float t) {
        return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
    }

    static ImVec4 Alpha(ImVec4 c, float a) {
        c.w = a;
        return c;
    }

    static float ClampFloat(float v, float mn, float mx) {
        if (v < mn) return mn;
        if (v > mx) return mx;
        return v;
    }

    static bool PremiumButton(const char* label, ImVec2 size, ImVec4 base, ImVec4 hover, ImVec4 active) {
        ImGui::PushStyleColor(ImGuiCol_Button, base);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
        bool clicked = ImGui::Button(label, size);
        ImGui::PopStyleColor(3);
        return clicked;
    }

    static void StatusPill(const char* text, ImVec4 color) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        float w = ImGui::CalcTextSize(text).x + 42.0f;
        float h = 32.0f;
        ImGui::InvisibleButton(text, ImVec2(w, h));
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), IM_COL32(255, 255, 255, 18), h * 0.5f);
        dl->AddCircleFilled(ImVec2(p.x + 17.0f, p.y + h * 0.5f), 5.5f, ImGui::GetColorU32(color));
        dl->AddText(ImVec2(p.x + 30.0f, p.y + 8.0f), IM_COL32(235, 242, 255, 255), text);
    }

    void RenderLoginPanel(bool* isLoggedIn) {
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 screenSize = io.DisplaySize;
        ImVec4 bg       = ImVec4(0.018f, 0.024f, 0.040f, 0.98f);
        ImVec4 panel    = ImVec4(0.038f, 0.052f, 0.086f, 0.96f);
        ImVec4 card     = ImVec4(0.060f, 0.082f, 0.130f, 0.96f);
        ImVec4 accent   = ImVec4(0.000f, 0.640f, 1.000f, 1.0f);
        ImVec4 accent2  = ImVec4(0.110f, 0.920f, 1.000f, 1.0f);
        ImVec4 text     = ImVec4(0.94f, 0.98f, 1.00f, 1.0f);
        ImVec4 muted    = ImVec4(0.57f, 0.66f, 0.78f, 1.0f);
        ImVec4 success  = ImVec4(0.13f, 0.92f, 0.54f, 1.0f);
        ImVec4 danger   = ImVec4(1.00f, 0.20f, 0.26f, 1.0f);
        ImVec4 warning  = ImVec4(1.00f, 0.72f, 0.22f, 1.0f);

        float winWidth = ClampFloat(screenSize.x * 0.72f, 720.0f, 920.0f);
        float winHeight = ClampFloat(screenSize.y * 0.66f, 470.0f, 590.0f);
        ImGui::SetNextWindowPos(ImVec2((screenSize.x - winWidth) * 0.5f, (screenSize.y - winHeight) * 0.5f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, bg);
        ImGui::PushStyleColor(ImGuiCol_Border, Alpha(accent, 0.45f));
        ImGui::PushStyleColor(ImGuiCol_Text, text);
        ImGui::PushStyleColor(ImGuiCol_TextDisabled, muted);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, card);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, MixColor(card, accent, 0.18f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, MixColor(card, accent, 0.28f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 28.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(16, 14));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(14, 14));

        ImGui::Begin("ZEUS KERNEL Login", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 wp = ImGui::GetWindowPos();
        ImVec2 ws = ImGui::GetWindowSize();
        dl->AddRectFilled(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), ImGui::GetColorU32(MixColor(bg, accent, 0.045f)), 28.0f);
        dl->AddRect(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), ImGui::GetColorU32(Alpha(accent, 0.45f)), 28.0f, 0, 2.0f);
        dl->AddCircleFilled(ImVec2(wp.x + winWidth - 94.0f, wp.y + 78.0f), 74.0f, ImGui::GetColorU32(Alpha(accent2, 0.12f)));
        dl->AddCircle(ImVec2(wp.x + winWidth - 94.0f, wp.y + 78.0f), 104.0f, ImGui::GetColorU32(Alpha(accent, 0.16f)), 80, 2.0f);

        ImGui::SetCursorPos(ImVec2(32, 28));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, Alpha(card, 0.74f));
        ImGui::BeginChild("##login_brand", ImVec2(winWidth - 64.0f, 118.0f), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::SetCursorPos(ImVec2(24, 20));
        ImGui::TextColored(accent2, "ZEUS");
        ImGui::SameLine();
        ImGui::TextColored(text, "ZEUS KERNEL");
        ImGui::SetCursorPos(ImVec2(24, 50));
        ImGui::TextColored(muted, "Secure gateway for the command dashboard");
        ImGui::SetCursorPos(ImVec2(24, 78));
        if (g_LoginState.status == LoginStatus::SUCCESS) StatusPill("ACCESS GRANTED", success);
        else if (g_LoginState.status == LoginStatus::CHECKING) StatusPill("VERIFYING", warning);
        else if (g_LoginState.status == LoginStatus::FAILED) StatusPill("ACCESS DENIED", danger);
        else StatusPill("WAITING FOR KEY", accent2);
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::SetCursorPos(ImVec2(32, 170));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, panel);
        ImGui::BeginChild("##login_content", ImVec2(winWidth - 64.0f, winHeight - 202.0f), true, ImGuiWindowFlags_NoScrollbar);

        if (g_LoginState.status == LoginStatus::SUCCESS) {
            ImGui::SetCursorPos(ImVec2(28, 30));
            ImGui::TextColored(success, "Access enabled successfully.");
            ImGui::SetCursorPos(ImVec2(28, 64));
            ImGui::TextColored(muted, "Redirecting to the main command interface...");
            ImGui::SetCursorPos(ImVec2(28, 112));
            ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(ImGui::GetContentRegionAvail().x - 28.0f, 22.0f), "Loading");
            *isLoggedIn = true;
        } else {
            ImGui::SetCursorPos(ImVec2(28, 24));
            ImGui::TextColored(text, "License Key");
            ImGui::SetCursorPos(ImVec2(28, 54));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 28.0f);
            ImGui::InputText("##zeus_key", g_LoginState.userKey, IM_ARRAYSIZE(g_LoginState.userKey), ImGuiInputTextFlags_Password);
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(28, 112));
            ImGui::Checkbox("Remember this session", &g_LoginState.rememberMe);

            ImGui::SetCursorPos(ImVec2(28, 164));
            if (g_LoginState.status == LoginStatus::CHECKING) {
                ImGui::TextColored(warning, "Checking key...");
                ImGui::SetCursorPos(ImVec2(28, 204));
                ImGui::ProgressBar(-1.0f * (float)ImGui::GetTime(), ImVec2(ImGui::GetContentRegionAvail().x - 28.0f, 22.0f), "Please wait");
            } else {
                float full = ImGui::GetContentRegionAvail().x - 28.0f;
                if (PremiumButton("SIGN IN", ImVec2(full * 0.62f, 56.0f), accent, accent2, accent)) {
                    g_LoginState.status = LoginStatus::CHECKING;
                    std::string key = g_LoginState.userKey;
                    std::thread([=]() {
                        std::string msg;
                        if (PerformNetworkLogin(key, msg)) {
                            g_LoginState.status = LoginStatus::SUCCESS;
                            g_LoginState.message = "OK";
                        } else {
                            g_LoginState.status = LoginStatus::FAILED;
                            g_LoginState.message = msg;
                        }
                    }).detach();
                }
                ImGui::SameLine(0.0f, 12.0f);
                if (PremiumButton("CLEAR", ImVec2(full * 0.38f - 12.0f, 56.0f), card, MixColor(card, accent, 0.30f), accent)) {
                    g_LoginState.userKey[0] = '\0';
                    g_LoginState.message.clear();
                    g_LoginState.status = LoginStatus::IDLE;
                }
            }

            if (g_LoginState.status == LoginStatus::FAILED) {
                ImGui::SetCursorPos(ImVec2(28, ImGui::GetCursorPosY() + 16.0f));
                ImGui::TextColored(danger, "[X] %s", g_LoginState.message.c_str());
            }
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::End();
        ImGui::PopStyleVar(5);
        ImGui::PopStyleColor(7);
    }
}

#endif // LOGIN_UI_H
