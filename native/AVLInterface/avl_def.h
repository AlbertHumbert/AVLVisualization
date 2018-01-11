//
// Created by 林嘉民 on 17/12/10.
//

/**
 * 该头文件定义该二叉树实现的所有结构体和所有别名
 */

#ifndef AVL_AVL_DEF_H
#define AVL_AVL_DEF_H

#define OK  1
#define ERROR 0

//平衡因子
#define Factor int
#define LH 1
#define EH 0
#define RH -1

#define KeyType long
#define ElemType char

//节点结构体
typedef struct AVLNode {
    KeyType key;
    ElemType *data;
    Factor bf;
    struct AVLNode *lchild, *rchild;
} AVLNode;

//树结构体
typedef struct AVLTree {
    AVLNode *root;
    long size;
} AVLTree;


#endif //AVL_AVL_DEF_H
