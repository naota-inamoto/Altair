
package altair.net;

import java.lang.*;
import java.io.*;

public class Message implements Serializable {
    public static int OBJECT = 1;
    public static int BYTE_ARRAY = 2;

    protected String protocol;
    protected String message;
    protected String[] args;
    protected byte[] data;

    public Message(String message) {
        this.protocol = "";
        this.message = message;
        args = null; data = null;
    }

    public Message(String protocol, String message) {
        this.protocol = protocol;
        this.message = message;
        args = null; data = null;
    }

    public String getProtocol() {
        return protocol;
    }

    public String getMessage() {
        return message;
    }

    public String[] getArg() {
        return args;
    }

    public String getArg(int index) {
        return args[index];
    }

    public byte[] getData() {
        return data;
    }

    public void setArg(String a1) {
        args = new String[1];
        args[0] = a1;
    }

    public void setArg(String a1, String a2) {
        args = new String[2];
        args[0] = a1; args[1] = a2;
    }

    public void setArg(String a1, String a2, String a3) {
        args = new String[3];
        args[0] = a1; args[1] = a2; args[2] = a3;
    }

    public void setArg(String a1, String a2, String a3, String a4) {
        args = new String[4];
        args[0] = a1; args[1] = a2; args[2] = a3; args[3] = a4;
    }

    public void setData(byte[] data, int from, int to) {
        int len = to - from;
        this.data = new byte[len];
        for (int i = 0; i < len; i++)
		this.data[i] = data[from + i];
    }

    public String toString() {
        StringBuffer buf = new StringBuffer();
        buf.append(message);
        if (args != null) {
            int n = args.length;
            for (int i = 0; i < n; i++) {
                buf.append(" -arg "); buf.append(args[i]);
            }
         }
         if (data != null) {
             int n = data.length;
             buf.append(" -data "); buf.append((new Integer(n)).toString());
         }
         return new String(buf);
    }
}
