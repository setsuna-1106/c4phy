// 双摆模拟 SDL2 可视化
// 编译:
//   clang++ -Wall -Wextra -O2 -Iinclude main.cpp src/Physics/DoublePendulum.cpp \
//       src/Math/Vector.cpp -o main $(pkg-config --cflags --libs sdl2 SDL2_ttf)

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cmath>
#include <cstdio>
#include <random>
#include <stdexcept>
#include <string>

#include "Math/Vector.h"
#include "Physics/DoublePendulum.h"

namespace {

constexpr int    kWinW     = 900;      // 窗口宽 [px]
constexpr int    kWinH     = 640;      // 窗口高 [px]
constexpr int    kPivotY   = 60;       // 悬挂点 y 坐标 [px]
constexpr double kDt       = 1.0 / 240.0;  // 物理定步长 [s]
constexpr int    kTrailMax = 600;      // 轨迹点数上限(60fps 下约 10 s)
constexpr double kDeg2Rad  = 3.14159265358979323846 / 180.0;

// SDL2 没有圆形图元,用水平扫描线画实心圆
void draw_circle(SDL_Renderer* r, int cx, int cy, int rad,
                 Uint8 cr, Uint8 cg, Uint8 cb) {
    SDL_SetRenderDrawColor(r, cr, cg, cb, 255);
    for (int dy = -rad; dy <= rad; ++dy) {
        const int dx = static_cast<int>(std::sqrt(rad * rad - dy * dy));
        SDL_RenderDrawLine(r, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

// ---------- Trail: 定长 ring buffer 轨迹,无动态分配 ----------
class Trail {
public:
    void push(float x, float y) {
        pts_[head_] = {x, y};
        head_ = (head_ + 1) % kTrailMax;
        if (len_ < kTrailMax) ++len_;
    }

    void clear() { head_ = 0; len_ = 0; }

    // 从最新点向最旧点绘制,透明度渐隐(保留下限,防止旧点完全不可见)
    void draw(SDL_Renderer* r) const {
        for (int i = 0; i < len_; ++i) {
            const int idx = (head_ - 1 - i + kTrailMax) % kTrailMax;
            const double t = static_cast<double>(len_ - i) / len_;  // 1 新 -> 0 旧
            const Uint8 alpha = static_cast<Uint8>(60 + 165 * t);
            SDL_SetRenderDrawColor(r, 90, 160, 255, alpha);
            const SDL_Rect dot = {static_cast<int>(pts_[idx].x) - 1,
                                  static_cast<int>(pts_[idx].y) - 1, 2, 2};
            SDL_RenderFillRect(r, &dot);
        }
    }

private:
    SDL_FPoint pts_[kTrailMax] = {};
    int head_ = 0;
    int len_  = 0;
};

// ---------- SdlApp: RAII 封装 SDL / 窗口 / 渲染器 / 字体 ----------
class SdlApp {
public:
    SdlApp() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error(SDL_GetError());
        if (TTF_Init() != 0) throw std::runtime_error(TTF_GetError());

        window_ = SDL_CreateWindow("Double Pendulum (C++ / SDL2 / RK4)",
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   kWinW, kWinH, SDL_WINDOW_SHOWN);
        if (!window_) throw std::runtime_error(SDL_GetError());

        renderer_ = SDL_CreateRenderer(window_, -1,
                                       SDL_RENDERER_ACCELERATED
                                           | SDL_RENDERER_PRESENTVSYNC);
        if (!renderer_) throw std::runtime_error(SDL_GetError());
        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);

        font_ = open_font();  // 找不到字体则 HUD 文本自动降级隐藏
    }

    ~SdlApp() {
        if (font_) TTF_CloseFont(font_);
        if (renderer_) SDL_DestroyRenderer(renderer_);
        if (window_) SDL_DestroyWindow(window_);
        TTF_Quit();
        SDL_Quit();
    }

    SdlApp(const SdlApp&) = delete;             // 资源独占,禁止拷贝
    SdlApp& operator=(const SdlApp&) = delete;

    SDL_Renderer* renderer() { return renderer_; }

    void draw_text(int x, int y, const std::string& text, SDL_Color c) {
        if (!font_) return;
        SDL_Surface* surf = TTF_RenderText_Blended(font_, text.c_str(), c);
        if (!surf) return;
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surf);
        if (tex) {
            SDL_Rect dst{x, y, surf->w, surf->h};
            SDL_RenderCopy(renderer_, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
        }
        SDL_FreeSurface(surf);
    }

private:
    static TTF_Font* open_font() {
        static const char* candidates[] = {
            "/System/Library/Fonts/Geneva.ttf",
            "/System/Library/Fonts/Supplemental/Arial.ttf",
            "/System/Library/Fonts/Helvetica.ttc",
            "/System/Library/Fonts/Menlo.ttc",
        };
        for (const char* path : candidates) {
            if (TTF_Font* f = TTF_OpenFont(path, 14)) return f;
        }
        return nullptr;
    }

    SDL_Window*   window_   = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font*     font_     = nullptr;
};

// 随机性留在应用层,物理类保持确定性
DPState random_state(std::mt19937& rng) {
    std::uniform_real_distribution<double> ang(-135.0, 135.0);
    return {ang(rng) * kDeg2Rad, ang(rng) * kDeg2Rad, 0.0, 0.0};
}

}  // namespace

int main() {
    try {
        SdlApp app;
        SDL_Renderer* ren = app.renderer();

        const DPParams params{1.0, 0.8, 1.0, 1.0, 9.81};
        const double scale = (kWinH - kPivotY - 30) / (params.l1 + params.l2);  // px/m

        std::mt19937 rng{std::random_device{}()};
        DoublePendulum pendulum(params, random_state(rng));
        Trail trail;

        // 物理坐标(m,y 向上)-> 屏幕坐标(px,y 向下)
        const auto to_px = [&](const vector2D& v) {
            return SDL_Point{kWinW / 2 + static_cast<int>(v.x * scale),
                             kPivotY - static_cast<int>(v.y * scale)};
        };

        bool running = true;
        bool paused  = false;
        double acc        = 0;   // 时间累加器,物理速率与帧率解耦
        Uint64 prev_cnt  = SDL_GetPerformanceCounter();

        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            paused = !paused;
                            break;
                        case SDLK_r:
                            pendulum.reset(random_state(rng));
                            trail.clear();
                            break;
                        case SDLK_c:
                            trail.clear();
                            break;
                        default:
                            break;
                    }
                }
            }

            // 固定步长推进;clamp 帧耗时防止卡顿后"死亡螺旋"
            const Uint64 now_cnt = SDL_GetPerformanceCounter();
            double frame = static_cast<double>(now_cnt - prev_cnt)
                           / SDL_GetPerformanceFrequency();
            prev_cnt = now_cnt;
            if (frame > 0.05) frame = 0.05;

            if (!paused) {
                acc += frame;
                while (acc >= kDt) {
                    pendulum.step(kDt);
                    acc -= kDt;
                }
                const SDL_Point p2 = to_px(pendulum.bob2());
                trail.push(static_cast<float>(p2.x), static_cast<float>(p2.y));
            }

            // ---- 渲染 ----
            SDL_SetRenderDrawColor(ren, 16, 16, 20, 255);
            SDL_RenderClear(ren);

            trail.draw(ren);

            const SDL_Point p1 = to_px(pendulum.bob1());
            const SDL_Point p2 = to_px(pendulum.bob2());
            const int r1 = static_cast<int>(10 * std::sqrt(params.m1));
            const int r2 = static_cast<int>(10 * std::sqrt(params.m2));

            SDL_SetRenderDrawColor(ren, 220, 220, 220, 255);   // 摆杆
            SDL_RenderDrawLine(ren, kWinW / 2, kPivotY, p1.x, p1.y);
            SDL_RenderDrawLine(ren, p1.x, p1.y, p2.x, p2.y);

            draw_circle(ren, p1.x, p1.y, r1, 230, 90, 90);     // 摆锤 1
            draw_circle(ren, p2.x, p2.y, r2, 90, 200, 120);    // 摆锤 2
            draw_circle(ren, kWinW / 2, kPivotY, 4, 130, 130, 130);  // 悬挂点

            // ---- HUD ----
            char buf[128];
            const SDL_Color white{230, 230, 230, 255};
            const SDL_Color gray{150, 150, 150, 255};
            std::snprintf(buf, sizeof buf, "RK4   dt = 1/240 s   t = %.1f s",
                          pendulum.time());
            app.draw_text(10, 8, buf, white);

            // 能量误差随幅度变色:绿 < 1e-6,黄 < 1e-4,红更高
            const double err = pendulum.energy_error();
            const SDL_Color err_color = std::fabs(err) < 1e-6
                ? SDL_Color{120, 230, 140, 255}
                : std::fabs(err) < 1e-4 ? SDL_Color{240, 200, 90, 255}
                                        : SDL_Color{240, 110, 110, 255};
            std::snprintf(buf, sizeof buf, "energy error = %+.2e", err);
            app.draw_text(10, 28, buf, err_color);

            app.draw_text(10, kWinH - 24,
                          "Space: pause    R: random reset    C: clear trail",
                          gray);
            if (paused) app.draw_text(kWinW / 2 - 34, kWinH / 2, "PAUSED",
                                      SDL_Color{240, 200, 90, 255});

            SDL_RenderPresent(ren);
        }
    } catch (const std::exception& e) {
        std::fprintf(stderr, "init failed: %s\n", e.what());
        return 1;
    }
    return 0;
}
