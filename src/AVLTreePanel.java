import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by 林嘉民 on 17/12/22.
 * IntelliJ IDEA for mac 下运行通过
 */
public class AVLTreePanel extends JPanel implements ActionListener {

    private AVLNode mRoot;
    public static final int mStartX = 400;
    public static final int mStartY = 20;
    public static final int mDivideY = 60;
    public static final String CMD_INSERT = "插入";
    public static final String CMD_DELETE = "移除";
    public static final String CMD_SEARCH = "查找";
    public static final String CMD_FIRE = "销毁";
    public static final String CMD_MERGE = "合并";
    public static final String CMD_SPLIT = "分裂";

    private JButton mInsertButton;
    private JButton mDeleteButton;
    private JButton mSearchButton;
    private JButton mFireAllButton;
    private JButton mMergeButton;
    private JButton mSplitButton;

    private JTextField mKeyTextField;
    private JComboBox<String> mComboBox;
    private IDataService mCallback;
    private AVLNode mFindingResult;

    public AVLTreePanel() {
        mInsertButton = new JButton(CMD_INSERT);
        mSearchButton = new JButton(CMD_SEARCH);
        mFireAllButton = new JButton(CMD_FIRE);
        mDeleteButton = new JButton(CMD_DELETE);
        mMergeButton = new JButton(CMD_MERGE);
        mSplitButton = new JButton(CMD_SPLIT);
        mKeyTextField = new JTextField();

        mInsertButton.addActionListener(this);
        mDeleteButton.addActionListener(this);
        mSearchButton.addActionListener(this);
        mFireAllButton.addActionListener(this);
        mSplitButton.addActionListener(this);
        mMergeButton.addActionListener(this);


        mKeyTextField.setColumns(6);
        mComboBox = new JComboBox<>();
        mComboBox.addItem("1");
        mComboBox.addItem("2");

        setLayout(new BorderLayout());
        JPanel jPanel = new JPanel();
        jPanel.add(new JLabel("关键字："), BorderLayout.SOUTH);
        jPanel.add(mKeyTextField);
        jPanel.add(new JLabel("树编号："), BorderLayout.SOUTH);
        jPanel.add(mComboBox, BorderLayout.SOUTH);
        jPanel.add(mInsertButton, BorderLayout.SOUTH);
        jPanel.add(mSearchButton, BorderLayout.SOUTH);
        jPanel.add(mDeleteButton, BorderLayout.SOUTH);
        jPanel.add(mFireAllButton, BorderLayout.SOUTH);
        jPanel.add(mMergeButton, BorderLayout.SOUTH);
        jPanel.add(mSplitButton, BorderLayout.SOUTH);
        this.add(jPanel, BorderLayout.SOUTH);
    }

    @Override
    public void paint(Graphics g) {
        super.paint(g);
        AVLNode root;
        if ((root = mCallback.getRoot(0)) != null) {

            g.setColor(Color.orange);
            paintTree(g, root, mStartX, mStartY, (int) (4 * Math.pow(2, getAVLTreeHeightRecursively(root))));
        }

        if ((root = mCallback.getRoot(1)) != null) {

            g.setColor(Color.orange);
            paintTree(g, root, mStartX, 320, (int) (4 * Math.pow(2, getAVLTreeHeightRecursively(root))));
        }

    }

    //递归获取平衡二叉树高度，单一路径
    int getAVLTreeHeightRecursively(AVLNode node) {

        return node == null ? 0 : node.bf == 1 ?
                getAVLTreeHeightRecursively(node.lchild) + 1 : getAVLTreeHeightRecursively(node.rchild) + 1;

    }


    private void paintTree(Graphics g, AVLNode node, int x, int y, int divideX) {

        if (node != null) {
            if (node.lchild != null) g.drawLine(x + 10, y + 10, x - divideX + 10, y + mDivideY + 10);
            paintTree(g, node.lchild, x - divideX, y + mDivideY, divideX / 2);
            if (node.rchild != null) g.drawLine(x + 10, y + 10, x + 10 + divideX, y + 10 + mDivideY);
            paintTree(g, node.rchild, x + divideX, y + mDivideY, divideX / 2);


            if (mFindingResult != null && node.key == mFindingResult.key) g.setColor(Color.red);
            else g.setColor(Color.orange);
            g.fillOval(x, y, 20, 20);

            g.setColor(Color.BLUE);
            g.drawString(node.key + "", x + 2, y + 16);
            g.setColor(Color.orange);
        }

    }

    public AVLNode getRoot() {
        return mRoot;
    }

    public void setRoot(AVLNode mRoot) {
        this.mRoot = mRoot;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (mCallback == null) return;
        int treeIndex = mComboBox.getSelectedIndex();
        System.out.println(treeIndex);
        System.out.println(treeIndex);
        switch (e.getActionCommand()) {

            case CMD_INSERT:
                if (mKeyTextField.getText().isEmpty()) return;
                mCallback.onInsert(Long.parseLong(mKeyTextField.getText()), treeIndex);
                break;
            case CMD_DELETE:
                if (mKeyTextField.getText().isEmpty()) return;
                mCallback.onDelete(Long.parseLong(mKeyTextField.getText()), treeIndex);
                break;
            case CMD_SEARCH:
                if (mKeyTextField.getText().isEmpty()) return;
                mFindingResult = mCallback.onSearch(Long.parseLong(mKeyTextField.getText()), treeIndex);
                break;
            case CMD_FIRE:
                mCallback.onFire(treeIndex);
                break;
            case CMD_MERGE:
                mCallback.onMerge();
                break;
            case CMD_SPLIT:
                if (mKeyTextField.getText().isEmpty()) return;
                mCallback.onSplit(Long.parseLong(mKeyTextField.getText()), treeIndex);
                break;

        }
        repaint();
    }

    public void setOptionCallback(IDataService callback) {
        mCallback = callback;
    }
}
