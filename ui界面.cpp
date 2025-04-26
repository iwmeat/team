#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>

// ��ť�ṹ��
struct Button {
    IMAGE img;          // ��ťͼƬ
    float x, y;         // ��ǰλ��
    float originX, originY; // ԭʼλ��
    float width, height;// ԭʼ�ߴ�
    float scale;        // ��ǰ���ű���
    bool isHover;       // ��ͣ״̬
    bool isAnimating;   // �������״̬
};

// ��ʼ��ͼƬ��ť
void InitButton(Button* btn, const TCHAR* path, float x, float y) {
    // ����ͼƬ������͸��ͨ��
    loadimage(&btn->img, path, 0, 0, true);
    btn->originX = btn->x = x;
    btn->originY = btn->y = y;
    btn->width = btn->img.getwidth();
    btn->height = btn->img.getheight();
    btn->scale = 1.0f;
    btn->isHover = false;
    btn->isAnimating = false;
}

// ���°�ť״̬��������������
void UpdateButton(Button* btn, float deltaTime) {
    const float maxScale = 1.2f;    // ��ͣ�Ŵ����
    const float clickScale = 0.9f;  // �����С����
    const float animationSpeed = 8.0f;

    // ����Ŀ�����ű���
    float targetScale = 1.0f;
    if (btn->isAnimating) {
        targetScale = clickScale;
    }
    else if (btn->isHover) {
        targetScale = maxScale;
    }

    // ƽ���������ű���
    btn->scale += (targetScale - btn->scale) * animationSpeed * deltaTime;

    // �������ź�λ�ã��������ĵ㲻�䣩
    float newWidth = btn->width * btn->scale;
    float newHeight = btn->height * btn->scale;
    btn->x = btn->originX - newWidth / 2;
    btn->y = btn->originY - newHeight / 2;
}

// ����ͼƬ��ť���������putimage���ã�
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
    // ��������
    const int winWidth = 1280;
    const int winHeight = 720;

    // ��ʼ������
    initgraph(winWidth, winHeight, EW_SHOWCONSOLE);
    HWND hwnd = GetHWnd();

    // ���д���
    RECT rect;
    GetWindowRect(hwnd, &rect);
    SetWindowPos(hwnd, NULL,
        (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2,
        0, 0, SWP_NOSIZE);

    // ���ر���ͼ
    IMAGE bgImg;
    loadimage(&bgImg, _T("C:\\Users\\liangxin\\Desktop\\ֲ���ս��ʬ�زİ�\\ֲ���ս��ʬ�زİ�\\images\\titlescreen.jpg"), winWidth, winHeight);

    // ��ʼ��ͼƬ��ť
    Button startBtn;
    InitButton(&startBtn,
        _T("C:\\Users\\liangxin\\Desktop\\ֲ���ս��ʬ�زİ�\\ֲ���ս��ʬ�زİ�\\images\\OIP-C (1).jpg"),
        winWidth / 2,    // ����X����
        winHeight * 0.7  // 70%�߶ȴ�
    );

    // ���صڶ�����ͼƬ
    IMAGE gameBg;
    loadimage(&gameBg, _T("C:\\Users\\liangxin\\Desktop\\ֲ���ս��ʬ�زİ�\\ֲ���ս��ʬ�زİ�\\seanim\\SelectorScreen_BG_Right.jpg"), winWidth, winHeight);

    // ʱ�����
    DWORD lastTime = GetTickCount();
    bool inGame = false;

    while (true) {
        // ����ʱ������ƽ��������
        DWORD currentTime = GetTickCount();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        BeginBatchDraw();

        // ���Ƶ�ǰ����
        if (!inGame) {
            // ���Ʊ����Ͱ�ť
            putimage(0, 0, &bgImg);
            UpdateButton(&startBtn, deltaTime);
            DrawButton(&startBtn);
        }
        else {
            // ������Ϸ����
            putimage(0, 0, &gameBg);
        }

        // ��������¼�
        MOUSEMSG msg;
        if (MouseHit()) {
            msg = GetMouseMsg();

            // ���㰴ť��ײ���򣨻������ź�ߴ磩
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

        // ESC�˳����
        if (_kbhit() && _getch() == 27) break;

        EndBatchDraw();
        Sleep(10);
    }

    closegraph();
    return 0;
}