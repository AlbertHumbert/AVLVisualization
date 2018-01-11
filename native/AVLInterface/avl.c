//
// Created by 林嘉民 on 17/12/10.
//
/**
 * AVL树接口具体实现
 */

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <printf.h>
#include <stdbool.h>
#include "avl.h"


AVLNode *newAVLNode(KeyType key, void *data);

int insertAndAdjust(AVLNode **nodeP, KeyType key, void *data, int *taller);

void adjustAfterInsertL(AVLNode **nodeP, int *taller);

void adjustAfterInsertR(AVLNode **nodeP, int *taller);

int removeAndAdjust(AVLNode **nodeP, int key, int *shorter);

int adjustAfterRemoveR(AVLNode **nodeP, int *shorter);

int adjustAfterRemoveL(AVLNode **nodeP, int *shorter);

void replaceNodeWithPrecursor(AVLNode **lchild, AVLNode *toReplace, int *shorter);

void rotateLR(AVLNode **node);

void rotateRL(AVLNode **node);

void simpleRotateL(AVLNode **nodeP);

void simpleRotateR(AVLNode **nodeP);


/*
 * 创建AVL树
 */
AVLTree *newAVLTree() {

    AVLTree *avlTree;

    if ((avlTree = (AVLTree *) malloc(sizeof(AVLTree))) != NULL) {
        avlTree->root = NULL;
        avlTree->size = 0;
    }

    return avlTree;
}

//递归销毁二叉树节点
void releaseAVLTreeRecursively(AVLNode *avlNode) {

    if (avlNode) {
        releaseAVLTreeRecursively(avlNode->lchild);
        releaseAVLTreeRecursively(avlNode->rchild);
        free(avlNode);
    }
}


/*
 * 销毁AVL树
 */
int releaseAVLTree(AVLTree *avlTree) {
    if (avlTree == NULL)return ERROR;
    releaseAVLTreeRecursively(avlTree->root);
    free(avlTree);
    return OK;
}

//递归访问二叉树节点，并调用函数指针指向的函数
void visitAVLTreeRecursively(AVLNode *avlNode, void (*fun)(AVLNode *node)) {
    if (avlNode) {
        if (avlNode->lchild) visitAVLTreeRecursively(avlNode->lchild, fun);
        fun(avlNode);
        if (avlNode->rchild)visitAVLTreeRecursively(avlNode->rchild, fun);
    }
}

/*
 * 遍历AVL树
 */
void visitAVLTree(AVLTree *avlTree, void (*fun)(AVLNode *node)) {

    visitAVLTreeRecursively(avlTree->root, fun);

}

//递归打印二叉树
void printNodeRecursively(AVLNode *node, int depth) {

    if (node) {

        printNodeRecursively(node->rchild, depth + 1);
        for (int i = 0; i < depth; i++) {
            printf("   ");
        }
        printf(" * (key : %ld  bf : %d dp : %d)\n", node->key, node->bf, depth);
        printNodeRecursively(node->lchild, depth + 1);
    }
}

/*
 * 使用凹入表打印平衡二叉树
 */
void printAVLTree(AVLTree *avlTree) {

    if (!avlTree->root)printf("Empty Tree\n");
    else printNodeRecursively(avlTree->root, 0);

}


//递归获取平衡二叉树高度
int getAVLTreeHeightRecursively(AVLNode *node) {

    int rDepth;
    int lDepth;
    if (!node)return 0;

    lDepth = getAVLTreeHeightRecursively(node->lchild);
    rDepth = getAVLTreeHeightRecursively(node->rchild);

    return lDepth > rDepth ? lDepth + 1 : rDepth + 1;


}

/*
 * 获取AVL树的高度
 */
int getAVLTreeHeight(AVLTree *avlTree) {
    if (avlTree == NULL)return ERROR;
    return getAVLTreeHeightRecursively(avlTree->root);;
}

/*
 * 获取AVL树的大小
 */
long getAVLTreeSize(AVLTree *avlTree) {
    if (avlTree == NULL)return ERROR;
    return avlTree->size;
}

/*
 * 检查AVL树是否为空
 */
int isAVLTreeEmpty(AVLTree *avlTree) {
    if (avlTree == NULL)return ERROR;
    return OK;
}


/*
 * 查找指定键的节点
 */

AVLNode *findAVLData(AVLTree *avlTree, KeyType key) {
    if (avlTree == NULL)return NULL;

    AVLNode *node = avlTree->root;

    while (node) {
        if (key < node->key)node = node->lchild;
        else if (key > node->key)node = node->rchild;
        else break;
    }

    return node;
}


/*
 * 移除AVL树中指定键的数据
 */
int removeAVLData(AVLTree *avlTree, KeyType key) {

    int shorter = 0;

    //移除失败？
    if (avlTree == NULL || !removeAndAdjust(&avlTree->root, key, &shorter))
        return ERROR;

    avlTree->size--;
    return OK;
}

//销毁当前节点并把前驱移到被销毁节点的位置
void replaceNodeWithPrecursor(AVLNode **lchild, AVLNode *toReplace, int *shorter) {

    AVLNode *rchlid = *lchild;
    if (!rchlid->rchild) {
        toReplace->key = rchlid->key;
        toReplace->data = rchlid->data;

        *lchild = rchlid->lchild;
        free(rchlid);
        *shorter = true;
    } else {
        replaceNodeWithPrecursor(&rchlid->rchild, toReplace, shorter);
        if (*shorter)adjustAfterRemoveR(lchild, shorter);
    }
}

//寻找移除节点并且根据平衡因子判断是否需要平衡处理
int removeAndAdjust(AVLNode **nodeP, int key, int *shorter) {

    //到达叶子节点，key不存在
    if (!*nodeP) return ERROR;

    //查得
    if (key == (*nodeP)->key) {

        if (!(*nodeP)->lchild) {
            (*nodeP) = (*nodeP)->rchild;
            *shorter = true;
        } else if (!(*nodeP)->rchild) {
            (*nodeP) = (*nodeP)->lchild;
            *shorter = true;
        } else {
            replaceNodeWithPrecursor(&(*nodeP)->lchild, *nodeP, shorter);
            if (*shorter == true)adjustAfterRemoveL(nodeP, shorter);

        }

        //小于
    } else if (key < (*nodeP)->key) {

        if (!removeAndAdjust(&(*nodeP)->lchild, key, shorter))return ERROR;
        if (*shorter) adjustAfterRemoveL(nodeP, shorter);
        //大于
    } else {

        if (!removeAndAdjust(&(*nodeP)->rchild, key, shorter))return ERROR;
        if (*shorter) adjustAfterRemoveR(nodeP, shorter);
    }
    return true;
}


//处理右子树上移除节点导致的失衡
int adjustAfterRemoveR(AVLNode **nodeP, int *shorter) {

    AVLNode *node = *nodeP;
    AVLNode *lchild;
    AVLNode *lrchlid;

    if (node->bf == RH) {
        node->bf = EH;
        *shorter = true;
    } else if (node->bf == EH) {
        node->bf = LH;
        *shorter = false;
    } else {
        lchild = node->lchild;
        lrchlid = lchild->rchild;

        //相当于LL失衡，简单右旋
        if (lchild->bf == LH) {
            node->bf = lchild->bf = EH;
            simpleRotateR(nodeP);
            *shorter = true;

            //相当于LL失衡，简单右旋
        } else if (lchild->bf == EH) {
            node->bf = LH;
            lchild->bf = RH;
            simpleRotateR(nodeP);
            *shorter = false;

            //LR型失衡
            //需要根据左节点的右孩子的平衡因子修改被改动节点的平衡因子
        } else {

            if (lrchlid->bf == LH) {
                node->bf = RH;
                lchild->bf = EH;
                lrchlid->bf = EH;

            } else if (lrchlid->bf == RH) {
                node->bf = EH;
                lchild->bf = LH;
                lrchlid->bf = EH;

            } else if (lrchlid->bf == EH) {
                node->bf = lchild->bf = lrchlid->bf = EH;
            }

            printf("rm R");
            rotateLR(nodeP);
            //由于原先是失衡状态
            //因此旋转过后高度必然缩小
            *shorter = true;

        }

    }
    return false;
}

//处理左子树上移除节点导致的失衡
int adjustAfterRemoveL(AVLNode **nodeP, int *shorter) {


    AVLNode *node = *nodeP;
    AVLNode *rchild;
    AVLNode *rlchlid;

    //左高，移除后等高，不用调整
    if (node->bf == LH) {
        node->bf = EH;
        *shorter = true;

        //等高，不用调整
    } else if (node->bf == EH) {
        node->bf = RH;
        *shorter = false;
    } else {
        rchild = node->rchild;
        rlchlid = rchild->lchild;

        //相当于RR失衡，简单左旋
        if (rchild->bf == RH) {
            node->bf = rchild->bf = EH;
            simpleRotateL(nodeP);
            *shorter = true;

            //相当于RR失衡，简单左旋
        } else if (rchild->bf == EH) {
            node->bf = RH;
            rchild->bf = LH;
            simpleRotateL(nodeP);
            *shorter = false;

        } else {

            //RL型失衡
            //需要根据左节点的右孩子的平衡因子修改被改动节点的平衡因子

            if (rlchlid->bf == LH) {
                node->bf = EH;
                rchild->bf = RH;
                rlchlid->bf = EH;
            } else if (rlchlid->bf == RH) {
                node->bf = LH;
                rchild->bf = EH;
                rlchlid->bf = EH;

            } else if (rlchlid->bf == EH) {
                node->bf = rchild->bf = rlchlid->bf = EH;
            }

            rotateRL(nodeP);
            //由于原先是失衡状态
            //因此旋转过后高度必然缩小
            *shorter = true;

        }

    }
    return false;
}


/*
 * 向AVL树插入数据
 */
int insertAVLData(AVLTree *avlTree, KeyType key, void *data) {

    int taller; //标记子树高度是否增加

    //插入失败？
    if (avlTree == NULL || !insertAndAdjust(&avlTree->root, key, data, &taller))
        return ERROR;

    avlTree->size++;
    return OK;
}

/*
 * 寻找插入点插入并且根据平衡因子判断是需要处理失衡
 */
int insertAndAdjust(AVLNode **nodeP, KeyType key, void *data, int *taller) {

    //到达叶子节点
    if (!*nodeP) {
        if ((*nodeP = newAVLNode(key, data)) == NULL)return ERROR;
        (*taller) = true;
        return true;
    }
        //重复
    else if (key == (*nodeP)->key) {
        (*taller) = false;
        return ERROR;

        //小于
    } else if (key < (*nodeP)->key) {

        if (!insertAndAdjust(&(*nodeP)->lchild, key, data, taller))return ERROR;
        if (*taller)adjustAfterInsertL(nodeP, taller);

        //大于
    } else {

        if (!insertAndAdjust(&(*nodeP)->rchild, key, data, taller))return ERROR;
        if (*taller)adjustAfterInsertR(nodeP, taller);
    }

    return true;
}

/*
 * 处理在左子树上插入导致的失衡
 */
void adjustAfterInsertL(AVLNode **nodeP, int *taller) {

    AVLNode *node = *nodeP;
    AVLNode *lrchild;
    AVLNode *lchild;

    //左右子树等高，不用调整
    if (node->bf == EH) {
        node->bf = LH;
        *taller = true;

        //右子树高，插入后等高，不用调整
    } else if (node->bf == RH) {
        node->bf = EH;
        *taller = false;

        //左子树高，插入后失衡，进行调整
    } else if (node->bf == LH) {

        lchild = node->lchild;
        lrchild = lchild->rchild;

        //LL型失衡调整，简单右旋
        if (lchild->bf == LH) {
            node->bf = lchild->bf = EH;
            simpleRotateR(nodeP);

            //LR型失衡调整
        } else if (lchild->bf == RH) {

            if (lrchild->bf == LH) {
                node->bf = RH;
                lchild->bf = EH;

            } else if (lrchild->bf == RH) {
                node->bf = EH;
                lchild->bf = LH;

            } else if (lrchild->bf == EH) {
                node->bf = lchild->bf = EH;
                printf("adjust when lrchild == EH\n");

            }

            lrchild->bf = EH;
            rotateLR(nodeP);
        }

        *taller = false;

        //若左子树的左右子树高度相等，则不应该调整
    } else printf("error bf");

}

/*
 * 处理在右子树上插入导致的失衡
 */
void adjustAfterInsertR(AVLNode **nodeP, int *taller) {

    AVLNode *node = *nodeP;
    AVLNode *rchild;
    AVLNode *rlchild;

    //左右子树等高，不用调整
    if (node->bf == EH) {
        node->bf = RH;
        *taller = true;

        //左子树高，插入后平衡，不用调整
    } else if (node->bf == LH) {
        node->bf = EH;
        *taller = false;

        //右子树高，插入后失衡，进行调整
    } else if (node->bf == RH) {
        rchild = node->rchild;
        rlchild = rchild->lchild;

        //RR型失衡，简单左旋
        if (rchild->bf == RH) {
            node->bf = rchild->bf = EH;
            simpleRotateL(nodeP);


            //RL型失衡，先右旋再左旋
        } else if (rchild->bf == LH) {

            if (rlchild->bf == EH) {
                node->bf = rchild->bf = EH;

            } else if (rlchild->bf == LH) {
                node->bf = EH;
                rchild->bf = RH;

            } else if (rlchild->bf == RH) {
                node->bf = LH;
                rchild->bf = EH;
            } else printf("bf error");

            rlchild->bf = EH;
            rotateRL(nodeP);
        }

        *taller = false;

    } else printf("bf error");

}


/*
 * LR型失衡旋转,先左旋再右旋
 */
void rotateLR(AVLNode **node) {
    simpleRotateL(&(*node)->lchild);
    simpleRotateR(node);
}

/*
 * RL型失衡旋转,先右旋再左旋
 */
void rotateRL(AVLNode **node) {
    simpleRotateR(&(*node)->rchild);
    simpleRotateL(node);
}

/*
 * 简单右旋
 */
void simpleRotateR(AVLNode **nodeP) {

    AVLNode *parent = *nodeP;
    AVLNode *lchild = parent->lchild;
    parent->lchild = lchild->rchild;
    lchild->rchild = parent;
    *nodeP = lchild;

}

/*
 * 简单左旋
 */
void simpleRotateL(AVLNode **nodeP) {
    AVLNode *parent = *nodeP;
    AVLNode *rchlid = parent->rchild;
    parent->rchild = rchlid->lchild;
    rchlid->lchild = parent;
    *nodeP = rchlid;

}

/*
 * 创建新节点
 */
AVLNode *newAVLNode(KeyType key, void *data) {
    AVLNode *node = (AVLNode *) malloc(sizeof(AVLNode));
    node->key = key;
    node->data = data;
    node->lchild = NULL;
    node->rchild = NULL;
    node->bf = false;
    return node;
}


//重新调整节点的平衡因子
int resetBF(AVLNode *node) {

    int rDepth;
    int lDepth;
    int depth;
    if (!node)return 0;

    lDepth = resetBF(node->lchild);
    rDepth = resetBF(node->rchild);

    if (lDepth > rDepth) {
        depth = lDepth + 1;
        node->bf = LH;

    } else if (lDepth < rDepth) {
        depth = rDepth + 1;
        node->bf = RH;
    } else {
        depth = rDepth + 1;
        node->bf = EH;
    }

    return depth;

}

//添加节点
void addTOArray(AVLNode **array, AVLNode *avlNode, long index) {
    array[index] = avlNode;
}

//递归遍历树并添加到数组中
void addAVLTreeRecursively(AVLNode **array, AVLNode *avlNode, long *index) {
    if (avlNode) {
        if (avlNode->lchild)addAVLTreeRecursively(array, avlNode->lchild, index);
        addTOArray(array, avlNode, *index);
        (*index)++;
        if (avlNode->rchild)addAVLTreeRecursively(array, avlNode->rchild, index);
    }
}

//将有序数组转化为平衡二叉树
AVLNode *sortedArrayToAVL(AVLNode **array, long start, long end) {

    AVLNode *rootNode = NULL;

    if (start <= end) {
        long mid = (start + end) / 2;
        rootNode = array[mid];
        rootNode->lchild = sortedArrayToAVL(array, start, mid - 1);
        rootNode->rchild = sortedArrayToAVL(array, mid + 1, end);
    }
    return rootNode;
}

//打印节点
void printAVLNode(AVLNode *node) {
    printf(" * (key : %ld  bf : %d )\n", node->key, node->bf);
}


/*
 * 合并两棵平衡二叉树
 */
void mergeAVLTree(AVLTree *tree[2], AVLTree **result) {

    long index = 0;
    long mid = tree[0]->size;
    long i = 0;
    long j = mid + 1;
    long k = 0;
    long last = tree[0]->size + tree[1]->size;

    AVLNode **array = malloc((tree[0]->size + tree[1]->size) * sizeof(AVLNode *));
    AVLNode **temp = malloc((tree[0]->size + tree[1]->size) * sizeof(AVLNode *));
    //中序遍历平衡二叉树，保存节点到指针数组，这样出来的数组一定是按key有序的
    addAVLTreeRecursively(array, tree[0]->root, &index);
    addAVLTreeRecursively(array, tree[1]->root, &index);

    while (i <= mid && j <= last) {
        if (array[i]->key == array[j]->key) {
            continue;
        }
        if (array[i]->key < array[j]->key)
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }

    while (i <= mid)
        temp[k++] = array[i++];

    while (j <= last)
        temp[k++] = array[j++];

    for (i = 0; i < k; i++)
        array[i] = temp[i];

    for (i = 0; i < last; i++) {
        printAVLNode(array[i]);
    }

    //使用二分法的原理重新构造平衡二叉树，这样出来的树一定是平衡且符合查找树定义的
    (*result)->root = sortedArrayToAVL(array, 0, last - 1);
    (*result)->size = last;
    //重新调整平衡因子
    resetBF((*result)->root);

}

//递归遍历树并添加到数组中，并查找指定的节点
void addAndFindAVLNodeRecursively(AVLNode **array, AVLNode *avlNode, long *index, long keyToFind, long *resultIndex) {
    if (avlNode) {

        if (avlNode->lchild)
            addAndFindAVLNodeRecursively(array, avlNode->lchild, index, keyToFind, resultIndex);
        addTOArray(array, avlNode, *index);
        if (avlNode->key == keyToFind)
            *resultIndex = *index;
        (*index)++;
        if (avlNode->rchild)
            addAndFindAVLNodeRecursively(array, avlNode->rchild, index, keyToFind, resultIndex);

    }
}

/*
 * 把一棵平衡二叉树分裂为两棵平衡二叉树，
 * 使得在一棵树中的所有关键字都小于或等于x，另一棵树中的任一关键字都大于x
 */
void splitAVLTree(AVLTree *avlTree, KeyType key, AVLTree **avlTrees) {

    long index = 0;
    long keyIndex = -1;
    AVLNode **array = malloc((avlTree->size * sizeof(AVLNode *)));
    //中序遍历平衡二叉树，保存节点到指针数组，这样出来的数组一定是按key有序的
    //并且在遍历过程中可同时记录查找节点并记录在数组中的索引，用于后面分割数组
    addAndFindAVLNodeRecursively(array, avlTree->root, &index, key, &keyIndex);

    if (keyIndex == -1) {
        printf("not found");
        return;
    }
    avlTrees[0] = malloc(sizeof(AVLTree *));
    avlTrees[1] = malloc(sizeof(AVLTree *));
    //使用二分法的原理重新构造平衡二叉树，这样出来的树一定是平衡且符合查找树定义的
    avlTrees[0]->root = sortedArrayToAVL(array, 0, keyIndex - 1);
    avlTrees[1]->root = sortedArrayToAVL(array, keyIndex + 1, avlTree->size - 1);
    avlTrees[0]->size = keyIndex;
    avlTrees[1]->size = avlTree->size - 1 - keyIndex;
    //重新调整平衡因子
    resetBF(avlTrees[0]->root);
    resetBF(avlTrees[1]->root);
}








