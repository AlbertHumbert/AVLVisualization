//
// Created by Albert Humbert on 17/12/22.
//

#include "DataHelper.h"

jobject convertAvlNode2jobject(JNIEnv *env, AVLNode *avlNode);

jobject setLchild(JNIEnv *env, jobject jAVLNode, AVLNode *lchild);

jobject setRchild(JNIEnv *env, jobject jAVLNode, AVLNode *rchild);

void createJAVLTree(JNIEnv *env, AVLNode *avlNode, jobject jAvlNode);

static AVLTree *sAvlTree = nullptr;
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

JNIEXPORT void JNICALL Java_DataHelper_insert
        (JNIEnv *env, jobject jobj, jlong jkey, jint jx, jint jy) {

    if (sAvlTree == nullptr)sAvlTree = newAVLTree();
    insertAVLData(sAvlTree, jkey, nullptr);

}


JNIEXPORT void JNICALL Java_DataHelper_remove
        (JNIEnv *env, jobject jobj, jlong jkey) {

    printf("%d", findAVLData(sAvlTree, jkey)->key);
    if (sAvlTree)removeAVLData(sAvlTree, jkey);


}

JNIEXPORT void JNICALL Java_DataHelper_resetPosition
        (JNIEnv *env, jobject jobj, jlong jkey, jint jx, jint jy) {

    AVLNode *avlNode = findAVLData(sAvlTree, jkey);
};

JNIEXPORT jobject JNICALL Java_DataHelper_findAVLNode
        (JNIEnv *env, jobject jobj, jlong jkey) {
    printAVLTree(sAvlTree);

    AVLNode *avlNode = findAVLData(sAvlTree, jkey);
    jobject jAVLNode = convertAvlNode2jobject(env, avlNode);
    createJAVLTree(env, avlNode, jAVLNode);

    return jAVLNode;
}

JNIEXPORT jobject JNICALL Java_DataHelper_getRoot
        (JNIEnv *env, jobject jobj) {
    if (!sAvlTree)return nullptr;
    AVLNode *avlNode = findAVLData(sAvlTree, sAvlTree->root->key);
    jobject jroot = convertAvlNode2jobject(env, avlNode);
    createJAVLTree(env, avlNode, jroot);
    return jroot;
}

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


jobject setLchild(JNIEnv *env, jobject jAVLNode, AVLNode *lchild) {

    jobject jLchild = convertAvlNode2jobject(env, lchild);
    jclass nodeClazz = env->FindClass("AVLNode");
    jfieldID lchildID = env->GetFieldID(nodeClazz, "lchild", "LAVLNode;"); //check
    env->SetObjectField(jAVLNode, lchildID, env->NewGlobalRef(jLchild));

    jobject child = env->GetObjectField(jAVLNode, lchildID);
    jfieldID keyID = env->GetFieldID(nodeClazz, "x", "I");
    int i = env->GetIntField(child, keyID);
    printf("%d", i);

    return jLchild;
}

jobject setRchild(JNIEnv *env, jobject jAVLNode, AVLNode *lchild) {

    jobject jRchild = convertAvlNode2jobject(env, lchild);
    jclass nodeClazz = env->FindClass("AVLNode");
    jfieldID rchildID = env->GetFieldID(nodeClazz, "rchild", "LAVLNode;"); //check
    env->SetObjectField(jAVLNode, rchildID, jRchild);
    return jRchild;

}


//构造Java层的树形结构
void createJAVLTree(JNIEnv *env, AVLNode *avlNode, jobject jAvlNode) {
    if (avlNode->lchild != nullptr) {
        jobject jLchild = setLchild(env, jAvlNode, avlNode->lchild);
        createJAVLTree(env, avlNode->lchild, jLchild);
    }


    if (avlNode->rchild != nullptr) {
        jobject jRchild = setRchild(env, jAvlNode, avlNode->rchild);
        createJAVLTree(env, avlNode->rchild, jRchild);
    }


    printf("create node\n");

}
