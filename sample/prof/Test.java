
import java.io.*;

public class Test {
    static PrintWriter pw;
    int count;

    public static void main(String[] av) throws Exception {
        pw = new PrintWriter(new FileOutputStream("java_prof.txt"));

        Test test = new Test();
        test.count = 100;

        long t1;
        long t2;

        t1 = System.currentTimeMillis();
        test.test_all();
        t2 = System.currentTimeMillis();
        pw.println("test_all: " + (t2-t1) + "msec");

        pw.close();
   }

    void test_all() {
        long t1;
        long t2;

        t1 = System.currentTimeMillis();
        test_loop();
        t2 = System.currentTimeMillis();
        pw.println("test_loop: " + (t2-t1) + "msec");

        t1 = System.currentTimeMillis();
        test_int_add();
        t2 = System.currentTimeMillis();
        pw.println("test_int_add: " + (t2-t1) + "msec");

        t1 = System.currentTimeMillis();
        test_string_add();
        t2 = System.currentTimeMillis();
        pw.println("test_string_add: " + (t2-t1) + "msec");

        t1 = System.currentTimeMillis();
        test_new();
        t2 = System.currentTimeMillis();
        pw.println("test_new: " + (t2-t1) + "msec");
    }

    void test_loop() {
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < 10000; j++) {
            }
        }
    }

    void test_int_add() {
        for (int i = 0; i < count; i++) {
            int s = 0;
            for (int j = 0; j < 10000; j++) {
                s += j;
            }
        }
    }

    void test_string_add() {
        for (int i = 0; i < count; i++) {
            String s = "";
            for (int j = 0; j < 10000; j++) {
                s += "abcd";
            }
        }
    }

    void test_new() {
        for (int i = 0; i < count; i++) {
            Test test;
            for (int j = 0; j < 10000; j++) {
                test = new Test();
            }
        }
    }
}

