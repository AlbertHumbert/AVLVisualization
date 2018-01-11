/**
 * Created by 林嘉民 on 17/12/22.
 * IntelliJ IDEA for mac 下运行通过
 */
public class DataService implements IDataService {

    private DataHelper mDataHelper;
    private AVLNode[] mRoot = new AVLNode[2];

    public DataService() {
        mDataHelper = DataHelper.getInstance();
    }

    @Override
    public AVLNode onSearch(long key,int treeIndex) {
        return mDataHelper.findAVLNode(key,treeIndex);
    }

    @Override
    public void onInsert(long key,int treeIndex) {
        mDataHelper.insert(key, 0, 0,treeIndex);
        mRoot[treeIndex] = mDataHelper.getRoot(treeIndex);

    }

    @Override
    public void onDelete(long key,int treeIndex) {
        mDataHelper.remove(key,treeIndex);
        mRoot[treeIndex] = mDataHelper.getRoot(treeIndex);
    }

    @Override
    public void onFire(int treeIndex) {
        mDataHelper.fireAll(treeIndex);
        mRoot[treeIndex] = mDataHelper.getRoot(treeIndex);
    }

    @Override
    public void onMerge() {
        mDataHelper.merge();
        mRoot[0] = mDataHelper.getRoot(0);
        mRoot[1] = mDataHelper.getRoot(1);

    }

    @Override
    public void onSplit(long key,int treeIndex) {
        mDataHelper.split(key,treeIndex);
        mRoot[0] = mDataHelper.getRoot(0);
        mRoot[1] = mDataHelper.getRoot(1);
    }

    @Override
    public AVLNode getRoot(int treeIndex) {
        return mRoot[treeIndex];
    }
}
