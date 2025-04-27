#ifndef LEVEL_SYSTEM_H
#define LEVEL_SYSTEM_H

#include <time.h>

#define MAX_WAVES 20        // 最大波次数量
#define MAX_ENEMY_TYPES 10  // 最大敌人种类
#define MAX_LEVELS 50       // 最大关卡数量

typedef struct {
    int type;       // 敌人类型
    int count;      // 生成数量
    float interval; // 生成间隔（秒）
} Wave;

typedef struct {
    int levelId;            // 关卡ID
    Wave waves[MAX_WAVES];  // 波次配置
    int waveCount;          // 总波次数
    int unlockCondition;    // 解锁条件（需要的前置关卡）
    int isCompleted;        // 是否已完成
} LevelData;

typedef struct {
    LevelData levels[MAX_LEVELS]; // 所有关卡数据
    int totalLevels;        // 总关卡数
    int currentLevel;       // 当前进行中的关卡
    int currentWave;        // 当前波次
    time_t lastSpawnTime;   // 上次生成时间
    int remainingEnemies;   // 剩余敌人数量
} LevelSystem;

// 系统初始化
LevelSystem* level_init();
void level_destroy(LevelSystem* system);

// 关卡管理
void level_load_data(LevelSystem* system, const LevelData* data, int count);
int level_unlock_check(const LevelSystem* system, int levelId);
int level_start(LevelSystem* system, int levelId);
void level_complete(LevelSystem* system);

// 游戏进程
int level_update(LevelSystem* system); // 返回剩余敌人数量
int level_is_wave_cleared(const LevelSystem* system);
int level_is_completed(const LevelSystem* system);

// 数据获取
const LevelData* level_get_current(const LevelSystem* system);
int level_get_progress(const LevelSystem* system);

#endif
