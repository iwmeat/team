#include "level_system.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

LevelSystem* level_init() {
    LevelSystem* system = malloc(sizeof(LevelSystem));
    memset(system, 0, sizeof(LevelSystem));
    
    // 初始化基础关卡数据
    LevelData defaultLevels[] = {
        {1, {{1,5,1.5f}, {2,3,2.0f}}, 2, 0, 0}, // 关卡1
        {2, {{1,8,1.0f}, {2,5,1.5f}, {3,2,3.0f}}, 3, 1, 0} // 关卡2
    };
    
    level_load_data(system, defaultLevels, sizeof(defaultLevels)/sizeof(LevelData));
    return system;
}

void level_destroy(LevelSystem* system) {
    free(system);
}

void level_load_data(LevelSystem* system, const LevelData* data, int count) {
    system->totalLevels = (count < MAX_LEVELS) ? count : MAX_LEVELS;
    memcpy(system->levels, data, sizeof(LevelData)*system->totalLevels);
}

int level_unlock_check(const LevelSystem* system, int levelId) {
    if (levelId < 1 || levelId > system->totalLevels) return 0;
    
    const LevelData* level = &system->levels[levelId-1];
    if (levelId == 1) return 1; // 第一关默认解锁
    
    // 检查前置关卡是否完成
    for (int i = 0; i < system->totalLevels; i++) {
        if (system->levels[i].levelId == level->unlockCondition) {
            return system->levels[i].isCompleted;
        }
    }
    return 0;
}

int level_start(LevelSystem* system, int levelId) {
    if (!level_unlock_check(system, levelId)) return 0;
    
    for (int i = 0; i < system->totalLevels; i++) {
        if (system->levels[i].levelId == levelId) {
            system->currentLevel = levelId;
            system->currentWave = 0;
            system->remainingEnemies = 0;
            system->lastSpawnTime = time(NULL);
            return 1;
        }
    }
    return 0;
}

int level_update(LevelSystem* system) {
    const LevelData* current = level_get_current(system);
    if (!current || system->currentWave >= current->waveCount) return 0;
    
    Wave* wave = &current->waves[system->currentWave];
    
    // 生成敌人逻辑
    if (wave->count > 0) {
        time_t now = time(NULL);
        if (difftime(now, system->lastSpawnTime) >= wave->interval) {
            system->remainingEnemies++;
            wave->count--;
            system->lastSpawnTime = now;
            
            // 这里应该触发敌人生成事件，返回敌人类型
            return wave->type;
        }
    }
    
    // 波次完成检测
    if (wave->count <= 0 && system->remainingEnemies <= 0) {
        system->currentWave++;
        if (system->currentWave < current->waveCount) {
            system->remainingEnemies = current->waves[system->currentWave].count;
        }
    }
    
    return -1; // 无新敌人生成
}

void level_complete(LevelSystem* system) {
    for (int i = 0; i < system->totalLevels; i++) {
        if (system->levels[i].levelId == system->currentLevel) {
            system->levels[i].isCompleted = 1;
            break;
        }
    }
    system->currentLevel = 0;
}

const LevelData* level_get_current(const LevelSystem* system) {
    for (int i = 0; i < system->totalLevels; i++) {
        if (system->levels[i].levelId == system->currentLevel) {
            return &system->levels[i];
        }
    }
    return NULL;
}

int level_get_progress(const LevelSystem* system) {
    return system->currentLevel * 100 + system->currentWave;
}

int level_is_wave_cleared(const LevelSystem* system) {
    return (system->currentWave >= level_get_current(system)->waveCount);
}

int level_is_completed(const LevelSystem* system) {
    return level_is_wave_cleared(system) && (system->remainingEnemies == 0);
}
