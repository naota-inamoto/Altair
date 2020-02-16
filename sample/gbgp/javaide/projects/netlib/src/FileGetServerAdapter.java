
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FileGetServerAdapter implements ReceiveListener, Runnable {
    public static final int BLOCKSIZE = 32768;

    protected String protocol;
    protected File f;
    protected FileInputStream is;
    protected byte[] data = new byte[blockSize()];
    protected byte[] buff = new byte[65536];

    protected boolean cancel = false;
    protected Connection conn = null;

    public FileGetServerAdapter(String protocol, String path) {
        this.protocol = protocol;
        f = new File(path);
        
    }

    public int blockSize() {
        return BLOCKSIZE;
    }

    public long fileSize() {
        return f.length();
    }

    public void open() throws IOException {
        is = new FileInputStream(f);
    }

    public void close() {
        try {
            is.close();
        } catch (IOException e) {
        }
    }

    public void sendError(Connection conn, IOException e) {
        NetLog.log("FileGetServerAdapter::sendError");
        conn.removeListener(protocol);
        close();
    }

    public void receiveError(Connection conn, ClassNotFoundException e) {
        NetLog.log("FileGetServerAdapter::receiveError(ClassNotFound)");
        conn.removeListener(protocol);
        close();
    }

    public void receiveError(Connection conn, IOException e) {
        NetLog.log("FileGetServerAdapter::receiveError");
        conn.removeListener(protocol);
        close();
    }

    public void received(Connection conn, Message msg) {
        NetLog.debugLog("received: " + msg.toString());
        String s = msg.getMessage();
        if (s.equals("req data")) {
            this.conn = conn;
            Thread t = new Thread(this);
            t.start();
        } else if (s.equals("cancel")) {
            cancel = true;
            conn.removeListener(protocol);
            close();
        }
    }

    public void run() {
        while (cancel == false) {
            int size;
            try {
                size = is.read(buff, 0, blockSize());
            } catch (IOException e) {
                conn.send(new Message(protocol, "error"));
                conn.removeListener(protocol);
                close();
                return;
            }
            if (size == 0) {
                conn.removeListener(protocol);
                close();
                break;
            } else {
                conn.send(protocol, buff, size);
            }
        }
    }

    public void received(Connection conn, byte[] buff, int size) {
    }
}
