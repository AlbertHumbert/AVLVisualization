/**
 * Created by 林嘉民 on 17/12/22.
 * IntelliJ IDEA for mac 下运行通过
 */
public class DataService implements IDataService {

    private DataHelper mDataHelper;
    private AVLNode mRoot;

    public DataService() {
        mDataHelper = DataHelper.getInstance();
    }

    @Override
    public AVLNode onSearch(long key) {
        return mDataHelper.findAVLNode(key);
    }

    @Override
    public void onInsert(long key) {
        mDataHelper.insert(key, 0, 0);
        mRoot = mDataHelper.getRoot();

    }

    @Override
    public void onDelete(long key) {
        mDataHelper.remove(key);
        mRoot = mDataHelper.getRoot();
    }

    @Override
    public void onFire() {
        mDataHelper.fireAll();
        mRoot = mDataHelper.getRoot();
    }

    @Override
    public AVLNode getRoot() {
        return mRoot;
    }
}
