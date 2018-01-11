

/**
 * Created by 林嘉民 on 17/12/18.
 * IntelliJ IDEA for mac 下运行通过
 * native层AVL结构的管理类
 */
public class DataHelper {

    private static volatile DataHelper INSTANCE;

    static {
        System.load("/Users/alberthumbert/Desktop/AVLVisualization/jni-lib/libavl-lib.dylib");
    }


    public static DataHelper getInstance() {
        if (INSTANCE == null) {
            synchronized (DataHelper.class) {
                if (INSTANCE == null) {
                    INSTANCE = new DataHelper();
                    init();
                }
            }
        }
        return INSTANCE;
    }

    private static native void init();

    public native void insert(long key, int x, int y,int treeIndex);

    public native void remove(long key,int treeIndex);

    public native void fireAll(int treeIndex);

    public native void resetPosition(long key, int x, int y,int treeIndex);

    public native AVLNode findAVLNode(long key,int treeIndex);

    public native AVLNode getRoot(int treeIndex);

    public native void merge();

    public native void split(long key,int treeIndex);

    public void findFinish() {
        System.out.println("finish");

    }

}
