// ZEUS KERNEL - COMMAND UI
// Full visual rebuild: dashboard layout, rail navigation, hero panels, cards,
// mobile-friendly spacing, and stable ImGui-only drawing. Feature bindings are
// intentionally kept identical to the original UI file.

int Tab = 0;
int guodu = 100;
bool isSetSize = false;

struct ZeusTheme {
    const char* name;
    ImVec4 window;
    ImVec4 rail;
    ImVec4 surface;
    ImVec4 surface2;
    ImVec4 card;
    ImVec4 cardSoft;
    ImVec4 accent;
    ImVec4 accent2;
    ImVec4 text;
    ImVec4 muted;
    ImVec4 success;
    ImVec4 warning;
    ImVec4 danger;
};

static ZeusTheme ZeusThemes[] = {
    {"MIDNIGHT DARK", ImVec4(0.008f,0.010f,0.014f,0.990f), ImVec4(0.012f,0.014f,0.020f,0.990f), ImVec4(0.018f,0.021f,0.030f,0.970f), ImVec4(0.026f,0.030f,0.042f,0.970f), ImVec4(0.033f,0.038f,0.052f,0.965f), ImVec4(0.044f,0.050f,0.066f,0.930f), ImVec4(0.250f,0.430f,0.760f,1.0f), ImVec4(0.460f,0.610f,0.920f,1.0f), ImVec4(0.92f,0.94f,0.98f,1.0f), ImVec4(0.52f,0.56f,0.64f,1.0f), ImVec4(0.150f,0.760f,0.440f,1.0f), ImVec4(0.900f,0.620f,0.240f,1.0f), ImVec4(0.880f,0.220f,0.260f,1.0f)},
    {"SOFT CYAN",     ImVec4(0.014f,0.020f,0.032f,0.985f), ImVec4(0.020f,0.030f,0.046f,0.985f), ImVec4(0.030f,0.042f,0.062f,0.965f), ImVec4(0.040f,0.054f,0.078f,0.965f), ImVec4(0.050f,0.066f,0.092f,0.960f), ImVec4(0.062f,0.080f,0.108f,0.920f), ImVec4(0.120f,0.560f,0.780f,1.0f), ImVec4(0.340f,0.760f,0.920f,1.0f), ImVec4(0.92f,0.96f,0.98f,1.0f), ImVec4(0.55f,0.62f,0.70f,1.0f), ImVec4(0.130f,0.760f,0.480f,1.0f), ImVec4(0.900f,0.650f,0.250f,1.0f), ImVec4(0.880f,0.230f,0.280f,1.0f)},
    {"DEEP PURPLE",  ImVec4(0.022f,0.018f,0.034f,0.985f), ImVec4(0.032f,0.026f,0.050f,0.985f), ImVec4(0.046f,0.036f,0.068f,0.965f), ImVec4(0.058f,0.046f,0.086f,0.965f), ImVec4(0.070f,0.056f,0.104f,0.960f), ImVec4(0.084f,0.070f,0.124f,0.920f), ImVec4(0.500f,0.360f,0.820f,1.0f), ImVec4(0.720f,0.560f,0.940f,1.0f), ImVec4(0.96f,0.94f,0.99f,1.0f), ImVec4(0.64f,0.60f,0.72f,1.0f), ImVec4(0.140f,0.760f,0.480f,1.0f), ImVec4(0.900f,0.650f,0.250f,1.0f), ImVec4(0.860f,0.240f,0.320f,1.0f)},
    {"DARK EMBER",   ImVec4(0.032f,0.020f,0.022f,0.985f), ImVec4(0.046f,0.028f,0.030f,0.985f), ImVec4(0.062f,0.038f,0.040f,0.965f), ImVec4(0.078f,0.048f,0.050f,0.965f), ImVec4(0.092f,0.058f,0.060f,0.960f), ImVec4(0.108f,0.070f,0.072f,0.920f), ImVec4(0.760f,0.250f,0.280f,1.0f), ImVec4(0.920f,0.460f,0.360f,1.0f), ImVec4(0.98f,0.94f,0.94f,1.0f), ImVec4(0.70f,0.60f,0.61f,1.0f), ImVec4(0.150f,0.760f,0.460f,1.0f), ImVec4(0.900f,0.660f,0.260f,1.0f), ImVec4(0.880f,0.220f,0.260f,1.0f)},
    {"FOREST DARK",  ImVec4(0.012f,0.026f,0.024f,0.985f), ImVec4(0.018f,0.038f,0.034f,0.985f), ImVec4(0.026f,0.052f,0.046f,0.965f), ImVec4(0.034f,0.068f,0.058f,0.965f), ImVec4(0.044f,0.082f,0.070f,0.960f), ImVec4(0.054f,0.098f,0.084f,0.920f), ImVec4(0.160f,0.620f,0.400f,1.0f), ImVec4(0.420f,0.800f,0.580f,1.0f), ImVec4(0.92f,0.98f,0.94f,1.0f), ImVec4(0.56f,0.68f,0.62f,1.0f), ImVec4(0.140f,0.780f,0.480f,1.0f), ImVec4(0.900f,0.660f,0.260f,1.0f), ImVec4(0.880f,0.220f,0.260f,1.0f)},
    {"WARM GOLD",    ImVec4(0.030f,0.026f,0.020f,0.985f), ImVec4(0.044f,0.036f,0.026f,0.985f), ImVec4(0.060f,0.050f,0.034f,0.965f), ImVec4(0.076f,0.062f,0.040f,0.965f), ImVec4(0.092f,0.076f,0.048f,0.960f), ImVec4(0.110f,0.090f,0.058f,0.920f), ImVec4(0.820f,0.560f,0.210f,1.0f), ImVec4(0.980f,0.760f,0.360f,1.0f), ImVec4(0.98f,0.96f,0.90f,1.0f), ImVec4(0.70f,0.66f,0.56f,1.0f), ImVec4(0.150f,0.760f,0.440f,1.0f), ImVec4(0.900f,0.660f,0.260f,1.0f), ImVec4(0.880f,0.240f,0.220f,1.0f)}
};

struct ZeusTabInfo {
    const char* code;
    const char* title;
    const char* subtitle;
};

static ZeusTabInfo ZeusTabs[] = {
    {"01", "Dashboard", "Quick control"},
    {"02", "Vision",    "Player overlay"},
    {"03", "World",     "Loot and alerts"},
    {"04", "Control",   "Aim tuning"},
    {"05", "Studio",    "Theme/config"},
    {"06", "Diagnostics","Runtime data"}
};

struct ZeusToggleDef {
    const char* title;
    const char* desc;
    bool* value;
};

static float ZClamp(float v, float mn, float mx) {
    if (v < mn) return mn;
    if (v > mx) return mx;
    return v;
}

static ImVec4 ZAlpha(ImVec4 c, float a) {
    c.w = a;
    return c;
}

static ImVec4 ZMix(ImVec4 a, ImVec4 b, float t) {
    return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
}

static int ZThemeCount() {
    return (int)(sizeof(ZeusThemes) / sizeof(ZeusThemes[0]));
}

static ZeusTheme& ZeusCurrentTheme() {
    int idx = intIo[0];
    if (idx < 0 || idx >= ZThemeCount()) idx = 0;
    intIo[0] = idx;
    return ZeusThemes[idx];
}

static void ZPushStyle(const ZeusTheme& t) {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, t.window);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, t.surface);
    ImGui::PushStyleColor(ImGuiCol_Border, ZAlpha(t.accent, 0.34f));
    ImGui::PushStyleColor(ImGuiCol_Text, t.text);
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, t.muted);
    ImGui::PushStyleColor(ImGuiCol_Button, ZAlpha(t.cardSoft, 0.98f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ZAlpha(t.accent, 0.34f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ZAlpha(t.accent2, 0.42f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, t.card);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ZMix(t.card, t.accent, 0.18f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ZMix(t.card, t.accent, 0.28f));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, t.accent);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, t.accent2);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 28.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 22.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 16.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 16.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 12));
}

static void ZPopStyle() {
    ImGui::PopStyleVar(7);
    ImGui::PopStyleColor(13);
}

static void ZDrawWindowBackdrop(ImDrawList* dl, ImVec2 p, ImVec2 s, const ZeusTheme& t, float rounding) {
    ImVec2 br(p.x + s.x, p.y + s.y);
    dl->AddRectFilled(p, br, ImGui::GetColorU32(ZMix(t.window, t.accent, 0.045f)), rounding);
    dl->AddCircleFilled(ImVec2(br.x - 120.0f, p.y + 88.0f), 150.0f, ImGui::GetColorU32(ZAlpha(t.accent2, 0.055f)));
    dl->AddCircleFilled(ImVec2(p.x + 84.0f, br.y - 80.0f), 118.0f, ImGui::GetColorU32(ZAlpha(t.accent, 0.050f)));
    dl->AddRect(p, br, ImGui::GetColorU32(ZAlpha(t.accent, 0.42f)), rounding, 0, 2.0f);
    dl->AddRect(ImVec2(p.x + 5, p.y + 5), ImVec2(br.x - 5, br.y - 5), ImGui::GetColorU32(ZAlpha(t.accent2, 0.10f)), rounding - 4.0f, 0, 1.0f);
}

static void ZText(const char* text, ImVec4 color) {
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
}

static void ZTextWrappedColor(const char* text, ImVec4 color) {
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::TextWrapped("%s", text);
    ImGui::PopStyleColor();
}

static void ZStatusDot(const ZeusTheme& t, ImVec4 c, const char* label) {
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    float h = 30.0f;
    float w = ImGui::CalcTextSize(label).x + 36.0f;
    ImGui::InvisibleButton(label, ImVec2(w, h));
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), ImGui::GetColorU32(ZAlpha(t.cardSoft, 0.78f)), h * 0.5f);
    dl->AddCircleFilled(ImVec2(p.x + 16.0f, p.y + h * 0.5f), 5.0f, ImGui::GetColorU32(c));
    dl->AddText(ImVec2(p.x + 28.0f, p.y + 7.0f), ImGui::GetColorU32(t.text), label);
}

static bool ZActionButton(const char* label, ImVec2 size, ImVec4 base, ImVec4 hover, ImVec4 active) {
    ImGui::PushStyleColor(ImGuiCol_Button, base);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, active);
    bool clicked = ImGui::Button(label, size);
    ImGui::PopStyleColor(3);
    return clicked;
}

static void ZSwitch(const char* id, bool* v, const ZeusTheme& t) {
    ImGui::PushID(id);
    ImVec2 p = ImGui::GetCursorScreenPos();
    float h = 34.0f;
    float w = 64.0f;
    bool changed = ImGui::InvisibleButton("##switch", ImVec2(w, h));
    if (changed) *v = !*v;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec4 bg = *v ? t.accent : ImVec4(0.20f, 0.23f, 0.30f, 1.0f);
    ImVec4 bg2 = *v ? t.accent2 : ImVec4(0.27f, 0.30f, 0.38f, 1.0f);
    dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), ImGui::GetColorU32(ZMix(bg, bg2, 0.45f)), h * 0.5f);
    dl->AddRect(p, ImVec2(p.x + w, p.y + h), ImGui::GetColorU32(ZAlpha(t.text, 0.16f)), h * 0.5f, 0, 1.0f);
    float knobX = *v ? (p.x + w - h * 0.5f) : (p.x + h * 0.5f);
    dl->AddCircleFilled(ImVec2(knobX, p.y + h * 0.5f), h * 0.5f - 4.0f, IM_COL32(250, 253, 255, 255));
    ImGui::PopID();
}

static void ZEdgeDragWindow(const char* prefix, const ImVec2& size, ImGuiIO& io, float edge = 18.0f) {
    // Window movement is intentionally limited to edges/corners so sliders and touch scrolling
    // in the center of the menu are not hijacked by drag behavior.
    ImVec2 wp = ImGui::GetWindowPos();
    auto drag_zone = [&](const char* id, ImVec2 pos, ImVec2 zoneSize) {
        ImGui::SetCursorPos(pos);
        ImGui::InvisibleButton(id, zoneSize);
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImGui::SetWindowPos(ImVec2(wp.x + io.MouseDelta.x, wp.y + io.MouseDelta.y));
        }
    };
    char id[64];
    snprintf(id, sizeof(id), "##%s_top_edge", prefix);    drag_zone(id, ImVec2(0, 0), ImVec2(size.x, edge));
    snprintf(id, sizeof(id), "##%s_left_edge", prefix);   drag_zone(id, ImVec2(0, edge), ImVec2(edge, size.y - edge * 2.0f));
    snprintf(id, sizeof(id), "##%s_right_edge", prefix);  drag_zone(id, ImVec2(size.x - edge, edge), ImVec2(edge, size.y - edge * 2.0f));
    snprintf(id, sizeof(id), "##%s_bottom_edge", prefix); drag_zone(id, ImVec2(0, size.y - edge), ImVec2(size.x, edge));
}

static void ZMiniLauncher(const ZeusTheme& t, ImGuiIO& io) {
    static bool miniPlaced = false;
    static float dragDistance = 0.0f;
    float miniW = 202.0f;
    float miniH = 76.0f;
    if (!miniPlaced) {
        ImGui::SetNextWindowPos(ImVec2(px - miniW * 0.5f, py - miniH * 0.5f), ImGuiCond_Always);
        miniPlaced = true;
    }
    ImGui::SetNextWindowSize(ImVec2(miniW, miniH), ImGuiCond_Always);
    ImGui::Begin("##ZEUS_KERNEL_FLOAT", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    ImVec2 br(wp.x + ws.x, wp.y + ws.y);

    // Clean dark launcher: fixed neutral base, soft theme accent, no heavy neon fill.
    ImVec4 base      = ImVec4(0.020f, 0.023f, 0.032f, 0.965f);
    ImVec4 baseSoft  = ImVec4(0.036f, 0.041f, 0.056f, 0.940f);
    ImVec4 edge      = ZAlpha(t.accent2, 0.34f);
    ImVec4 glow      = ZAlpha(t.accent, 0.085f);
    ImVec4 label     = ImVec4(0.940f, 0.965f, 1.000f, 1.0f);
    ImVec4 subLabel  = ImVec4(0.560f, 0.610f, 0.710f, 1.0f);

    dl->AddRectFilled(wp, br, ImGui::GetColorU32(base), 22.0f);
    dl->AddRectFilled(ImVec2(wp.x + 2.0f, wp.y + 2.0f), ImVec2(br.x - 2.0f, br.y - 2.0f), ImGui::GetColorU32(baseSoft), 20.0f);
    dl->AddCircleFilled(ImVec2(wp.x + 40.0f, wp.y + ws.y * 0.50f), 27.0f, ImGui::GetColorU32(glow));
    dl->AddRect(wp, br, ImGui::GetColorU32(edge), 22.0f, 0, 1.6f);
    dl->AddRect(ImVec2(wp.x + 5.0f, wp.y + 5.0f), ImVec2(br.x - 5.0f, br.y - 5.0f), ImGui::GetColorU32(ZAlpha(t.text, 0.075f)), 18.0f, 0, 1.0f);
    dl->AddRectFilled(ImVec2(wp.x + 14.0f, wp.y + 18.0f), ImVec2(wp.x + 48.0f, wp.y + 64.0f), ImGui::GetColorU32(ZAlpha(t.accent, 0.18f)), 14.0f);
    dl->AddCircleFilled(ImVec2(wp.x + 31.0f, wp.y + 41.0f), 5.2f, ImGui::GetColorU32(t.accent2));

    // Whole mini card is clickable to open, and draggable to move. Dragging will not open it.
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::InvisibleButton("##mini_whole_card_click_drag", ws);
    if (ImGui::IsItemActivated()) dragDistance = 0.0f;
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        dragDistance += fabsf(io.MouseDelta.x) + fabsf(io.MouseDelta.y);
        ImGui::SetWindowPos(ImVec2(wp.x + io.MouseDelta.x, wp.y + io.MouseDelta.y));
    }
    if (ImGui::IsItemDeactivated()) {
        if (dragDistance < 7.0f) {
            IsMenuOpen = true;
            MemuSwitch = true;
        }
        dragDistance = 0.0f;
    }

    dl->AddText(ImVec2(wp.x + 60.0f, wp.y + 29.0f), ImGui::GetColorU32(label), "ZEUS KERNEL");
    dl->AddCircleFilled(ImVec2(br.x - 24.0f, wp.y + ws.y * 0.5f), 4.0f, ImGui::GetColorU32(t.success));
    ImGui::End();
}

static void ZRadarPositionHandle(const ZeusTheme& t, ImGuiIO& io) {
    float w = 92.0f, h = 52.0f;
    if (NumIo[53] <= 1.0f) NumIo[53] = 180.0f;
    if (NumIo[54] <= 1.0f) NumIo[54] = 260.0f;
    ImGui::SetNextWindowPos(ImVec2(NumIo[53] - w * 0.5f, NumIo[54] - h * 0.5f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
    ImGui::Begin("##ZEUS_RADAR_HANDLE", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), ImGui::GetColorU32(ZAlpha(t.surface2, 0.72f)), 14.0f);
    dl->AddRect(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), ImGui::GetColorU32(t.accent2), 14.0f, 0, 1.4f);
    dl->AddText(ImVec2(wp.x + 15.0f, wp.y + 16.0f), ImGui::GetColorU32(t.text), "RADAR");
    ImGui::SetCursorPos(ImVec2(0, 0));
    ImGui::InvisibleButton("##radar_drag", ws);
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        NumIo[53] += io.MouseDelta.x;
        NumIo[54] += io.MouseDelta.y;
    }
    ImGui::End();
}

static bool ZNavItem(int index, const ZeusTheme& t, float width) {
    bool selected = (Tab == index);
    ImGui::PushID(index);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 size(width, 66.0f);
    bool clicked = ImGui::InvisibleButton("##nav", size);
    if (clicked) Tab = index;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec4 bg = selected ? ZMix(t.accent, t.card, 0.28f) : ZAlpha(t.cardSoft, 0.58f);
    ImVec4 border = selected ? t.accent2 : ZAlpha(t.text, 0.08f);
    dl->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), ImGui::GetColorU32(bg), 18.0f);
    dl->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), ImGui::GetColorU32(ZAlpha(border, selected ? 0.70f : 0.18f)), 18.0f, 0, selected ? 2.0f : 1.0f);
    if (selected) dl->AddRectFilled(ImVec2(p.x + 5.0f, p.y + 12.0f), ImVec2(p.x + 10.0f, p.y + size.y - 12.0f), ImGui::GetColorU32(t.accent2), 4.0f);
    dl->AddText(ImVec2(p.x + 22.0f, p.y + 13.0f), ImGui::GetColorU32(selected ? t.accent2 : t.muted), ZeusTabs[index].code);
    dl->AddText(ImVec2(p.x + 62.0f, p.y + 11.0f), ImGui::GetColorU32(selected ? t.text : ZAlpha(t.text, 0.86f)), ZeusTabs[index].title);
    ImGui::PopID();
    return clicked;
}

static void ZMetricCard(const char* title, const char* value, const ZeusTheme& t, ImVec2 size) {
    ImGui::PushID(title);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, t.card);
    ImGui::BeginChild("##metric", size, true, ImGuiWindowFlags_NoScrollbar);
    ImVec2 p = ImGui::GetWindowPos();
    ImVec2 s = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 5.0f, p.y + s.y), ImGui::GetColorU32(t.accent), 4.0f);
    ImGui::SetCursorPos(ImVec2(18, 14));
    ZTextWrappedColor(title, t.muted);
    ImGui::SetCursorPos(ImVec2(18, 46));
    ZTextWrappedColor(value, t.accent2);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopID();
}

static void ZHero(const char* overline, const char* title, const char* desc, const ZeusTheme& t) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ZMix(t.surface2, t.accent, 0.10f));
    ImGui::BeginChild("##hero", ImVec2(0, 96), true, ImGuiWindowFlags_NoScrollbar);
    ImVec2 p = ImGui::GetWindowPos();
    ImVec2 s = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, ImVec2(p.x + s.x, p.y + s.y), ImGui::GetColorU32(ZMix(t.surface2, t.accent, 0.10f)), 22.0f);
    dl->AddCircleFilled(ImVec2(p.x + s.x - 74.0f, p.y + 44.0f), 54.0f, ImGui::GetColorU32(ZAlpha(t.accent2, 0.12f)));
    dl->AddCircle(ImVec2(p.x + s.x - 74.0f, p.y + 44.0f), 72.0f, ImGui::GetColorU32(ZAlpha(t.accent, 0.18f)), 64, 2.0f);

    ImGui::SetCursorPos(ImVec2(24, 18));
    ZText(overline, t.accent2);
    ImGui::SetCursorPos(ImVec2(24, 46));
    ZText(title, t.text);
    (void)desc; // Page descriptions disabled.
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

static void ZSectionTitle(const char* title, const char* desc, const ZeusTheme& t) {
    (void)desc; // Section descriptions disabled.
    ImGui::Dummy(ImVec2(1, 2));
    ZText(title, t.text);
    ImGui::Dummy(ImVec2(1, 4));
}

static void ZToggleTile(const char* title, const char* desc, bool* value, const ZeusTheme& t, ImVec2 size) {
    (void)desc; // Descriptions under options are intentionally disabled.
    ImGui::PushID(title);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, *value ? ZMix(t.card, t.accent, 0.15f) : t.card);
    ImGui::BeginChild("##toggle_tile", size, true, ImGuiWindowFlags_NoScrollbar);
    ImVec2 p = ImGui::GetWindowPos();
    ImVec2 s = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), ImGui::GetColorU32(*value ? ZAlpha(t.accent2, 0.52f) : ZAlpha(t.text, 0.08f)), 18.0f, 0, *value ? 2.0f : 1.0f);
    dl->AddCircleFilled(ImVec2(p.x + 25.0f, p.y + size.y * 0.5f), 8.0f, ImGui::GetColorU32(*value ? t.accent2 : ZAlpha(t.muted, 0.55f)));

    ImGui::SetCursorPos(ImVec2(48, size.y * 0.5f - 10.0f));
    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + size.x - 140.0f);
    ZTextWrappedColor(title, t.text);
    ImGui::PopTextWrapPos();
    ImGui::SetCursorPos(ImVec2(size.x - 82.0f, size.y * 0.5f - 17.0f));
    ZSwitch("##switcher", value, t);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopID();
}

static void ZRenderToggleGrid(const ZeusToggleDef* items, int count, const ZeusTheme& t, int columns) {
    if (columns < 1) columns = 1;
    float gap = 14.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    float tileW = (avail - gap * (float)(columns - 1)) / (float)columns;
    if (tileW < 260.0f && columns > 1) {
        columns = 1;
        tileW = avail;
    }
    for (int i = 0; i < count; ++i) {
        ZToggleTile(items[i].title, items[i].desc, items[i].value, t, ImVec2(tileW, 78.0f));
        if ((i % columns) != columns - 1 && i != count - 1) ImGui::SameLine(0.0f, gap);
    }
}

static void ZSliderTile(const char* title, float* value, float minv, float maxv, const ZeusTheme& t, bool showValue = true) {
    ImGui::PushID(title);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ZAlpha(t.card, 0.58f));
    ImGui::BeginChild("##slider_tile", ImVec2(0, 104), false, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(18, 10));
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 18.0f);
    CustomSliderFloat(title, value, minv, maxv, showValue);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopID();
}

static void ZComboTile(const char* title, float* current, const char* items[], int count, const ZeusTheme& t) {
    ImGui::PushID(title);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, t.card);
    ImGui::BeginChild("##combo_tile", ImVec2(0, 98), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(18, 14));
    ZText(title, t.text);
    ImGui::SetCursorPos(ImVec2(18, 50));
    CustomCombo(title, current, items, count);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopID();
}

static void ZRenderThemeSelector(const ZeusTheme& t) {
    ZSectionTitle("Theme Engine", "Pick a complete visual identity. This changes the whole command center, not only accent colors.", t);
    float gap = 12.0f;
    float avail = ImGui::GetContentRegionAvail().x;
    int columns = 2;
    float w = (avail - gap) * 0.5f;
    if (w < 250.0f) { columns = 1; w = avail; }
    for (int i = 0; i < ZThemeCount(); ++i) {
        ImGui::PushID(i);
        bool selected = (intIo[0] == i);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, selected ? ZMix(ZeusThemes[i].card, ZeusThemes[i].accent, 0.16f) : ZeusThemes[i].card);
        ImGui::BeginChild("##theme", ImVec2(w, 86), true, ImGuiWindowFlags_NoScrollbar);
        ImVec2 p = ImGui::GetWindowPos();
        ImVec2 s = ImGui::GetWindowSize();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddRect(p, ImVec2(p.x + s.x, p.y + s.y), ImGui::GetColorU32(selected ? ZeusThemes[i].accent2 : ZAlpha(t.text, 0.10f)), 18.0f, 0, selected ? 2.0f : 1.0f);
        dl->AddRectFilledMultiColor(ImVec2(p.x + 18, p.y + 20), ImVec2(p.x + 78, p.y + 64),
            ImGui::GetColorU32(ZeusThemes[i].accent), ImGui::GetColorU32(ZeusThemes[i].accent2),
            ImGui::GetColorU32(ZeusThemes[i].surface), ImGui::GetColorU32(ZeusThemes[i].accent));
        ImGui::SetCursorPos(ImVec2(96, 18));
        ZText(ZeusThemes[i].name, selected ? ZeusThemes[i].accent2 : t.text);
        ImGui::SetCursorPos(ImVec2(96, 46));
        ZText(selected ? "ACTIVE THEME" : "Tap to activate", t.muted);
        ImGui::SetCursorPos(ImVec2(0, 0));
        if (ImGui::InvisibleButton("##theme_hit", ImVec2(s.x, s.y))) intIo[0] = i;
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopID();
        if ((i % columns) != columns - 1 && i != ZThemeCount() - 1) ImGui::SameLine(0.0f, gap);
    }
}

static void ZRailFooter(const ZeusTheme& t) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ZAlpha(t.card, 0.76f));
    ImGui::BeginChild("##rail_footer", ImVec2(0, 92), true, ImGuiWindowFlags_NoScrollbar);
    ZText("THEME", t.muted);
    ImGui::Spacing();
    ZText(ZeusCurrentTheme().name, t.accent2);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

static void ZDashboard(const ZeusTheme& t) {
    ZHero("ZEUS KERNEL", "Dashboard", "A cleaner launch screen for fast control, safer spacing, clear status blocks, and manual config actions.", t);
    ImGui::Spacing();
    float gap = 14.0f;
    float w = (ImGui::GetContentRegionAvail().x - gap * 2.0f) / 3.0f;
    ZMetricCard("Overlay State", IsMenuOpen ? "MENU OPEN" : "FLOATING", t, ImVec2(w, 92)); ImGui::SameLine(0.0f, gap);
    ZMetricCard("Theme", t.name, t, ImVec2(w, 92)); ImGui::SameLine(0.0f, gap);
    char fpsText[32]; snprintf(fpsText, sizeof(fpsText), "%d FPS", FPS);
    ZMetricCard("Render", fpsText, t, ImVec2(w, 92));

    ZSectionTitle("Command Deck", "Primary actions are grouped here so they are easy to hit on touch screens.", t);
    float btnW = (ImGui::GetContentRegionAvail().x - gap * 3.0f) / 4.0f;
    if (ZActionButton("RELOAD", ImVec2(btnW, 58), t.accent, t.accent2, t.accent)) DrawInt();
    ImGui::SameLine(0.0f, gap);
    if (ZActionButton("SAVE", ImVec2(btnW, 58), t.success, ZMix(t.success, t.accent2, 0.30f), t.success)) { SaveWeaponConfig(); NumIoSave("ZEUS_CONFIG"); }
    ImGui::SameLine(0.0f, gap);
    if (ZActionButton("LOAD", ImVec2(btnW, 58), t.cardSoft, ZMix(t.cardSoft, t.accent, 0.45f), t.accent)) { LoadWeaponConfig(); NumIoLoad("ZEUS_CONFIG"); }
    ImGui::SameLine(0.0f, gap);
    if (ZActionButton("EXIT", ImVec2(btnW, 58), t.danger, ImVec4(1.0f,0.34f,0.38f,1.0f), ImVec4(0.72f,0.08f,0.12f,1.0f))) exit(0);

    ZSectionTitle("System Modifiers", "Core overlay behavior and shortcut options.", t);
    ZeusToggleDef homeToggles[] = {
        {"Anti Screen Recording", "Hide the overlay from capture when the device supports it.", &DrawIo[42]},
        {"No Background Mode", "Use a lighter transparent drawing background.", &DrawIo[41]},
        {"Game FPS Booster", "Reduce menu render load with a draw FPS limiter.", &DrawIo[40]},
        {"Volume Shortcut", "Use the volume shortcut for quick control while playing.", &DrawIo[103]}
    };
    ZRenderToggleGrid(homeToggles, 4, t, 2);
    ZSectionTitle("Performance", "Limit overlay draw rate when you want a lighter menu.", t);
    ZSliderTile("Draw FPS Limit", &NumIo[12], 30.0f, 144.0f, t, true);
}

static void ZVision(const ZeusTheme& t) {
    ZHero("VISUAL INTELLIGENCE", "Vision", "Player overlay modules are organized as compact cards with clear names, descriptions, and touch-safe toggles.", t);
    ZSectionTitle("Player Information", "Identity, distance, health, line and structure modules.", t);
    ZeusToggleDef espToggles[] = {
        {"Enemy Counter", "Show the alive enemy count indicator.", &DrawIo[79]},
        {"Player Skeleton", "Draw player bone structure.", &DrawIo[3]},
        {"2D Box", "Draw a clean player box.", &DrawIo[1]},
        {"Snap Lines", "Draw directional lines to targets.", &DrawIo[2]},
        {"Health Bar", "Show current health state.", &DrawIo[4]},
        {"Distance", "Show distance text in meters.", &DrawIo[5]},
        {"Player Name", "Show player name text.", &DrawIo[6]},
        {"Team ID", "Show squad number beside players.", &DrawIo[8]},
        {"Weapon Info", "Show current weapon information.", &DrawIo[11]},
        {"Radar", "Enable the mini radar module.", &DrawIo[10]}
    };
    ZRenderToggleGrid(espToggles, 10, t, 2);
    ZSectionTitle("Vision Tuning", "Adjust visual weight and radar scale.", t);
    ZSliderTile("Visual Thickness", &NumIo[32], 0.1f, 10.0f, t, true);
    ZSliderTile("Radar Zoom", &NumIo[17], 0.0f, 500.0f, t, true);
}

static void ZWorld(const ZeusTheme& t) {
    ZHero("WORLD SCANNER", "World", "Vehicle, loot and map object controls.", t);
    ZSectionTitle("Map Objects", "", t);
    ZeusToggleDef worldToggles[] = {
        {"Vehicle ESP", "", &DrawIo[14]},
        {"Death Crates", "", &Config.showChest},
        {"Air Drops", "", &Config.showDrop},
        {"Grenade Alert", "", &DrawIo[15]},
        {"Item ESP", "Show only selected loot items.", &DrawIo[104]}
    };
    ZRenderToggleGrid(worldToggles, 5, t, 2);

    if (DrawIo[104]) {
        ZSectionTitle("Item ESP List", "Each item has its own switch and color profile.", t);
        float gap = 12.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        int columns = 2;
        float tileW = (avail - gap) * 0.5f;
        if (tileW < 260.0f) { columns = 1; tileW = avail; }
        for (int i = 0; i < ZEUS_ITEM_ESP_COUNT; ++i) {
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, g_ZeusItemESP[i].enabled ? ZMix(t.card, t.accent, 0.14f) : t.card);
            ImGui::BeginChild("##item_esp_tile", ImVec2(tileW, 76), true, ImGuiWindowFlags_NoScrollbar);
            ImVec2 p = ImGui::GetWindowPos();
            ImVec2 sz = ImGui::GetWindowSize();
            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddRect(p, ImVec2(p.x + sz.x, p.y + sz.y), ImGui::GetColorU32(g_ZeusItemESP[i].enabled ? ZAlpha(t.accent2, 0.45f) : ZAlpha(t.text, 0.08f)), 16.0f, 0, g_ZeusItemESP[i].enabled ? 2.0f : 1.0f);
            dl->AddCircleFilled(ImVec2(p.x + 22.0f, p.y + 38.0f), 7.0f, ImGui::GetColorU32(g_ZeusItemESP[i].color));
            ImGui::SetCursorPos(ImVec2(42, 24));
            ImGui::Checkbox(g_ZeusItemESP[i].name, &g_ZeusItemESP[i].enabled);
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopID();
            if ((i % columns) != columns - 1 && i != ZEUS_ITEM_ESP_COUNT - 1) ImGui::SameLine(0.0f, gap);
        }
    }

    ZeusSyncItemESPConfig();
    ZSectionTitle("World Tuning", "", t);
    ZSliderTile("Item Max Distance", &NumIo[33], 20.0f, 400.0f, t, true);
}

static void ZControl(const ZeusTheme& t) {
    ZHero("CONTROL LAB", "Control", "Aim-related settings are grouped into activation, target selection, touch behavior, and recoil tuning sections.", t);
    ZSectionTitle("Activation", "Main modules and targeting filters.", t);
    ZeusToggleDef aimToggles[] = {
        {"Aimbot", "Enable the main aim system.", &DrawIo[20]},
        {"Sniper Aimbot", "Enable sniper-specific handling.", &DrawIo[33]},
        {"Shotgun Aimbot", "Enable shotgun-specific handling.", &DrawIo[38]},
        {"Touch Position", "Show or edit touch control area.", &DrawIo[21]},
        {"Ignore Knocked", "Skip knocked players.", &DrawIo[22]},
        {"Ignore Bots", "Skip bot targets.", &DrawIo[23]},
        {"Hide FOV Circle", "Hide the aim FOV indicator.", &DrawIo[27]},
        {"Click Touch", "Use click-touch behavior.", &DrawIo[56]}
    };
    ZRenderToggleGrid(aimToggles, 8, t, 2);

    ZSectionTitle("Target Profile", "Select how targeting is resolved.", t);
    const char* chargePos[] = {"Charging Port Right", "Charging Port Left"};
    ZComboTile("Charging Position", &NumIo[41], chargePos, 2, t);
    NumIo[10] = (NumIo[41] == 0) ? 0.0f : 1.0f;
    const char* targetItems[] = {"Head", "Chest", "Pelvis"};
    ZComboTile("Target Bone", &NumIo[42], targetItems, 3, t);
    if (NumIo[42] == 0) NumIo[8] = 1.0f; else if (NumIo[42] == 1) NumIo[8] = 2.0f; else NumIo[8] = 3.0f;
    const char* modes[] = {"Fire", "Scope"};
    ZComboTile("Aimbot Mode", &NumIo[43], modes, 2, t);
    NumIo[0] = (NumIo[43] == 0) ? 0.0f : 1.0f;

    ZSectionTitle("Motion Tuning", "Speed, smoothness, prediction and distance limits.", t);
    ZSliderTile("FOV Radius", &NumIo[3], 10.0f, 600.0f, t, true);
    if (DrawIo[56]) ZSliderTile("Click Range", &NumIo[50], 5.0f, 600.0f, t, true);
    if (DrawIo[21]) ZSliderTile("Touch Radius", &NumIo[7], 5.0f, 600.0f, t, true);
    ZSliderTile("Aimbot Speed", &NumIo[16], 0.0f, 30.0f, t, true);
    ZSliderTile("Smoothness", &NumIo[9], 0.0f, 30.0f, t, true);
    ZSliderTile("Prediction Velocity", &NumIo[36], 0.0f, 30.0f, t, true);
    ZSliderTile("Hipfire Distance", &NumIo[98], 25.0f, 500.0f, t, true);
    ZSliderTile("Aimbot Distance", &NumIo[30], 0.0f, 500.0f, t, true);

    ZSectionTitle("Weapon Pressure", "Per-stance recoil pressure values for the current weapon slot.", t);
    ZSliderTile("Overall Recoil", &g_WeaponPressureDB[MyWeapon], 0.1f, 10.0f, t, true);
    ZSliderTile("Crouch Recoil", &g_WeaponPressureDX[MyWeapon], 0.1f, 10.0f, t, true);
    ZSliderTile("Prone Recoil", &g_WeaponPressurePX[MyWeapon], 0.1f, 10.0f, t, true);
    ZSliderTile("Click Speed", &NumIo[76], 0.0f, 15.0f, t, true);
    ZSliderTile("Shotgun Distance", &NumIo[68], 0.0f, 500.0f, t, true);
}

static void ZStudio(const ZeusTheme& t) {
    ZHero("UI STUDIO", "Studio", "Theme selection and configuration actions live here so the main pages stay clean.", t);
    ZRenderThemeSelector(t);
    ZSectionTitle("Configuration", "Changes are not auto-saved. Save manually when you want to keep them.", t);
    float gap = 14.0f;
    float w = (ImGui::GetContentRegionAvail().x - gap) * 0.5f;
    if (ZActionButton("SAVE CONFIG", ImVec2(w, 60), t.success, ZMix(t.success, t.accent2, 0.30f), t.success)) { SaveWeaponConfig(); NumIoSave("ZEUS_CONFIG"); }
    ImGui::SameLine(0.0f, gap);
    if (ZActionButton("LOAD SAVED", ImVec2(w, 60), t.cardSoft, ZMix(t.cardSoft, t.accent, 0.45f), t.accent)) { LoadWeaponConfig(); NumIoLoad("ZEUS_CONFIG"); }
    ImGui::Spacing();
    ZTextWrappedColor("Crash log: /storage/emulated/0/ZEUS_KERNEL_CRASH.log", t.muted);
}

static void ZDiagnostics(const ZeusTheme& t) {
    ZHero("RUNTIME DIAGNOSTICS", "Diagnostics", "Live overview for render status, driver state, tracked players and memory base information.", t);
    float gap = 14.0f;
    float w = (ImGui::GetContentRegionAvail().x - gap) * 0.5f;
    char baseStr[64]; snprintf(baseStr, sizeof(baseStr), libUE4 > 0 ? "0x%lx" : "NOT FOUND", libUE4);
    ZMetricCard("libUE4 Base", baseStr, t, ImVec2(w, 96)); ImGui::SameLine(0.0f, gap);
    ZMetricCard("Driver Status", "ONLINE", t, ImVec2(w, 96));
    char fpsStr[32]; snprintf(fpsStr, sizeof(fpsStr), "%d FPS", FPS);
    ZMetricCard("Render FPS", fpsStr, t, ImVec2(w, 96)); ImGui::SameLine(0.0f, gap);
    char maxP[32]; extern int MaxPlayerCount; snprintf(maxP, sizeof(maxP), "%d", MaxPlayerCount);
    ZMetricCard("Max Players Tracked", maxP, t, ImVec2(w, 96));
    ZSectionTitle("Details", "Useful notes for checking what the overlay is doing.", t);
    ZTextWrappedColor("Vehicle names are displayed in English. Object class-name ESP is disabled; Item ESP uses the selected item list only.", t.muted);
}

void win1() {
    ImGuiIO& io = ImGui::GetIO();
    ZeusTheme& theme = ZeusCurrentTheme();
    MemuSwitch = IsMenuOpen;

    ZPushStyle(theme);
    if (DrawIo[10] && MemuSwitch) {
        ZRadarPositionHandle(theme, io);
    }

    if (!MemuSwitch) {
        ZMiniLauncher(theme, io);
        ZPopStyle();
        return;
    }

    static bool menuPlaced = false;
    float displayW = io.DisplaySize.x > 1.0f ? io.DisplaySize.x : 1280.0f;
    float displayH = io.DisplaySize.y > 1.0f ? io.DisplaySize.y : 800.0f;
    float UI_W = ZClamp(displayW * 0.86f, 980.0f, 1280.0f);
    float UI_H = ZClamp(displayH * 0.82f, 640.0f, 850.0f);
    if (!menuPlaced) {
        ImGui::SetNextWindowPos(ImVec2(px - UI_W * 0.5f, py - UI_H * 0.5f), ImGuiCond_Always);
        menuPlaced = true;
    }
    ImGui::SetNextWindowSize(ImVec2(UI_W, UI_H), ImGuiCond_Always);

    ImGui::Begin("ZEUS KERNEL##MAIN", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    ZDrawWindowBackdrop(dl, wp, ws, theme, 28.0f);

    // Edge-only drag zones. The center of the menu is reserved for touch scrolling/sliders.
    ZEdgeDragWindow("main", ws, io, 18.0f);

    ImGui::SetCursorPos(ImVec2(32, 18));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ZAlpha(theme.card, 0.70f));
    ImGui::BeginChild("##brand", ImVec2(360, 62), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(18, 10));
    ZText("ZEUS KERNEL", theme.text);
    ImGui::SetCursorPos(ImVec2(18, 36));
    ZText("Premium tactical dashboard", theme.muted);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(UI_W - 410.0f, 30.0f));
    ZStatusDot(theme, theme.success, "READY");
    ImGui::SameLine(0.0f, 10.0f);
    ZStatusDot(theme, theme.accent2, theme.name);

    ImGui::SetCursorPos(ImVec2(UI_W - 78.0f, 22.0f));
    if (ZActionButton("X", ImVec2(52, 50), theme.danger, ImVec4(1.0f,0.35f,0.38f,1.0f), ImVec4(0.70f,0.08f,0.12f,1.0f))) {
        IsMenuOpen = false;
        MemuSwitch = false;
    }

    float railW = 262.0f;
    float topY = 96.0f;
    float pad = 24.0f;
    float railH = UI_H - topY - pad;
    float contentX = pad + railW + 18.0f;
    float contentW = UI_W - contentX - pad;
    float contentH = railH;

    ImGui::SetCursorPos(ImVec2(pad, topY));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, theme.rail);
    ImGui::BeginChild("##zeus_rail", ImVec2(railW, railH), true, ImGuiWindowFlags_NoScrollbar);
    ImVec2 railAvail = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPos(ImVec2(18, 18));
    ZText("NAVIGATION", theme.muted);
    ImGui::SetCursorPosX(16);
    for (int i = 0; i < 6; ++i) {
        ZNavItem(i, theme, railW - 32.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8.0f);
    }
    float footerY = railH - 144.0f;
    if (footerY > ImGui::GetCursorPosY()) ImGui::SetCursorPosY(footerY);
    ZRailFooter(theme);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(contentX, topY));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, theme.surface);
    ImGui::BeginChild("##zeus_content_shell", ImVec2(contentW, contentH), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(18, 18));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ZAlpha(theme.window, 0.28f));
    ImGui::BeginChild("##zeus_content_scroll", ImVec2(contentW - 36.0f, contentH - 36.0f), false, ImGuiWindowFlags_NoScrollbar);

    if (Tab == 0) ZDashboard(theme);
    else if (Tab == 1) ZVision(theme);
    else if (Tab == 2) ZWorld(theme);
    else if (Tab == 3) ZControl(theme);
    else if (Tab == 4) ZStudio(theme);
    else if (Tab == 5) ZDiagnostics(theme);

    TouchScroll(true);
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::End();
    ZPopStyle();
}
