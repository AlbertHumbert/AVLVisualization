/**
 * Created by 林嘉民 on 17/12/22.
 * IntelliJ IDEA for mac 下运行通过
 */
public interface IDataService {

    AVLNode onSearch(long key,int treeIndex);

    void onInsert(long key,int treeIndex);

    void onDelete(long key,int treeIndex);

    void onFire(int treeIndex);

    void onMerge();

    void onSplit(long key,int treeIndex);

    AVLNode getRoot(int treeIndex);
}
