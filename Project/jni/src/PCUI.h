//====PC    UI
void RectUI(ImVec2 center, float width, float height, ImTextureID icon)
{
    ImGui::SetCursorPos({0, 180});
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 image_min{center.x - width, center.y - height};
    ImVec2 image_max{center.x + width, center.y + height};

    draw_list->AddImage(icon, image_min, image_max, ImVec2(0, 0), ImVec2(1, 1));
}



inline ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
return ImVec2(a.x + b.x, a.y + b.y);
}

inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
return ImVec2(a.x - b.x, a.y - b.y);
}

inline ImVec2 operator*(const ImVec2& a, float s) {
return ImVec2(a.x * s, a.y * s);
}

inline ImVec2 operator/(const ImVec2& a, float s) {
return ImVec2(a.x / s, a.y / s);
}

struct Snowflake {
    ImVec2 pos;     
    float speed;    
    float size;      
    float x_vel;    
};
namespace ImGui {


    // 水平组合框扩展版
    void HoriComboEx(
        const char* label,
        const std::vector<const char*>& items,
        const std::vector<bool>& is_button,
        int& selected_index,
        int spacing = 5
    );

    IMGUI_API void DrawTitleBar(
        const char* main_label,
        const ImVec4& bar_color = ImVec4(1.0f, 1.0f, 1.0f, 0.3f),
        float bar_width = 2.0f,
        float text_padding = 20.0f
    );



}
namespace ImGui { // 必须包裹在 ImGui 命名空间内




void DrawTitleBar(
    const char* main_label,
    const ImVec4& bar_color,
    float bar_width,
    float text_padding
) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 start_pos = ImGui::GetCursorScreenPos();
    const float line_height = ImGui::GetTextLineHeight();

    // 绘制主标题文字
    ImGui::SetCursorScreenPos(start_pos);
    ImGui::TextUnformatted(main_label);

    // 获取文本右边界和窗口右边界
    const ImVec2 text_end_pos = ImGui::GetItemRectMax();
    const float bar_start_x = text_end_pos.x + text_padding;
    const float window_right = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

    // 计算蓝条垂直中线位置
    const float bar_y = start_pos.y + line_height * 0.5f;

    // 绘制水平蓝条（从文字右侧+20px到窗口最右）
    draw_list->AddLine(
        ImVec2(bar_start_x, bar_y),
        ImVec2(window_right, bar_y),
        ImColor(bar_color),
        bar_width
    );

    // 更新光标位置
    ImGui::SetCursorScreenPos(ImVec2(
        start_pos.x,
        start_pos.y + line_height + ImGui::GetStyle().ItemSpacing.y
    ));
}
}


void ImGui::HoriComboEx(const char* label,const std::vector<const char*>& items,const std::vector<bool>& is_button,int& selected_index,int spacing) {
    struct ComboState {
        float anim_pos=0.0f;
        float bar_height=0.0f;
        float target_pos=0.0f;
        float target_height=0.0f;
        std::vector<ImVec4> text_colors;
    };
    static std::unordered_map<ImGuiID,ComboState> states;
    ImGuiID id=GetID(label);
    ComboState& state=states[id];
    if(state.text_colors.size()!=items.size()) {
        state.text_colors.resize(items.size(),ImVec4(0.65f,0.65f,0.65f,1.0f));
    }
    
    // 保存原始间距并设置为0
    ImVec2 original_spacing = GetStyle().ItemSpacing;
    GetStyle().ItemSpacing = ImVec2(0, 0);
    
    BeginGroup();
    PushID(label);
    const float line_height=GetTextLineHeight()*1.3f; // 使用基础行高，不要额外间距
    const float bar_width=4.0f;
    const ImVec4 active_color_target(1.0f,1.0f,1.0f,1.0f);
    const ImVec4 normal_color_target(0.65f,0.65f,0.65f,1.0f);
    const float color_lerp_speed=0.15f;
    const float pos_lerp_speed=0.03f;
    const float height_lerp_speed=0.03f;

    // 计算总高度 - 去掉spacing
    const float total_height = items.size() * line_height;

    BeginChild("##container",ImVec2(GetContentRegionAvail().x,total_height),false);

    float current_y=0.0f;
    int button_count=0;
    for(size_t i=0;i<items.size();++i) {
        SetCursorPosY(current_y);

        if(is_button[i]) {
            // 按钮交互区域 - 高度与文字一致
            const ImVec2 button_size(GetContentRegionAvail().x-bar_width-8,line_height);
            InvisibleButton(items[i],button_size);

            if(IsItemClicked()) {
                selected_index=button_count;
            }

            // 更新目标位置 - 蓝条高度与文字高度一致
            if(button_count==selected_index) {
                state.target_pos=current_y;
                state.target_height=line_height; // 与文字同高
            }

            // 文字颜色过渡
            const bool is_selected=(button_count==selected_index);
            const ImVec4 target_color=is_selected?active_color_target:normal_color_target;
            state.text_colors[i]=ImLerp(state.text_colors[i],target_color,color_lerp_speed);

            // 绘制按钮文字
            const ImVec2 text_size=CalcTextSize(items[i]);
            GetWindowDrawList()->AddText(
                ImVec2(
                    GetItemRectMin().x+(button_size.x-text_size.x)*0.42f,
                    GetItemRectMin().y+(line_height-text_size.y)*0.5f
                ),
                ColorConvertFloat4ToU32(state.text_colors[i]),
                items[i]
            );
            button_count++;
        } else {
            // 普通文字绘制
            const ImVec2 text_size=CalcTextSize(items[i]);
            SetCursorPosY(current_y);
            char title[256];
            snprintf(title,sizeof(title),"%s",items[i]);
            ImGui::DrawTitleBar(title);
        }

        current_y+=line_height; // 去掉spacing
    }

    // 更新蓝条动画
    state.anim_pos+=(state.target_pos-state.anim_pos)*pos_lerp_speed;
    state.bar_height+=(state.target_height-state.bar_height)*height_lerp_speed;

    // 绘制左侧蓝条 - 高度与文字一致
    const ImVec2 bar_min(
        GetWindowPos().x+2,
        GetWindowPos().y+state.anim_pos+9
    );
    const ImVec2 bar_max(
        bar_min.x+bar_width,
        bar_min.y+state.bar_height-15
    );
    GetWindowDrawList()->AddRectFilled(
        bar_min, 
        bar_max, 
        ImGui::GetColorU32(ImVec4(101.0f/255.0f,102.0f/255.0f,151.0f/255.0f,0.8f)),
        2.0f
    );

    EndChild();
    PopID();
    EndGroup();
    
    // 恢复原始间距
    GetStyle().ItemSpacing = original_spacing;
}
std::vector<Snowflake> snowflakes;
bool is_snow_initialized = false;

void InitSnow(ImGuiWindow* window) {
    srand(static_cast<unsigned>(time(nullptr)));
    ImVec2 window_size = window->Size;
    snowflakes.clear();

    for (int i = 0; i < 20; ++i) {
        Snowflake flake;
        flake.pos = ImVec2(
            static_cast<float>(rand() % static_cast<int>(window_size.x)), 
            static_cast<float>(rand() % static_cast<int>(window_size.y))
        );

        flake.speed = 280.0f + static_cast<float>(rand() % 380);
        flake.size = 2.0f + static_cast<float>(rand() % 6);
        flake.x_vel = (rand() % 300 - 24) * 0.01f;

        snowflakes.push_back(flake);
    }
}

void UpdateAndRenderSnow(ImGuiWindow* window) {
    if (!is_snow_initialized) {
        InitSnow(window);
        is_snow_initialized = true;
    }

    ImVec2 window_pos = window->Pos;
    ImVec2 window_size = window->Size;
    float delta_time = ImGui::GetIO().DeltaTime;

    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    for (auto& flake : snowflakes) {
        // 更新位置
        flake.pos.x += flake.x_vel * 35.0f * delta_time;
        flake.pos.y += flake.speed * delta_time * (0.5f + flake.size * 0.06f);

        // 垂直边界检查（去掉20像素缓冲）
        if (flake.pos.y > window_size.y) {
            flake.pos.y = 0.0f;
            flake.pos.x = static_cast<float>(rand() % static_cast<int>(window_size.x));
        }

        // 水平边界检查（去掉20像素缓冲）
        if (flake.pos.x < 0) flake.pos.x = window_size.x;
        else if (flake.pos.x > window_size.x) flake.pos.x = 0;

        // 透明度计算
        float alpha = 80.0f + (flake.size / 14.0f) * 175.0f;
        alpha = ImClamp(alpha, 80.0f, 255.0f);

        // 屏幕坐标
        ImVec2 screen_pos = ImVec2(
            window_pos.x + flake.pos.x,
            window_pos.y + flake.pos.y
        );

        // 渲染雪花
        draw_list->AddCircleFilled(
            screen_pos,
            flake.size,
            IM_COL32(255, 255, 255, static_cast<int>(alpha)),
            16 
        );
    }
}




inline void LabelA(const char* fmt_or_text, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt_or_text);
    vsnprintf(buffer, sizeof(buffer), fmt_or_text, args);
    va_end(args);

    // First output "ZEUS KERNEL "
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "ZEUS KERNEL ");
    ImGui::SameLine(0, 0);

    // Then output "v1.0" with designated color
    ImGui::TextColored(ImVec4(72 / 255.0f, 69 / 255.0f, 83 / 255.0f, 1.0f), "v1.0");
    ImGui::Separator();
}
inline void LabelC(const char* fmt_or_text, ...) {
    char buffer[1024];                       // Output buffer
    va_list args;
    va_start(args, fmt_or_text);
    vsnprintf(buffer, sizeof(buffer), fmt_or_text, args);
    va_end(args);

    ImGui::TextColored(ImColor(255, 255, 255, 255), "%s", buffer);
    //ImGui::Separator();
}

inline void LabelB(const char* fmt_or_text, ...)
{
    char buffer[1024];
    va_list args;
    va_start(args, fmt_or_text);
    vsnprintf(buffer, sizeof(buffer), fmt_or_text, args);
    va_end(args);

    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", buffer);

    ImGui::SameLine();
    
    // Use Separator but adjust to same line as text
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() / 2 - 1);
    ImGui::Separator();
}

void VerticalLineUI(){
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    float border_x = window_pos.x + 7*32.4; // Left menu width
    draw_list->AddLine(ImVec2(border_x, window_pos.y), ImVec2(border_x, window_pos.y + window_size.y), ImColor(80, 80, 80, 255), 1.0f);
}

struct ButtonAnim {
float Alpha = 0.0f;
bool IsPressed = false;
};
static ImGuiStorage g_ButtonState;
bool CustomButtonEx(const char* label)
{
    bool clicked = false;
    ImGui::PushID(label);

    static ImGuiID s_SelectedButtonID = 0;
    ImGuiID currentID = ImGui::GetID(label);

    ButtonAnim* anim = (ButtonAnim*)g_ButtonState.GetVoidPtr(currentID);
    if (!anim) {
        g_ButtonState.SetVoidPtr(currentID, new ButtonAnim());
        anim = (ButtonAnim*)g_ButtonState.GetVoidPtr(currentID);
    }

    // 减小按钮高度，去掉上方空白
    ImGui::InvisibleButton("##btn", ImVec2(-FLT_MIN, ImGui::GetTextLineHeight()));  // 改为文本行高

    bool is_hovered = ImGui::IsItemHovered();
    bool is_clicked = ImGui::IsItemClicked();

    float target_alpha = (is_hovered || anim->IsPressed) ? 1.0f : 0.0f;
    float delta = ImGui::GetIO().DeltaTime * 5.0f;
    anim->Alpha = ImClamp(anim->Alpha + (target_alpha - anim->Alpha) * delta, 0.0f, 1.0f);

    if (is_clicked) {
        clicked = true;
        anim->IsPressed = true;
        s_SelectedButtonID = currentID; // 设置当前选中的按钮ID
    }
    if (ImGui::IsMouseReleased(0)) {
        anim->IsPressed = false;
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();

    // 如果这是选中的按钮，使用白色；否则使用原来的颜色逻辑
    ImU32 text_color = (currentID == s_SelectedButtonID) 
        ? IM_COL32(255, 255, 255, 255) 
        : IM_COL32(
            128 + (int)(127 * anim->Alpha),
            128 + (int)(127 * anim->Alpha),
            128 + (int)(127 * anim->Alpha),
            anim->IsPressed ? 255 : 200  
        );

    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos(
        min.x + (max.x - min.x - text_size.x) * 0.5f,
        min.y + (max.y - min.y - text_size.y) * 0.5f
    );
    draw_list->AddText(text_pos, text_color, label);

    ImGui::PopID();
    return clicked;
}
bool CustomButton(const char* label)
{
    bool clicked = CustomButtonEx(label);
    if (clicked) {
        // 点击处理
    }
    return clicked;
}



ImVec2 GetPhysicalDisplaySize();


ImVec2 GetPhysicalDisplaySize() {
const ImGuiIO& io = ImGui::GetIO();
#if defined(_WIN32)

return ImVec2(io.DisplaySize.x * io.DisplayFramebufferScale.x,
io.DisplaySize.y * io.DisplayFramebufferScale.y);
#else

return io.DisplaySize;
#endif
}

#include <deque>
#include <string>
#include "imgui.h"

#include <deque>
#include <string>

struct Toast {
std::string content;
float start_time;
float anim_progress = 0.0f; 
float vertical_offset = 0.0f;   
float target_offset = 0.0f;      
};

static std::deque<Toast> toasts;

void ShowToast(const char* label, bool state) {
Toast new_toast;
new_toast.content = std::string(label) + (state ? " Enabled" : " Disabled");
new_toast.start_time = ImGui::GetTime();


const float toast_height = 50.0f * ImGui::GetIO().DisplayFramebufferScale.y;
const float spacing = 60.0f * ImGui::GetIO().DisplayFramebufferScale.y;
for (auto& t : toasts) {
t.target_offset += toast_height + spacing;
}

toasts.push_back(new_toast);


if (toasts.size() > 4) {
toasts.front().anim_progress = 2.0f;
}
}

void UpdateAndRenderToasts() {
    const float current_time = ImGui::GetTime();
    const float delta_time = ImGui::GetIO().DeltaTime;

    // 使用你提供的屏幕尺寸
    const ImVec2 screen_size = ImVec2(displayInfo.width, displayInfo.height);

    const float right_margin = 20.0f * ImGui::GetIO().DisplayFramebufferScale.x;
    const float bottom_margin = 20.0f * ImGui::GetIO().DisplayFramebufferScale.y;
    const float toast_width = 220.0f * ImGui::GetIO().DisplayFramebufferScale.x;
    const float toast_height = 50.0f * ImGui::GetIO().DisplayFramebufferScale.y;
    const float spacing = 60.0f * ImGui::GetIO().DisplayFramebufferScale.y;

    // 更新垂直偏移动画
    for (auto& toast : toasts) {
        const float animation_speed = 10.0f;
        toast.vertical_offset = ImLerp(
            toast.vertical_offset,
            toast.target_offset,
            ImClamp(delta_time * animation_speed, 0.0f, 0.7f)
        );
    }

    // 计算基础位置（屏幕右下角）
    const float base_x = screen_size.x - right_margin;
    const float base_y = screen_size.y - bottom_margin - 40;

    for (int i = 0; i < toasts.size();) {
        Toast& toast = toasts[i];
        const float elapsed = current_time - toast.start_time;
        toast.anim_progress = elapsed / 1.0f;

        // 计算垂直位置
        float current_y = base_y - toast.vertical_offset - toast_height;
        float current_x = base_x - toast_width;
        
        // 进入动画
        if (toast.anim_progress < 1.0f) {
            float slide_t = ImClamp(toast.anim_progress * 5.0f, 0.0f, 1.0f);
            current_x = ImLerp(base_x, base_x - toast_width, slide_t);
        }
        // 退出动画
        else if (toast.anim_progress >= 2.0f) {
            float exit_t = ImClamp((toast.anim_progress - 2.0f) * 2.0f, 0.0f, 1.0f);
            current_x = ImLerp(base_x - toast_width, base_x + toast_width, exit_t);
        }

        // 渲染提示窗口
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 10));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.15f, 0.95f));

        ImGui::SetNextWindowPos(ImVec2(current_x, current_y), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(toast_width, toast_height + 40));

        if (ImGui::Begin(
            ("##Toast_" + std::to_string(i)).c_str(),
            nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoFocusOnAppearing
        )) {
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 text_size = ImGui::CalcTextSize(toast.content.c_str());

            ImGui::SetCursorPos(
                ImVec2(
                    (window_size.x - text_size.x) * 0.5f,
                    (window_size.y - text_size.y) * 0.5f
                )
            );

            ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "%s", toast.content.c_str());
            ImGui::End();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(2);

        // 移除过期的提示
        if (toast.anim_progress >= 3.0f) {
            toasts.erase(toasts.begin() + i);

            // 重新计算剩余提示的偏移
            float accumulated_offset = 0.0f;
            for (auto it = toasts.rbegin(); it != toasts.rend(); ++it) {
                it->target_offset = accumulated_offset;
                accumulated_offset += (toast_height + spacing);
            }
        } else {
            ++i;
        }
    }
}
//按钮

bool CustomCheckbox(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float square_size = ImGui::GetFrameHeight() * 0.7f;
    const ImVec2 pos = window->DC.CursorPos;

    // 增加间隔：总高度包含内容、间距和横线
    const float vertical_padding = 8.0f; // 上下各2像素的间距
    const float total_height = square_size + vertical_padding + 3.0f; // 包含间距和1像素横线
    
    const ImRect total_bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + total_height));
    
    const ImRect text_bb(
        pos + ImVec2(0, vertical_padding * 0.5f), 
        ImVec2(pos.x + label_size.x, pos.y + vertical_padding * 0.5f + square_size)
    );
    
    // 复选框位置也考虑垂直间距
    const ImRect frame_bb(
        ImVec2(total_bb.Max.x - square_size - 14.6f, pos.y + vertical_padding * 0.5f),
        ImVec2(total_bb.Max.x - 14.6f, pos.y + vertical_padding * 0.5f + square_size)
    );

    // 横线的位置 - 在总边界框的底部
    const ImVec2 line_start(total_bb.Min.x, total_bb.Max.y - 1.0f);
    const ImVec2 line_end(total_bb.Max.x, total_bb.Max.y - 1.0f);

    ImGui::ItemSize(total_bb, 0.0f); // 使用0作为垂直填充，通过total_height控制间距
    if (!ImGui::ItemAdd(total_bb, id))
        return false;

    float& progress = *window->StateStorage.GetFloatRef(id, 0.0f);
    bool& prev_state = *window->StateStorage.GetBoolRef(id + 1, false);

    if (prev_state != *v) {
        prev_state = *v;
    }

    float target = *v ? 1.0f : 0.0f;
    float delta = (target - progress) * ImMin(g.IO.DeltaTime * 10.0f, 1.0f);
    progress += delta;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);   
    }

    if (pressed) {
        ShowToast(label, *v);
    }

    // 渲染文本
    const float text_offset_y = (square_size - label_size.y) * 0.5f;
    const ImVec2 text_pos = ImVec2(
        text_bb.Min.x, 
        frame_bb.Min.y + text_offset_y 
    );
    ImGui::RenderText(text_pos, label);

    // 绘制复选框背景
    const ImVec4 bg_color_off(0.6f, 0.6f, 0.6f, 0.2f);
    const ImVec4 bg_color_on(101.0f/255, 102.0f/255, 151.0f/255, 0.8f);
    ImVec4 bg_color = ImLerp(bg_color_off, bg_color_on, progress);
    const float rounding = 5.0f;
    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(bg_color), rounding);

    // 绘制对勾动画
    if (progress > 0.0f) {
        const float pad = ImMax(2.0f, square_size * 0.15f);
        const float size = square_size - pad * 2.0f;
        const float t = ImClamp(progress, 0.0f, 1.0f);

        ImVec2 check_pos = frame_bb.Min + ImVec2(pad, pad);
        const ImVec2 center = frame_bb.GetCenter();

        const ImVec2 p0(check_pos.x + size * 0.1f, check_pos.y + size * 0.5f);
        const ImVec2 p1(check_pos.x + size * 0.4f, check_pos.y + size * 0.8f);
        const ImVec2 p2(check_pos.x + size * 0.9f, check_pos.y + size * 0.2f);

        const ImVec2 anim_p0 = ImLerp(center, p0, t);
        const ImVec2 anim_p1 = ImLerp(center, p1, t);
        const ImVec2 anim_p2 = ImLerp(center, p2, t);

        const ImU32 check_color = IM_COL32(255, 255, 255, (int)(255 * t));
        const float line_thickness = 3.0f;

        ImVec2 points[] = { anim_p0, anim_p1, anim_p2 };
        window->DrawList->AddPolyline(
            points, 
            3, 
            check_color,
            ImDrawFlags_RoundCornersAll,  
            line_thickness              
        );

        const float dot_radius = line_thickness * 0.5f * t;
        window->DrawList->AddCircleFilled(anim_p1, dot_radius, check_color);
    }

    // 绘制横线 - 改为纯白色
    const ImU32 line_color = IM_COL32(100, 100, 100, 255); // 纯白色
    window->DrawList->AddLine(line_start, line_end, line_color, 1.0f);

    return pressed;
}


static float CubicEase(float t) {
if (t < 0.5f) {
return 4 * t * t * t;
} else {
float f = (-2 * t + 2);
return 1 - (f * f * f) / 2;
}
}
struct ComboAnimState {
float Progress = 0.0f;
bool IsOpen = false;
bool IsAnimating = false;
float AnimStartTime = 0.0f;
};

bool CustomCombo(const char* label, float* current_value, const char* items[], int items_count) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiStyle& style = ImGui::GetStyle();

    ImGuiStorage* storage = window->DC.StateStorage;
    ImGuiID id = window->GetID(label);
    ComboAnimState* state = (ComboAnimState*)storage->GetVoidPtr(id);
    if (!state) {
        state = IM_NEW(ComboAnimState)();
        storage->SetVoidPtr(id, state);
    }

    // 将float值转换为索引
    int current_index = static_cast<int>(*current_value);
    if (current_index < 0) current_index = 0;
    if (current_index >= items_count) current_index = items_count - 1;
    
    const char* preview = items[current_index];
    const float frame_height = ImGui::GetFrameHeight();
    const float arrow_size = frame_height * 0.4f;
    const float spacing = 8.0f;
    const float rounding = 6.0f;

    const ImVec2 label_size = ImGui::CalcTextSize(label);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (frame_height - label_size.y) * 0.5f);
    ImGui::Text("%s", label);
    ImGui::SameLine();

    const ImVec2 preview_size = ImGui::CalcTextSize(preview);
    const float content_width = preview_size.x + arrow_size + spacing * 3;
    const float expand_width = content_width * 1.8f; 

    const ImVec2 content_max = ImGui::GetWindowContentRegionMax();
    const float right_edge = content_max.x;
    const float pos_x = right_edge - content_width;
    const float clamped_pos_x = ImMax(ImGui::GetWindowContentRegionMin().x, pos_x);

    ImGui::SetCursorPosX(clamped_pos_x);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - (frame_height - label_size.y) * 0.5f);
    const ImVec2 pos = window->DC.CursorPos;

    ImRect bb(pos, pos + ImVec2(content_width, frame_height));

    bool pressed = ImGui::ButtonBehavior(bb, id, nullptr, nullptr, ImGuiButtonFlags_PressedOnClick);
    if (pressed) {
        state->IsOpen = !state->IsOpen;
        state->IsAnimating = true;
        state->AnimStartTime = (float)ImGui::GetTime();
    }

    if (state->IsAnimating) {
        const float anim_duration = 0.3f;
        const float elapsed = (float)ImGui::GetTime() - state->AnimStartTime;
        const float raw_progress = ImClamp(elapsed / anim_duration, 0.0f, 1.0f);

        state->Progress = CubicEase(raw_progress);

        if (!state->IsOpen) {
            state->Progress = 1.0f - state->Progress;
        }

        if (elapsed >= anim_duration) {
            state->IsAnimating = false;
            state->Progress = state->IsOpen ? 1.0f : 0.0f;
        }
    }

    const float bg_expand = expand_width * state->Progress;
    const ImRect bg_rect(
        pos.x + content_width - bg_expand, 
        pos.y,
        pos.x + content_width,
        pos.y + frame_height
    );

    if (bg_expand > 0) {
        const ImU32 bg_color = IM_COL32(101, 102, 151, (int)(state->Progress * 255));
        window->DrawList->AddRectFilled(bg_rect.Min, bg_rect.Max, bg_color, rounding);
    }

    const float text_start = pos.x + content_width - arrow_size - spacing - preview_size.x;
    const float text_offset = bg_expand - (content_width - preview_size.x - arrow_size - spacing);
    const ImVec2 text_pos(
        text_start - text_offset * 0.5f, 
        pos.y + (frame_height - preview_size.y) * 0.5f
    );

    const ImU32 text_color = ImGui::GetColorU32(ImLerp(
        ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled),
        ImVec4(1, 1, 1, 1),
        state->Progress
    ));
    window->DrawList->AddText(text_pos, text_color, preview);

    const ImVec2 arrow_center(
        pos.x + content_width - arrow_size * 0.3f - spacing,
        pos.y + frame_height * 0.5f
    );

    const float anim_angle = IM_PI * 0.5f * state->Progress;
    const float radius = arrow_size * 0.4f;

    const ImVec2 points[3] = {
        ImVec2(cosf(anim_angle), sinf(anim_angle)),
        ImVec2(cosf(anim_angle + 2.0f * IM_PI / 3.0f), sinf(anim_angle + 2.0f * IM_PI / 3.0f)),
        ImVec2(cosf(anim_angle - 2.0f * IM_PI / 3.0f), sinf(anim_angle - 2.0f * IM_PI / 3.0f))
    };

    const ImU32 arrow_color = ImGui::GetColorU32(ImLerp(
        ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled),
        ImVec4(1, 1, 1, 1),
        state->Progress
    ));
    window->DrawList->AddTriangleFilled(
        arrow_center + points[0] * radius,
        arrow_center + points[1] * radius,
        arrow_center + points[2] * radius,
        arrow_color
    );

    // 记录是否选择了项
    bool item_selected = false;

    if (state->IsOpen) {
        ImGui::SetNextWindowPos(bg_rect.GetBL() + ImVec2(0, style.ItemSpacing.y));
        ImGui::SetNextWindowSize(ImVec2(bg_rect.GetWidth() * 0.9f, 0)); 

        ImGui::Begin("##ComboPopup", nullptr, 
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings
        );

        for (int i = 0; i < items_count; ++i) {
            if (ImGui::Selectable(items[i], current_index == i)) {
                *current_value = static_cast<float>(i);  // 将索引转换为float值
                state->IsOpen = false;
                state->IsAnimating = true;
                state->AnimStartTime = (float)ImGui::GetTime();
                item_selected = true;  // 标记已选择
            }
        }
        ImGui::End();
    }

    // 如果选择了项，立即关闭弹出窗口
    if (item_selected) {
        // 这里可以添加额外的关闭逻辑，如果需要的话
    }

    return pressed || item_selected;
}

struct FixedLayout {
ImVec2 start_pos;     
float current_y;       
float item_spacing;   

FixedLayout(ImVec2 base_pos, float spacing = 5.0f) 
: start_pos(base_pos), current_y(0), item_spacing(spacing) {}

ImVec2 NextPos(float height) {
ImVec2 pos = start_pos + ImVec2(0, current_y);
current_y += height + item_spacing;
return pos;
}
};





#include <map>

struct SliderState {
float target_value;
bool needs_init = true;
};
static std::map<ImGuiID, SliderState> slider_states;
bool CustomSliderFloat(const char* label, float* value,
float min_value, float max_value,
bool show_decimal = false, bool first_item = false)
{
    (void)first_item;
    ImGui::PushID(label);
    ImGuiID id = ImGui::GetID(label);
    bool value_changed = false;

    if (slider_states.find(id) == slider_states.end()) {
        slider_states[id] = SliderState();
        slider_states[id].target_value = *value;
    }

    ImVec2 start = ImGui::GetCursorScreenPos();
    ImVec2 content_size = ImGui::GetContentRegionAvail();
    if (content_size.x < 180.0f) content_size.x = 180.0f;

    const float label_h = 26.0f;
    const float bar_h = 32.0f;
    const float bar_y = label_h + 14.0f;
    const float total_h = bar_y + bar_h + 12.0f;
    const float pad_x = 8.0f;
    const float bar_w = content_size.x - pad_x * 2.0f;

    ImGui::InvisibleButton("##slider_area", ImVec2(content_size.x, total_h));
    const bool is_active = ImGui::IsItemActive();

    if (is_active) {
        const float mouse_x_rel = ImGui::GetMousePos().x - (start.x + pad_x);
        slider_states[id].target_value = (mouse_x_rel / bar_w) * (max_value - min_value) + min_value;
        slider_states[id].target_value = ImClamp(slider_states[id].target_value, min_value, max_value);
        *value = slider_states[id].target_value;
        value_changed = true;
    } else {
        slider_states[id].target_value = *value;
    }

    *value = ImClamp(*value, min_value, max_value);
    const float denom = (max_value - min_value) == 0.0f ? 1.0f : (max_value - min_value);
    const float pct = ImClamp((*value - min_value) / denom, 0.0f, 1.0f);

    char value_text[32];
    if (show_decimal)
        snprintf(value_text, sizeof(value_text), "%.1f", *value);
    else
        snprintf(value_text, sizeof(value_text), "%.0f", *value);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImU32 title_col = IM_COL32(232, 238, 248, 255);
    const ImU32 value_col = IM_COL32(178, 190, 210, 255);
    const ImU32 bg_col = IM_COL32(42, 48, 64, 210);
    const ImU32 fill_col = IM_COL32(92, 122, 178, 255);
    const ImU32 knob_col = IM_COL32(236, 241, 250, 255);

    draw_list->AddText(ImVec2(start.x + pad_x, start.y + 2.0f), title_col, label);
    ImVec2 value_size = ImGui::CalcTextSize(value_text);
    draw_list->AddText(ImVec2(start.x + content_size.x - value_size.x - pad_x, start.y + 2.0f), value_col, value_text);

    ImVec2 bar_min(start.x + pad_x, start.y + bar_y);
    ImVec2 bar_max(start.x + pad_x + bar_w, start.y + bar_y + bar_h);
    draw_list->AddRectFilled(bar_min, bar_max, bg_col, bar_h * 0.5f);
    draw_list->AddRectFilled(bar_min, ImVec2(bar_min.x + bar_w * pct, bar_max.y), fill_col, bar_h * 0.5f, ImDrawFlags_RoundCornersAll);
    draw_list->AddCircleFilled(ImVec2(bar_min.x + bar_w * pct, bar_min.y + bar_h * 0.5f), 13.0f, knob_col);
    draw_list->AddCircle(ImVec2(bar_min.x + bar_w * pct, bar_min.y + bar_h * 0.5f), 13.0f, IM_COL32(80, 90, 112, 190), 32, 1.4f);

    ImGui::PopID();
    return value_changed;
}



namespace {
    bool g_IsAnyFilledSliderActive = false;
}
void TouchScroll(const bool& InertiaFlag = false)
{
    if (g_IsAnyFilledSliderActive) {
        return;
    }

    ImGuiWindow *WinPtr = ImGui::GetCurrentWindow();
    float ScrollMaxY = ImGui::GetScrollMaxY();
    if (WinPtr->SkipItems || ScrollMaxY <= 0.0f)
        return;

    static float ScrollVel = 0.0f;
    bool Hover = ImGui::IsWindowHovered();
    ImGuiIO &GuiIO = ImGui::GetIO();

    static ImVec2 StartTouchPos = ImVec2(0, 0);
    static bool Scroll = false;
    static float LastScrollY = 0.0f;
    static double LastTime = 0.0;
    static bool IsFilledSliderActive = false;

    auto UpdateScroll = [&](float deltaY, float deltaTime) {
        if (IsFilledSliderActive) return;

        float CurScrollY = ImGui::GetScrollY();
        float NewScrollY = CurScrollY + deltaY;
        NewScrollY = ImClamp(NewScrollY, 0.0f, ScrollMaxY);
        ImGui::SetScrollY(NewScrollY);

        ScrollVel = deltaY / (deltaTime + 0.0001f);
        LastScrollY = NewScrollY;
    };

    auto GetDeltaTime = [&]() {
        double CurTime = ImGui::GetTime();
        float DeltaTime = (float)(CurTime - LastTime);
        LastTime = CurTime;
        return DeltaTime;
    };

    auto ToggleParentWindowMove = [WinPtr](bool noMove) {
        if (ImGuiWindow *ParWinPtr = WinPtr->ParentWindow)
        {
            if (noMove)
            {
                ParWinPtr->Flags |= ImGuiWindowFlags_NoMove;
            }
            else
            {
                ParWinPtr->Flags &= ~ImGuiWindowFlags_NoMove;
            }
        }
    };

    if (Hover && GuiIO.MouseDown[0] && !Scroll && !IsFilledSliderActive)
    {
        StartTouchPos = GuiIO.MousePos;
        Scroll = true;
        LastScrollY = ImGui::GetScrollY();
        LastTime = ImGui::GetTime();
        ToggleParentWindowMove(true);
    }

    if (Scroll)
    {
        float DeltaTime = GetDeltaTime();

        if (GuiIO.MouseDown[0] && !IsFilledSliderActive)
        {
            float DeltaY = StartTouchPos.y - GuiIO.MousePos.y;
            UpdateScroll(DeltaY, DeltaTime);
            StartTouchPos = GuiIO.MousePos;
        }
        else
        {
            Scroll = false;
            ToggleParentWindowMove(false);
        }
    }

    if (!Scroll && !IsFilledSliderActive && fabsf(ScrollVel) > 0.1f && InertiaFlag)
    {
        float DeltaTime = GetDeltaTime();
        float CurScrollY = ImGui::GetScrollY();
        float NewScrollY = CurScrollY + ScrollVel * DeltaTime;
        NewScrollY = ImClamp(NewScrollY, 0.0f, ScrollMaxY);
        ImGui::SetScrollY(NewScrollY);

        ScrollVel *= powf(0.95f, DeltaTime * 60.0f);

        if (NewScrollY <= 0.0f || NewScrollY >= ScrollMaxY)
        {
            ScrollVel = 0.0f;
        }
    }
    else if (!Scroll && !IsFilledSliderActive)
    {
        ScrollVel = 0.0f;
    }
}

bool 滚动惯性;
struct SCROLL_MSG
{
    float max, now;
} ScrollMsg;

#include <unordered_map>
#include <type_traits>

template<typename T>
bool FilledSliderT(const char* label, T* value, T min, T max, T defaultValue, const char* format, bool showDefault = false)
{
    static_assert(std::is_arithmetic<T>::value, "Only numeric types are supported");

    if (*value == 0) *value = defaultValue;
    ImGui::PushID(label);

    ImGui::TextUnformatted(label);
    if (showDefault) {
        char buf[64];
        if constexpr (std::is_integral<T>::value)
            snprintf(buf, sizeof(buf), "Default: %d", defaultValue);
        else
            snprintf(buf, sizeof(buf), "Default: %.1f", defaultValue);
        ImGui::SameLine();
        ImGui::TextDisabled("(%s)", buf);
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(-1);
    const char* fmt = format ? format : (std::is_integral<T>::value ? "%d" : "%.1f");
    char val[64];
    snprintf(val, sizeof(val), fmt, *value);
    ImGui::Text("%s", val);

    float bar_h  = 15.f;
    float radius = 20.f;

    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImVec2 region(660, 20); // 固定宽度200像素
    ImVec2 bar_min(cursor.x + 20.0f, cursor.y);
        ImVec2 bar_max(cursor.x + region.x, cursor.y + bar_h);
    float progress = (float)(*value - min) / (float)(max - min);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(bar_min, bar_max, ImGui::GetColorU32(ImVec4(0.12f, 0.12f, 0.12f, 0.5f)), bar_h * 50.0f);
    dl->AddRectFilled(bar_min, ImVec2(bar_min.x + progress * region.x, bar_max.y), ImGui::GetColorU32(ImVec4(0.0f, 0.5f, 1.0f, 1.0f)), bar_h * 50.0f);

    ImVec2 center(bar_min.x + progress * region.x, bar_min.y + bar_h * 0.5f);
    dl->AddCircleFilled(center, radius, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

    ImVec2 tl(center.x - radius, center.y - radius);
ImGui::SetCursorScreenPos(tl);

    ImGui::PushID("__handle");
    bool active = ImGui::InvisibleButton("##h", ImVec2(radius * 2, radius * 2));
    ImGui::PopID();
bool changed = false;

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 mp = ImGui::GetMousePos();
        float x = ImClamp(mp.x, bar_min.x, bar_max.x);
        float norm = (x - bar_min.x) / (bar_max.x - bar_min.x);
        T new_value;
        if constexpr (std::is_integral<T>::value)
            new_value = min + (T)(norm * (max - min) + 0.5f);
        else
            new_value = min + norm * (max - min);
                if (new_value == 0) new_value = defaultValue;
        if (*value != new_value) { *value = new_value; changed = true; }

    }

    
    ImGui::PopID();
    return changed;
}

inline bool FilledSliderFloat(const char* label, float* value, float min, float max) { return FilledSliderT(label, value, min, max, (min + max) * 0.5f, "%.1f", false); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, const char* format) { return FilledSliderT(label, value, min, max, (min + max) * 0.5f, format, false); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, bool showDefault) { return FilledSliderT(label, value, min, max, (min + max) * 0.5f, "%.1f", showDefault); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, const char* format, bool showDefault) { return FilledSliderT(label, value, min, max, (min + max) * 0.5f, format, showDefault); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, float defaultValue) { return FilledSliderT(label, value, min, max, defaultValue, "%.1f", false); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, float defaultValue, const char* format) { return FilledSliderT(label, value, min, max, defaultValue, format, false); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, float defaultValue, bool showDefault) { return FilledSliderT(label, value, min, max, defaultValue, "%.1f", showDefault); }
inline bool FilledSliderFloat(const char* label, float* value, float min, float max, float defaultValue, const char* format, bool showDefault) { return FilledSliderT(label, value, min, max, defaultValue, format, showDefault); }

inline bool FilledSliderInt(const char* label, int* value, int min, int max) { return FilledSliderT(label, value, min, max, (min + max) / 2, "%d", false); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, const char* format) { return FilledSliderT(label, value, min, max, (min + max) / 2, format, false); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, bool showDefault) { return FilledSliderT(label, value, min, max, (min + max) / 2, "%d", showDefault); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, const char* format, bool showDefault) { return FilledSliderT(label, value, min, max, (min + max) / 2, format, showDefault); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, int defaultValue) { return FilledSliderT(label, value, min, max, defaultValue, "%d", false); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, int defaultValue, const char* format) { return FilledSliderT(label, value, min, max, defaultValue, format, false); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, int defaultValue, bool showDefault) { return FilledSliderT(label, value, min, max, defaultValue, "%d", showDefault); }
inline bool FilledSliderInt(const char* label, int* value, int min, int max, int defaultValue, const char* format, bool showDefault) { return FilledSliderT(label, value, min, max, defaultValue, format, showDefault); }




static std::unordered_map<const char*, float> 选中位置表;

void 按钮G(const char* fy_name, const std::vector<std::string>& names, float& 参数, float 居中 = 0.0f, float 按钮x = 125, float 按钮y = 70, float 矩形圆角 = 15.0f, ImVec4 color = ImVec4(1, 1, 1, 1.0f), float textsize = 80.0f)
{
    ImVec2 按钮尺寸 = ImVec2(按钮x, 按钮y);
    ImVec2 窗口尺寸 = ImVec2(250, 550);
    
    // 获取或创建选中位置
    auto& 选中位置 = 选中位置表[fy_name];
    
    // 使用 ImGui 自带的动画系统进行平滑插值
    float 目标位置 = static_cast<int>(参数) * (按钮y + 5.0f);
    选中位置 = ImLerp(选中位置, 目标位置, ImGui::GetIO().DeltaTime * 15.0f); // 15.0f 是动画速度
    
    if (居中)
    {
        //居中窗口位置
        ImVec2 窗口位置 = ImVec2( 0, 50 );
        ImGui::SetNextWindowPos(窗口位置);
    }
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0,0,0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, ImGui::GetStyle().ItemSpacing.y));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 矩形圆角);
    
    ImGui::BeginChild(fy_name, 窗口尺寸, false); // 子窗口
    
    // 使用 ImGui 自带的时间计算闪烁效果
    float time = ImGui::GetTime();
    
    // 获取当前选中的索引
    int 当前选中 = static_cast<int>(参数);
    
    // 计算跳动偏移
//    float 跳动偏移 = 0.0f;
  /*  if (当前选中 >= 0 && 当前选中 < names.size())
    {
        跳动偏移 = sin(time * 8.0f) * 3.0f; // 8.0f是跳动频率，3.0f是跳动幅度
    }*/
    
    // 绘制滑动背景条（带圆角）
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    
    // 绘制左侧竖线（带跳动动画）
    ImVec2 竖线起点 = ImVec2(window_pos.x + 12, window_pos.y-2 + 选中位置 /*+ 跳动偏移*/);
    ImVec2 竖线终点 = ImVec2(window_pos.x + 17, window_pos.y+2 + 选中位置 + 按钮y/* + 跳动偏移*/);
    draw_list->AddRectFilled(竖线起点, 竖线终点, ImColor(37, 225, 241, 255), 矩形圆角 * 0.5f);
    
    for (int i = 0; i < names.size(); i++)
    {
        bool 应该显示选中 = (当前选中 == i);
      //  float 文字跳动偏移 = 应该显示选中 ? 跳动偏移 : 0.0f;
        ImVec2 按钮位置 = ImVec2(5.0f, 按钮y * i + 5.0f * i);
        ImGui::SetCursorPos(按钮位置);
        
        std::string button_id = "##talent" + std::to_string(i);
        if (ImGui::Button(button_id.c_str(), 按钮尺寸))
        {
            参数 = static_cast<float>(i);
        }
        
        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
        
        std::string text = names[i];
        ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
        ImVec2 text_pos(
            min.x + (按钮x - text_size.x) / 2,
            min.y + (按钮y - text_size.y) / 2 /*+ 文字跳动偏移 */  // ← 只改这一行
        );
        
        // 绘制选中背景（带圆角和透明度闪烁）
        if (应该显示选中)
        {
           float alpha = (sin(time * 4.0f) + 1.0f) * 0.5f * 100 + 50; // 50-150之间的透明度变化            
           // draw_list->AddRectFilled(ImColor(37, 225, 241,50));
        }
        
        // 绘制文字
        ImColor text_color = 应该显示选中 ? ImColor(37,255,241,255) : ImColor(255,255,255,180);
        draw_list->AddText(text_pos, text_color, text.c_str());
    }
    
    ImGui::EndChild();
    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar(3);
}
void DrawTextBackground(){
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    float title_bar_height = 45.0f;
    float rounding = 28.0f;  // 统一圆角大小
    const ImDrawFlags rounding_flags = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 col_left = IM_COL32(101, 102, 151, 255);
    ImU32 col_right = IM_COL32(101, 102, 151, 0);
    
    float offset_x = 0.1f;
    float offset_y = -4.0f;
    float overall_left_offset = -5.0f;
    float shrink_right = 4.5f;
    
    ImVec2 adjusted_pos = ImVec2(window_pos.x + offset_x + overall_left_offset, window_pos.y + offset_y);
    float adjusted_width = window_size.x - shrink_right;
    
    // 使用调整后的位置和宽度
    draw_list->AddRectFilledMultiColor(
        adjusted_pos, 
        ImVec2(adjusted_pos.x + adjusted_width, adjusted_pos.y + title_bar_height + 10),
        col_left,   
        col_right,  
        col_right,  
        col_left    
    );
    
    // 使用统一的圆角大小
    draw_list->AddRect(
        adjusted_pos,
        ImVec2(adjusted_pos.x + adjusted_width, adjusted_pos.y + title_bar_height + 15),
        IM_COL32(24, 24, 25, 255),
        rounding,  // 统一使用相同的圆角
        rounding_flags,
        10.0f 
    );
    
    ImGui::SetCursorScreenPos(ImVec2(adjusted_pos.x + 10, adjusted_pos.y + (title_bar_height - ImGui::GetFontSize()) * 5.5f));
}
