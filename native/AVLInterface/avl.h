//
// Created by 林嘉民 on 17/12/10.
//

/**
 * 该头文件定义该平衡二叉树实现的所有的接口函数
 */

#include "avl_def.h"

#ifndef AVL_AVL_H
#define AVL_AVL_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * 创建AVL树
 * @return 成功返回树指针，失败返回 NULL
 */
AVLTree *newAVLTree();

/**
 * 销毁AVL树
 * @param avlTree
 * @return 成功返回 1 ，失败返回 0
 */
int releaseAVLTree(AVLTree *avlTree);

/**
 *
 * @param avlTree
 */
void printAVLTree(AVLTree *avlTree);

/**
 * 获取AVL树的高度
 * @param avlTree
 * @return 成功返回AVL树的高度，为空或失败返回 0
 */
int getAVLTreeHeight(AVLTree *avlTree);

/**
 * 获取AVL树的大小
 * @param avlTree
 * @return 成功返回节点个数，为空或失败返回 0
 */
long getAVLTreeSize(AVLTree *avlTree);

/**
 * 检查AVL树是否为空
 * @param avlTree
 * @return 为空返回 1 ，不为空或失败返回 0；
 */
int isAVLTreeEmpty(AVLTree *avlTree);

/**
 * 向AVL树插入数据
 * @param avlTree
 * @param key
 * @param data
 * @return 成功返回 1，失败返回 0
 */
int insertAVLData(AVLTree *avlTree, KeyType key, void *data);

/**
 * 移除AVL树中指定键的数据
 * @param avlTree
 * @param key
 * @return 成功返回 1 失败返回 0
 */
int removeAVLData(AVLTree *avlTree, KeyType key);


/**
 * 查找指定键的节点
 * @param avlTree
 * @param key
 * @return 成功返回节点指针，失败返回 NULL
 */
AVLNode *findAVLData(AVLTree *avlTree, KeyType key);

void visitAVLTree(AVLTree *avlTree, void (*fun)(AVLNode *node));


/**
 * 合并两颗AVL树
 * @param tree1
 * @param tree2
 * @param result
 */
void mergeAVLTree(AVLTree *tree[2], AVLTree **result);

/**
 * 分裂两颗AVL树
 * @param avlTree
 * @param avlTrees
 */
void splitAVLTree(AVLTree *avlTree, KeyType key, AVLTree *avlTrees[2]);


void printAVLNode(AVLNode *node);

#ifdef __cplusplus
}
#endif

#endif //AVL_AVL_H