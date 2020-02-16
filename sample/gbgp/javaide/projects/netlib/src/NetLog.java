
package altair.net;

import java.lang.*;
import java.io.*;

public class NetLog {
    public static boolean debug = false;

    public static void debugLog(String s) {
        if (debug)
            System.out.println(s);
    }

    public static void log(String s) {
        System.err.println(s);
    }

    public static void log(Exception e) {
        e.printStackTrace(System.err);
    }
}
