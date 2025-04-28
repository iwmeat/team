#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
// ���ֲ�����������浵����
#define MAX_PLANTS 50
#define MAX_SLOTS 3

// ֲ������ö��
typedef enum {
    PEASHOOTER,
    SUNFLOWER,
    WALLNUT,
    CHERRYBOMB,
    NONE
} PlantType;

// ֲ��ṹ��
typedef struct {
    PlantType type;
    int x, y;       // λ������
    int health;     // ����ֵ
    int cooldown;   // ��ȴʱ��
} Plant;

// ��Ϸ״̬�ṹ��
typedef struct {
    int level;          // ��ǰ�ؿ�
    int sun;            // ��������
    Plant plants[MAX_PLANTS]; // �ѷ��õ�ֲ��
    int plant_count;    // ��ǰֲ������
    int wave_number;    // ��ʬ����
    bool is_day;        // ����/��ҹģʽ
} GameState;

// �ļ�ͷ��ʶ�Ͱ汾��
const char FILE_HEADER[4] = { 'P', 'V', 'Z', 0x1A };
const int VERSION = 1;

// ������Ϸ״̬���ļ�
bool save_game(const char* filename, const GameState* state) {
    FILE* file= fopen(filename, "wb");
    if (!file) return false;

    // д���ļ�ͷ�Ͱ汾
    fwrite(FILE_HEADER, sizeof(char), 4, file);
    fwrite(&VERSION, sizeof(int), 1, file);

    // д����Ϸ����
    fwrite(state, sizeof(GameState), 1, file);

    fclose(file);
    return true;
}

// ���ļ�������Ϸ״̬
bool load_game(const char* filename, GameState* state) {
    FILE* file = fopen(filename, "rb");
    if (!file) return false;

    // ��֤�ļ�ͷ
    char header[4];
    int file_version;
    fread(header, sizeof(char), 4, file);
    fread(&file_version, sizeof(int), 1, file);

    if (memcmp(header, FILE_HEADER, 4) != 0 || file_version != VERSION) {
        fclose(file);
        return false;
    }

    // ��ȡ��Ϸ����
    fread(state, sizeof(GameState), 1, file);

    fclose(file);
    return true;
}

// ��ʾ�浵�˵�
void show_save_menu() {
    printf("\n=== �浵ϵͳ ===");
    printf("\n1. �������");
    printf("\n2. ���ؽ���");
    printf("\n3. ������Ϸ");
    printf("\nѡ�����: ");
}

// ʾ������ʼ������Ϸ
void new_game(GameState* state) {
    state->level = 1;
    state->sun = 50;
    state->plant_count = 0;
    state->wave_number = 1;
    state->is_day = true;
}

// ��ӡ��Ϸ״̬�����ڲ��ԣ�
void print_state(const GameState* state) {
    printf("\n=== ��Ϸ״̬ ===");
    printf("\n�ؿ�: %d", state->level);
    printf("\n����: %d", state->sun);
    printf("\nֲ������: %d", state->plant_count);
    printf("\n��ǰ����: %d", state->wave_number);
    printf("\nģʽ: %s", state->is_day ? "����" : "��ҹ");
}

int main() {
    GameState current_state;
    int choice;
    char filename[256];

    // ��ʼ������Ϸ
    new_game(&current_state);

    do {
        show_save_menu();
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            printf("\n����浵�ļ��� (����: save1.pvz): ");
            scanf_s("%255s", filename);
            if (save_game(filename, &current_state)) {
                printf("��Ϸ����ɹ���\n");
            }
            else {
                printf("����ʧ�ܣ�\n");
            }
            break;

        case 2:
            printf("\n����Ҫ���صĴ浵�ļ���: ");
            scanf_s("%255s", filename);
            if (load_game(filename, &current_state)) {
                printf("��Ϸ���سɹ���\n");
                print_state(&current_state);
            }
            else {
                printf("����ʧ�ܻ��ļ��𻵣�\n");
            }
            break;
        }
    } while (choice != 3);

    return 0;
}