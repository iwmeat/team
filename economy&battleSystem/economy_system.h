#ifndef ECONOMY_SYSTEM_H
#define ECONOMY_SYSTEM_H

#include <time.h>

#define MAX_ITEMS 20    // 商店最大商品种类
#define SUN_INTERVAL 5  // 自然阳光生成间隔（秒）

typedef struct {
    int id;             // 商品ID
    char name[32];      // 商品名称
    int price;          // 商品价格
    int stock;          // 库存量（-1表示无限）
} ShopItem;

typedef struct {
    int sunlight;               // 当前阳光值
    int autoGeneration;         // 是否启用自动生成
    time_t lastSunGenTime;      // 上次自然生成时间
    ShopItem shopItems[MAX_ITEMS]; // 商店商品列表
    int itemCount;              // 实际商品种类数
} EconomySystem;

// 系统初始化
EconomySystem* economy_init();
void economy_destroy(EconomySystem* system);

// 阳光管理
void economy_add_sunlight(EconomySystem* system, int amount);
int economy_get_sunlight(const EconomySystem* system);
int economy_consume_sunlight(EconomySystem* system, int amount);

// 商店管理
void economy_add_shop_item(EconomySystem* system, ShopItem item);
const ShopItem* economy_find_item(const EconomySystem* system, int itemId);
int economy_purchase_item(EconomySystem* system, int itemId);

// 系统更新
void economy_update(EconomySystem* system);

#endif
