
/* $Id: Test.java,v 1.3 1999/04/04 02:44:31 chiaki Exp inamoto $ */

import java.util.*;

public class Test {
    public static void main(String[] args) {
        Vector v = new Vector();
        v.add(new String("篠置 昭男"));
        v.add(new String("田中 千晶"));
        v.add(new String("国井 利康"));
        v.add(new String("稲本 直太"));

        // 稲本 loves 千晶
        // 千晶 marrys with 稲本

        int s = 0;
        for (int i = 1; i <= 1000; i++) {
            s += i;
        }
        System.out.println(s);
        System.exit(0);
    }
}
