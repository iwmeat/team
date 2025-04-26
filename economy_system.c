#include "economy_system.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

EconomySystem* economy_init() {
    EconomySystem* system = malloc(sizeof(EconomySystem));
    memset(system, 0, sizeof(EconomySystem));
    
    system->sunlight = 50;      // 初始阳光值
    system->autoGeneration = 1; // 启用自动生成
    system->lastSunGenTime = time(NULL);
    system->itemCount = 0;
    
    // 初始化默认商品
    economy_add_shop_item(system, (ShopItem){1, "Sunflower", 50, -1});
    economy_add_shop_item(system, (ShopItem){2, "Peashooter", 100, -1});
    economy_add_shop_item(system, (ShopItem){3, "Wall-nut", 50, 3});
    
    return system;
}

void economy_destroy(EconomySystem* system) {
    free(system);
}

void economy_add_sunlight(EconomySystem* system, int amount) {
    if (amount > 0) {
        system->sunlight += amount;
    }
}

int economy_get_sunlight(const EconomySystem* system) {
    return system->sunlight;
}

int economy_consume_sunlight(EconomySystem* system, int amount) {
    if (amount <= 0 || system->sunlight < amount) {
        return 0; // 消费失败
    }
    system->sunlight -= amount;
    return 1;     // 消费成功
}

void economy_add_shop_item(EconomySystem* system, ShopItem item) {
    if (system->itemCount < MAX_ITEMS) {
        system->shopItems[system->itemCount++] = item;
    }
}

const ShopItem* economy_find_item(const EconomySystem* system, int itemId) {
    for (int i = 0; i < system->itemCount; i++) {
        if (system->shopItems[i].id == itemId) {
            return &system->shopItems[i];
        }
    }
    return NULL;
}

int economy_purchase_item(EconomySystem* system, int itemId) {
    const ShopItem* item = economy_find_item(system, itemId);
    if (!item) return 0; // 商品不存在
    
    // 检查库存
    if (item->stock != -1 && item->stock <= 0) {
        return 0; // 库存不足
    }
    
    // 检查阳光是否足够
    if (!economy_consume_sunlight(system, item->price)) {
        return 0; // 阳光不足
    }
    
    // 更新库存
    if (item->stock != -1) {
        for (int i = 0; i < system->itemCount; i++) {
            if (system->shopItems[i].id == itemId) {
                system->shopItems[i].stock--;
                break;
            }
        }
    }
    
    return 1; // 购买成功
}

void economy_update(EconomySystem* system) {
    if (!system->autoGeneration) return;
    
    time_t current = time(NULL);
    if (difftime(current, system->lastSunGenTime) >= SUN_INTERVAL) {
        system->sunlight += 25; // 自动生成25阳光
        system->lastSunGenTime = current;
    }
}
