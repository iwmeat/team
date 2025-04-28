#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
// 最大植物数量和最大存档数量
#define MAX_PLANTS 50
#define MAX_SLOTS 3

// 植物类型枚举
typedef enum {
    PEASHOOTER,
    SUNFLOWER,
    WALLNUT,
    CHERRYBOMB,
    NONE
} PlantType;

// 植物结构体
typedef struct {
    PlantType type;
    int x, y;       // 位置坐标
    int health;     // 生命值
    int cooldown;   // 冷却时间
} Plant;

// 游戏状态结构体
typedef struct {
    int level;          // 当前关卡
    int sun;            // 阳光数量
    Plant plants[MAX_PLANTS]; // 已放置的植物
    int plant_count;    // 当前植物数量
    int wave_number;    // 僵尸波次
    bool is_day;        // 白天/黑夜模式
} GameState;

// 文件头标识和版本号
const char FILE_HEADER[4] = { 'P', 'V', 'Z', 0x1A };
const int VERSION = 1;

// 保存游戏状态到文件
bool save_game(const char* filename, const GameState* state) {
    FILE* file= fopen(filename, "wb");
    if (!file) return false;

    // 写入文件头和版本
    fwrite(FILE_HEADER, sizeof(char), 4, file);
    fwrite(&VERSION, sizeof(int), 1, file);

    // 写入游戏数据
    fwrite(state, sizeof(GameState), 1, file);

    fclose(file);
    return true;
}

// 从文件加载游戏状态
bool load_game(const char* filename, GameState* state) {
    FILE* file = fopen(filename, "rb");
    if (!file) return false;

    // 验证文件头
    char header[4];
    int file_version;
    fread(header, sizeof(char), 4, file);
    fread(&file_version, sizeof(int), 1, file);

    if (memcmp(header, FILE_HEADER, 4) != 0 || file_version != VERSION) {
        fclose(file);
        return false;
    }

    // 读取游戏数据
    fread(state, sizeof(GameState), 1, file);

    fclose(file);
    return true;
}

// 显示存档菜单
void show_save_menu() {
    printf("\n=== 存档系统 ===");
    printf("\n1. 保存进度");
    printf("\n2. 加载进度");
    printf("\n3. 返回游戏");
    printf("\n选择操作: ");
}

// 示例：初始化新游戏
void new_game(GameState* state) {
    state->level = 1;
    state->sun = 50;
    state->plant_count = 0;
    state->wave_number = 1;
    state->is_day = true;
}

// 打印游戏状态（用于测试）
void print_state(const GameState* state) {
    printf("\n=== 游戏状态 ===");
    printf("\n关卡: %d", state->level);
    printf("\n阳光: %d", state->sun);
    printf("\n植物数量: %d", state->plant_count);
    printf("\n当前波次: %d", state->wave_number);
    printf("\n模式: %s", state->is_day ? "白天" : "黑夜");
}

int main() {
    GameState current_state;
    int choice;
    char filename[256];

    // 初始化新游戏
    new_game(&current_state);

    do {
        show_save_menu();
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("\n输入存档文件名 (例如: save1.pvz): ");
            scanf_s("%255s", filename);
            if (save_game(filename, &current_state)) {
                printf("游戏保存成功！\n");
            }
            else {
                printf("保存失败！\n");
            }
            break;

        case 2:
            printf("\n输入要加载的存档文件名: ");
            scanf_s("%255s", filename);
            if (load_game(filename, &current_state)) {
                printf("游戏加载成功！\n");
                print_state(&current_state);
            }
            else {
                printf("加载失败或文件损坏！\n");
            }
            break;
        }
    } while (choice != 3);

    return 0;
}