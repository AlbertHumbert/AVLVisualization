//
// Created by 林嘉民 on 17/12/22.
//

#include <cstdlib>
#include "DataHelper.h"

jobject convertAvlNode2jobject(JNIEnv *env, AVLNode *avlNode);

jobject setLchild(JNIEnv *env, jobject jAVLNode, AVLNode *lchild);

jobject setRchild(JNIEnv *env, jobject jAVLNode, AVLNode *rchild);

void createJAVLTree(JNIEnv *env, AVLNode *avlNode, jobject jAvlNode);

static AVLTree **sAvlTree = nullptr;
jobject gNodeClazz;

/**
 * 完成native层初始化操作
 * TODO 使用 jni_load 在jvm加载时初始化
 */
JNIEXPORT void JNICALL Java_DataHelper_init
        (JNIEnv *env, jclass jclazz) {

    jclass nodeClazz = env->FindClass("AVLNode");
    gNodeClazz = env->NewGlobalRef(nodeClazz);

}

/*
 * 插入操作的native层实现
 */
JNIEXPORT void JNICALL Java_DataHelper_insert
        (JNIEnv *env, jobject jobj, jlong jkey, jint jx, jint jy,jint treeIndex) {

    if (sAvlTree == nullptr){
        sAvlTree = (AVLTree **)malloc(2*sizeof(AVLTree *));
        sAvlTree[0] = newAVLTree();
        sAvlTree[1] = newAVLTree();
    }
    insertAVLData(sAvlTree[treeIndex], jkey, nullptr);

}


/*
 * 删除操作的native层实现
 */
JNIEXPORT void JNICALL Java_DataHelper_remove
        (JNIEnv *env, jobject jobj, jlong jkey,jint treeIndex) {

    if (!sAvlTree || sAvlTree[treeIndex]->root == nullptr)return;
    printf("%li", findAVLData(sAvlTree[treeIndex], jkey)->key);
    if (sAvlTree)removeAVLData(sAvlTree[treeIndex], jkey);


}

/*
 * 重设坐标的native层实现
 */
JNIEXPORT void JNICALL Java_DataHelper_resetPosition
        (JNIEnv *env, jobject jobj, jlong jkey, jint jx, jint jy,jint treeIndex) {

    if (!sAvlTree || sAvlTree[treeIndex]->root == nullptr)return;
    AVLNode *avlNode = findAVLData(sAvlTree[treeIndex], jkey);
};

/*
 * 查找操作的native层实现
 */
JNIEXPORT jobject JNICALL Java_DataHelper_findAVLNode
        (JNIEnv *env, jobject jobj, jlong jkey,jint treeIndex) {
    if (!sAvlTree || !sAvlTree[treeIndex]->root)return nullptr;
    printAVLTree(sAvlTree[treeIndex]);

    AVLNode *avlNode = findAVLData(sAvlTree[treeIndex], jkey);
    if (!avlNode)return nullptr;
    jobject jAVLNode = convertAvlNode2jobject(env, avlNode);
    createJAVLTree(env, avlNode, jAVLNode);

    return jAVLNode;
}

/*
 * 获取根节点的native层实现
 */
JNIEXPORT jobject JNICALL Java_DataHelper_getRoot
        (JNIEnv *env, jobject jobj,jint treeIndex) {
    if (!sAvlTree || !sAvlTree[treeIndex]->root)return nullptr;
    AVLNode *avlNode = findAVLData(sAvlTree[treeIndex], sAvlTree[treeIndex]->root->key);
    jobject jroot = convertAvlNode2jobject(env, avlNode);
    createJAVLTree(env, avlNode, jroot);
    return jroot;
}

/*
 * 将native层的节点结构体转换为java层的节点对象
 */
jobject convertAvlNode2jobject(JNIEnv *env, AVLNode *avlNode) {

    if (!avlNode)return nullptr;
    jclass nodeClazz = env->FindClass("AVLNode");
    jmethodID mid = env->GetMethodID(nodeClazz, "<init>", "()V");
    jobject jAVLNode = env->NewObject(nodeClazz, mid);
    jfieldID keyID = env->GetFieldID(nodeClazz, "key", "J");
    jfieldID bfID = env->GetFieldID(nodeClazz, "bf", "I");
    jfieldID xID = env->GetFieldID(nodeClazz, "x", "I");
    jfieldID yID = env->GetFieldID(nodeClazz, "y", "I");

    env->SetLongField(jAVLNode, keyID, avlNode->key);
    env->SetIntField(jAVLNode, bfID, avlNode->bf);
    env->SetIntField(jAVLNode, xID, 1);
    env->SetIntField(jAVLNode, yID, 1);
    return jAVLNode;

}


/*
 * 设定java层节点对象的左孩子节点
 */
jobject setLchild(JNIEnv *env, jobject jAVLNode, AVLNode *lchild) {

    jobject jLchild = convertAvlNode2jobject(env, lchild);
    jclass nodeClazz = env->FindClass("AVLNode");
    jfieldID lchildID = env->GetFieldID(nodeClazz, "lchild", "LAVLNode;"); //check
    env->SetObjectField(jAVLNode, lchildID, env->NewGlobalRef(jLchild));

    jobject child = env->GetObjectField(jAVLNode, lchildID);
    jfieldID keyID = env->GetFieldID(nodeClazz, "x", "I");
    int i = env->GetIntField(child, keyID);

    return jLchild;
}


/*
 * 设定java层节点对象的左孩子节点
 */
jobject setRchild(JNIEnv *env, jobject jAVLNode, AVLNode *rchild) {

    jobject jRchild = convertAvlNode2jobject(env, rchild);
    jclass nodeClazz = env->FindClass("AVLNode");
    jfieldID rchildID = env->GetFieldID(nodeClazz, "rchild", "LAVLNode;"); //check
    env->SetObjectField(jAVLNode, rchildID, jRchild);
    return jRchild;

}

/*
 * 构造Java层节点的树形结构
 */
void createJAVLTree(JNIEnv *env, AVLNode *avlNode, jobject jAvlNode) {
    if (avlNode->lchild != nullptr) {
        jobject jLchild = setLchild(env, jAvlNode, avlNode->lchild);
        createJAVLTree(env, avlNode->lchild, jLchild);
    }


    if (avlNode->rchild != nullptr) {
        jobject jRchild = setRchild(env, jAvlNode, avlNode->rchild);
        createJAVLTree(env, avlNode->rchild, jRchild);
    }

}

/*
 * 移除所有节点
 */
JNIEXPORT void JNICALL Java_DataHelper_fireAll
        (JNIEnv *env, jobject jobj,jint treeIndex) {

    if (!sAvlTree || !sAvlTree[treeIndex]->root)return;
    releaseAVLTree(sAvlTree[treeIndex]);
    sAvlTree = nullptr;
}

/*
 * Class:     DataHelper
 * Method:    merge
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_DataHelper_merge
        (JNIEnv * env, jobject jobjx){

    AVLTree * avlTree = newAVLTree();
    mergeAVLTree(sAvlTree,&avlTree);
    sAvlTree[0] = avlTree;
    sAvlTree[1] = newAVLTree();

}
/*
 * Class:     DataHelper
 * Method:    split
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_DataHelper_split
        (JNIEnv * env, jobject jobj, jlong key, jint treeIndex){
    AVLTree ** avlTree = (AVLTree **)malloc(2*sizeof(AVLTree *));
    if(sAvlTree[treeIndex]->size ==1){
        removeAVLData(sAvlTree[treeIndex],key);
        return;
    }
    splitAVLTree(sAvlTree[treeIndex],key,avlTree);
    sAvlTree[0] = avlTree[0];
    sAvlTree[1] = avlTree[1];
    printAVLTree(sAvlTree[0]);
}