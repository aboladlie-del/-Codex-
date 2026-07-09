#include "Draw.h"


EGLDisplay display = EGL_NO_DISPLAY;
EGLConfig config;
EGLSurface surface = EGL_NO_SURFACE;
EGLContext context = EGL_NO_CONTEXT;

ANativeWindow *native_window;
//
int native_window_screen_x = 0;
int native_window_screen_y = 0;
android::ANativeWindowCreator::DisplayInfo displayInfo{0};
uint32_t orientation = 0;
bool g_Initialized = false;


bool initGUI_draw(uint32_t _screen_x, uint32_t _screen_y, bool log) {
    orientation = displayInfo.orientation;
    InitVulkan();
    SetupVulkan();
    ::native_window = android::ANativeWindowCreator::Create("ZEUS KERNEL", _screen_x, _screen_y, log);
    SetupVulkanWindow(::native_window, (int)_screen_x, (int)_screen_y);
    if (!ImGui_init()) {
        return false;
    }
    UploadFonts();
    return true;
}

void screen_config() {
    displayInfo = android::ANativeWindowCreator::GetDisplayInfo();
}
void ImGuiMenustyle1()
{  
ImGuiStyle * style = &ImGui::GetStyle();
style->FrameRounding = 30;//控件圆角
style->WindowBorderSize = 1.0;//框架描边宽度
style->WindowTitleAlign = ImVec2(0.5, 0.5);//标题居中
style->WindowRounding = 15;//窗口圆角
style->FrameRounding = 0;//滚动条圆角
style->GrabRounding = 0;//滑动条圆角
style->FramePadding = ImVec2(4, 4);//方块按钮
style->ScrollbarSize = 25; // 设置滚动条宽度
style->GrabMinSize = 5; // 设置滑块宽度
style->ChildRounding  = 20;
style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);
style->Colors[ImGuiCol_ChildBg] = ImColor(255, 255, 255, 0); //子窗口颜色
// 窗口背景颜色为白色，营造简洁明亮的氛围
style->Colors[ImGuiCol_WindowBg] = ImColor(21, 21, 21, 255); // 主窗口

// 弹出窗口背景颜色为白色，与窗口背景保持一致
style->Colors[ImGuiCol_PopupBg] = ImColor(219.3, 219.3, 219.3, 0); // 白色弹出背景

// 边框颜色为稍深的红色，增加节日气氛
style->Colors[ImGuiCol_Border] = ImColor(80, 80, 80, 255); // 深红色边框

// 框架背景颜色为白色，与窗口背景保持一致
style->Colors[ImGuiCol_FrameBg] = ImColor(230, 230, 230, 120); // 白色框架背景

// 激活、悬停时的框架背景颜色为稍深的红色，增加交互感
style->Colors[ImGuiCol_FrameBgActive] = ImColor(140, 140, 140, 255); // 深红色激活背景
style->Colors[ImGuiCol_FrameBgHovered] = ImColor(140, 140, 140, 255); // 深红色悬停背景

// 滚动条背景颜色为白色
style->Colors[ImGuiCol_ScrollbarBg] = ImColor(255, 255, 255, 0); // 白色滚动条背景

// 滚动条抓取颜色为稍深的红色
style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(94, 94, 97, 255); // 深红色滚动条抓取

// 鼠标悬停时的滚动条抓取颜色为稍深的红色
style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(94, 94, 97, 255); // 深红色悬停滚动条抓取

// 激活时的滚动条抓取颜色为稍深的红色
style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(94, 94, 97, 255); // 深红色激活滚动条抓取----------

// 选中标记颜色为稍深的红色
style->Colors[ImGuiCol_CheckMark] = ImColor(74, 146, 255, 255); // 深红色选中标记

// 滑块抓取颜色为稍深的红色
style->Colors[ImGuiCol_SliderGrab] = ImColor(74, 146, 255, 220); // 深红色滑块抓取

// 激活时的滑块抓取颜色为稍深的红色
style->Colors[ImGuiCol_SliderGrabActive] = ImColor(74, 146, 255, 200); // 深红色激活滑块抓取

// 分隔线颜色为稍深的红色
style->Colors[ImGuiCol_Separator] = ImColor(100, 100, 100, 235); // 深红色分隔线

// 激活时的分隔线颜色为稍深的红色
style->Colors[ImGuiCol_SeparatorActive] = ImColor(140, 140, 140, 255); // 深红色激活分隔线

// 鼠标悬停时的分隔线颜色为稍深的红色
style->Colors[ImGuiCol_SeparatorHovered] = ImColor(140, 140, 0, 255); // 深红色悬停分隔线

// 按钮颜色
style->Colors[ImGuiCol_Button] = ImColor(140, 140, 140, 255); // 深红色按钮

// 激活的按钮颜色为稍深的红色
style->Colors[ImGuiCol_ButtonActive] = ImColor(74, 146, 255, 255); // 深红色激活按钮---------------

// 鼠标悬停时的按钮颜色为稍深的红色
style->Colors[ImGuiCol_ButtonHovered] = ImColor(74, 146, 255, 255); // 深红色悬停按钮----------

// 激活的标题颜色为稍深的红色
style->Colors[ImGuiCol_HeaderActive] = ImColor(245, 245, 245, 100); // 深红色激活标题

// 鼠标悬停时的标题颜色为稍深的红色
style->Colors[ImGuiCol_HeaderHovered] = ImColor(245, 245, 245, 100); // 深红色悬停标题------------

// 设置下展标题颜色为稍深的红色
style->Colors[ImGuiCol_Header] = ImColor(200, 0, 0, 0); // 深红色标题

// 选中文本背景颜色为稍深的红色
style->Colors[ImGuiCol_TextSelectedBg] = ImColor(200, 0, 0, 255); // 深红色选中文本背景
}
ImFont* Font = nullptr;
ImFont* Font2 = nullptr;

#include <unistd.h>

bool loadSystemFonts() {
    ImGuiIO& io = ImGui::GetIO();
    
    ImFontConfig config;
    config.SizePixels = 32.0f; 
    config.OversampleH = 2;
    config.OversampleV = 2;
    
    const char* fontPath = "/system/fonts/DroidSansFallback.ttf";
    if (access(fontPath, F_OK) != 0) {
        fontPath = "/system/fonts/Roboto-Regular.ttf";
    }
    
    // Use ChineseSimplifiedCommon to avoid huge memory allocations that crash the app
    const ImWchar* glyphRanges = io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
    
    Font = io.Fonts->AddFontFromFileTTF(fontPath, 32.0f, &config, glyphRanges);
    if (!Font) Font = io.Fonts->AddFontDefault(&config);
    
    ImFontConfig config2;
    config2.SizePixels = 28.0f;
    config2.OversampleH = 2;
    config2.OversampleV = 2;
    
    Font2 = io.Fonts->AddFontFromFileTTF(fontPath, 28.0f, &config2, glyphRanges);
    if (!Font2) Font2 = io.Fonts->AddFontDefault(&config2);
    
    return Font != nullptr && Font2 != nullptr;
}

bool ImGui_init() {
    if (g_Initialized) {
        return true;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplAndroid_Init(native_window);
    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = NULL;
    loadSystemFonts();
    


    // 构建字体纹理
    io.Fonts->Build();
    
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    ImGuiMenustyle1();
    ::g_Initialized = true;
    return true;
}

void drawBegin() {
    screen_config();
    if (::orientation != displayInfo.orientation) {
        ::orientation = displayInfo.orientation;
        UpdateScreenData(displayInfo.width, displayInfo.height, displayInfo.orientation);

        }
      ImGui_ImplVulkan_NewFrame();
     ImGui_ImplAndroid_NewFrame(native_window_screen_x, native_window_screen_y);
    ImGui::NewFrame();
}

void drawEnd() {
    ImGui::Render();
    FrameRender(ImGui::GetDrawData());
    FramePresent();
}

void shutdown() {
    if (!g_Initialized) {
        return;
    }
    DeviceWait();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    CleanupVulkanWindow();
    CleanupVulkan();
    ANativeWindow_release(native_window);
    android::ANativeWindowCreator::Destroy(native_window);
    ::g_Initialized = false;
}
