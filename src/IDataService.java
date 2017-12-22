/**
 * Created by 林嘉民 on 17/12/22.
 * IntelliJ IDEA for mac 下运行通过
 */
public interface IDataService {

    void onSearch(long key);

    void onInsert(long key);

    void onDelete(long key);

    void onFire();

    AVLNode getRoot();
}
