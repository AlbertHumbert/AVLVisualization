import javax.swing.*;

/**
 * Created by 林嘉民 on 17/12/18.
 * IntelliJ IDEA for mac 下运行通过
 * AVL可视化程序入口
 */
public class AVLApplication {

    public static void main(String[] args) {
        start();
    }

    private static void start() {
        AVLTreePanel treePanel = new AVLTreePanel();
        treePanel.setOptionCallback(new DataService());

        JFrame jFrame = new JFrame();
        jFrame.setBounds(0, 0, 800, 640);
        jFrame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        jFrame.add(treePanel);
        jFrame.setVisible(true);
        treePanel.repaint();
    }
}
