#pragma once
#include "../code/catan.h"

#define FRONT 0
#define BACK 1

typedef struct _sList_* pList;
typedef struct _sList_ {
    size_t index;      // --索引 (HEAD: SIZE)
    pList PREV, NEXT;  // --前一個、下一個
} sList;

// *.被用功能：整理 List 的 index
static inline void setIndex(pList head) {
    pList node = head->NEXT;
    for (size_t idx = 0; idx < head->index; idx++)
        node->index = idx, node = node->NEXT;
    return;
}

// 1. initList: 初始化 List Head
static inline pList initList(void) {
    pList head = (pList)malloc(sizeof(sList));
    head->index = 0;                 // List Head 的初始化長度為 0 （沒有任何元素）
    head->PREV = head->NEXT = head;  // List Head 的前一個、下一個都指向自己
    return head;
}

// 2. entry: 透過 List 元素得到其所在的母體 struct entry
#define container(node, type, member) \
    (type*)((char*)node - ((size_t) & ((type*)0)->member))
#define entry(node, type) \
    container(node, type, list)

// 3. push: 新增元素到 List Head 的最前面、最後面
static inline void push(pList head, pList node, int mode) {
    ++head->index;  // List Head 的長度 +1
    if (mode == FRONT) {
        node->PREV = head, node->NEXT = head->NEXT;  // 設定新增元素的前一個、下一個
        head->NEXT->PREV = node, head->NEXT = node;  // List Head 的下一個指向新增元素
        setIndex(head);                              // 整理 List 的 index
    } else if (mode == BACK) {
        node->PREV = head->PREV, node->NEXT = head;  // 設定新增元素的前一個、下一個
        head->PREV->NEXT = node, head->PREV = node;  // List Head 的前一個指向新增元素
        node->index = head->index - 1;               // 新增元素的 index 為 List Head 的長度 - 1
    }
}

// 4. pop: 從 List Head 的最前面、最後面移除元素
static inline pList pop(pList head, int mode) {
    pList node = NULL;
    if (head->index == 0) return NULL;  // List Head 的長度為 0，無法移除元素

    --head->index;  // List Head 的長度 -1
    if (mode == FRONT) {
        node = head->NEXT;                                 // 設定要移除的元素
        head->NEXT = node->NEXT, node->NEXT->PREV = head;  // List Head 的下一個指向要移除元素的下一個
        setIndex(head);                                    // 整理 List 的 index
    } else if (mode == BACK) {
        node = head->PREV;                                 // 設定要移除的元素
        head->PREV = node->PREV, node->PREV->NEXT = head;  // List Head 的前一個指向要移除元素的前一個
    }
    return node;
}

// 5. get: 從 List Head 的最前面、最後面取得元素
static inline pList get(pList head, int mode) {
    if (head->index == 0) return NULL;  // List Head 的長度為 0，無法取得元素
    if (mode == FRONT) return head->NEXT;
    if (mode == BACK) return head->PREV;
    return NULL;
}

// 6. getNode: 從 List Head 取得第 index 個元素
static inline pList getNode(pList head, size_t index) {
    if (index >= head->index) return NULL;  // index 超過 List Head 的長度，無法取得元素

    pList tmp = head->NEXT;
    for (size_t idx = 0; idx < index; idx++) tmp = tmp->NEXT;  // 找到要取得的位置
    return tmp;
}

// 7. insNode: 在 List Head 插入成為第 index 個元素
static inline void insNode(pList head, pList node, size_t index) {
    if (index > head->index) return;  // index 超過 List Head 的長度，無法插入元素

    ++head->index;  // List Head 的長度 +1
    pList tmp = head->NEXT;
    for (size_t idx = 0; idx < index; idx++) tmp = tmp->NEXT;  // 找到要插入的位置
    node->PREV = tmp->PREV, node->NEXT = tmp;                  // 設定新增元素的前一個、下一個
    tmp->PREV->NEXT = node, tmp->PREV = node;                  // 設定要插入位置的前一個、下一個
    setIndex(head);                                            // 整理 List 的 index
    return;
}

// 8. rmNode: 從 List Head 移除第 index 個元素
static inline pList rmNode(pList head, size_t index) {
    if (index >= head->index) return NULL;  // index 超過 List Head 的長度，無法移除元素

    --head->index;  // List Head 的長度 -1
    pList tmp = head->NEXT;
    for (size_t idx = 0; idx < index; idx++) tmp = tmp->NEXT;  // 找到要移除的位置
    tmp->PREV->NEXT = tmp->NEXT, tmp->NEXT->PREV = tmp->PREV;  // 設定要移除位置的前一個、下一個
    setIndex(head);                                            // 整理 List 的 index
    return tmp;
}

// 9. forList: 透過 List Head 進行迴圈      * node 可以不用宣告，直接選擇要使用的變數名稱 *
#define forList(head, node) \
    for (pList node = head->NEXT; node != head; node = node->NEXT)
#define forList_R(head, node) \
    for (pList node = head->PREV; node != head; node = node->PREV)

// 10. shuffle: 透過 List Head 進行洗牌
static inline void shuffle(pList head) {
    if (head->index <= 1) return;  // List Head 的長度小於等於 1，無法進行洗牌

    pList tmp = head->NEXT;
    for (size_t idx = 0; idx < head->index; idx++) {
        size_t index = rand() % head->index;             // 隨機選擇一個位置
        pList node = getNode(head, index);               // 取得該位置的元素
        tmp->PREV->NEXT = node, node->PREV = tmp->PREV;  // 設定要交換位置的前一個、下一個
        tmp->NEXT->PREV = node, node->NEXT = tmp->NEXT;  // 設定要交換位置的前一個、下一個
        tmp->PREV = node, tmp->NEXT = node->NEXT;        // 設定要交換位置的前一個、下一個
        node->NEXT->PREV = tmp, node->NEXT = tmp;        // 設定要交換位置的前一個、下一個
        tmp = tmp->NEXT;                                 // 移動到下一個位置
    }
    return;
}
