
/* $Id: Test.java,v 1.3 1999/04/04 02:44:31 chiaki Exp inamoto $ */

import java.util.*;

public class Test {
    public static void main(String[] args) {
        Vector v = new Vector();
        v.add(new String("�u ���j"));
        v.add(new String("�c�� �珻"));
        v.add(new String("���� ���N"));
        v.add(new String("��{ ����"));

        // ��{ loves �珻
        // �珻 marrys with ��{

        int s = 0;
        for (int i = 1; i <= 1000; i++) {
            s += i;
        }
        System.out.println(s);
        System.exit(0);
    }
}
