#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>

// 按钮结构体
struct Button {
    IMAGE img;          // 按钮图片
    float x, y;         // 当前位置
    float originX, originY; // 原始位置
    float width, height;// 原始尺寸
    float scale;        // 当前缩放比例
    bool isHover;       // 悬停状态
    bool isAnimating;   // 点击动画状态
};

// 初始化图片按钮
void InitButton(Button* btn, const TCHAR* path, float x, float y) {
    // 加载图片并保留透明通道
    loadimage(&btn->img, path, 0, 0, true);
    btn->originX = btn->x = x;
    btn->originY = btn->y = y;
    btn->width = btn->img.getwidth();
    btn->height = btn->img.getheight();
    btn->scale = 1.0f;
    btn->isHover = false;
    btn->isAnimating = false;
}

// 更新按钮状态（带缓动动画）
void UpdateButton(Button* btn, float deltaTime) {
    const float maxScale = 1.2f;    // 悬停放大比例
    const float clickScale = 0.9f;  // 点击缩小比例
    const float animationSpeed = 8.0f;

    // 计算目标缩放比例
    float targetScale = 1.0f;
    if (btn->isAnimating) {
        targetScale = clickScale;
    }
    else if (btn->isHover) {
        targetScale = maxScale;
    }

    // 平滑过渡缩放比例
    btn->scale += (targetScale - btn->scale) * animationSpeed * deltaTime;

    // 计算缩放后位置（保持中心点不变）
    float newWidth = btn->width * btn->scale;
    float newHeight = btn->height * btn->scale;
    btn->x = btn->originX - newWidth / 2;
    btn->y = btn->originY - newHeight / 2;
}

// 绘制图片按钮（修正后的putimage调用）
void DrawButton(Button* btn) {
    putimage(
        (int)btn->x,
        (int)btn->y,
        (int)(btn->width * btn->scale),
        (int)(btn->height * btn->scale),
        &btn->img,
        0, 0,
        SRCAND
    );
}

int main()
{
    // 窗口设置
    const int winWidth = 1280;
    const int winHeight = 720;

    // 初始化窗口
    initgraph(winWidth, winHeight, EW_SHOWCONSOLE);
    HWND hwnd = GetHWnd();

    // 居中窗口
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(hwnd, NULL,
        (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2,
        0, 0, SWP_NOSIZE);

    // 加载背景图
    IMAGE bgImg;
    loadimage(&bgImg, _T("C:\\Users\\liangxin\\Desktop\\植物大战僵尸素材包\\植物大战僵尸素材包\\images\\titlescreen.jpg"), winWidth, winHeight);

    // 初始化图片按钮
    Button startBtn;
    InitButton(&startBtn,
        _T("C:\\Users\\liangxin\\Desktop\\植物大战僵尸素材包\\植物大战僵尸素材包\\images\\OIP-C (1).jpg"),
        winWidth / 2,    // 中心X坐标
        winHeight * 0.7  // 70%高度处
    );

    // 加载第二场景图片
    IMAGE gameBg;
    loadimage(&gameBg, _T("C:\\Users\\liangxin\\Desktop\\植物大战僵尸素材包\\植物大战僵尸素材包\\seanim\\SelectorScreen_BG_Right.jpg"), winWidth, winHeight);

    // 时间控制
    DWORD lastTime = GetTickCount();
    bool inGame = false;

    while (true) {
        // 计算时间差（用于平滑动画）
        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        BeginBatchDraw();

        // 绘制当前场景
        if (!inGame) {
            // 绘制背景和按钮
            putimage(0, 0, &bgImg);
            UpdateButton(&startBtn, deltaTime);
            DrawButton(&startBtn);
        }
        else {
            // 绘制游戏场景
            putimage(0, 0, &gameBg);
        }

        // 处理鼠标事件
        MOUSEMSG msg;
        if (MouseHit()) {
            msg = GetMouseMsg();

            // 计算按钮碰撞区域（基于缩放后尺寸）
            bool inButton = msg.x > startBtn.x &&
                msg.x < startBtn.x + startBtn.width * startBtn.scale &&
                msg.y > startBtn.y &&
                msg.y < startBtn.y + startBtn.height * startBtn.scale;

            switch (msg.uMsg) {
            case WM_MOUSEMOVE:
                startBtn.isHover = inButton;
                break;

            case WM_LBUTTONDOWN:
                if (inButton && !inGame) {
                    startBtn.isAnimating = true;
                }
                break;

            case WM_LBUTTONUP:
                if (inButton && !inGame) {
                    inGame = true;
                    startBtn.isAnimating = false;
                }
                break;
            }
        }

        // ESC退出检测
        if (_kbhit() && _getch() == 27) break;

        EndBatchDraw();
        Sleep(10);
    }

    closegraph();
    return 0;
}