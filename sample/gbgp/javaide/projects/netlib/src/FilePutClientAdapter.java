
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FilePutClientAdapter implements ReceiveListener {
    public static final int BLOCKSIZE = 32768;

    protected String localPath;
    protected String remotePath;
    protected ErrorListener el = null;
    protected FileTransferListener l = null;

    protected String protocol;
    protected File f;
    protected FileInputStream is;
    protected long size = 0;
    protected long totalSize;
    protected byte[] data = new byte[blockSize()];

    protected boolean cancel_flag = false;

    public FilePutClientAdapter(String localPath, String remotePath) {
        this.localPath = localPath;
        this.remotePath = remotePath;
    }

    public int blockSize() {
        return BLOCKSIZE;
    }

    public void addErrorListener(ErrorListener el) {
        this.el = el;
    }

    public void addFileTransferListener(FileTransferListener l) {
        this.l = l;
    }

    public void sendRequest(Connection conn) {
        f =  new File(localPath);
        long size = f.length();
        totalSize = size;
        if (l != null)
            l.totalFileSize(size);
        try {
            is = new FileInputStream(f);
        } catch (IOException e) {
            if (l != null)
                l.cannotOpenFile(localPath);
            return;
        }
        protocol = "file put action";
        Action action = new FilePutAction(protocol, remotePath);
        conn.addListener(protocol, this);
        conn.send(action);
    }

    public void close() {
        try {
            is.close();
        } catch (IOException e) {
        }
    }

    public void cancel() {
        cancel_flag = true;
    }

    public boolean checkCancel(Connection conn) {
        if (cancel_flag == false)
            return false;
        conn.send(new Message(protocol, "cancel"));
        return true;
    }

    public void sendError(Connection conn, IOException e) {
        NetLog.log("FilePutClientAdapter::sendError");
        conn.removeListener(protocol);
        close();
        if (el != null)
            el.sendError(conn, e);
    }

    public void receiveError(Connection conn, ClassNotFoundException e) {
        NetLog.log("FilePutClientAdapter::receiveError(ClassNotFound)");
        conn.removeListener(protocol);
        close();
        if (el != null)
            el.receiveError(conn, e);
    }

    public void receiveError(Connection conn, IOException e) {
        NetLog.log("FilePutClientAdapter::receiveError");
        conn.removeListener(protocol);
        close();
        if (el != null)
            el.receiveError(conn, e);
    }

    public void received(Connection conn, Message msg) {
        NetLog.debugLog("received: " + msg.toString());
        String s = msg.getMessage();

        if (s.equals("cannot create")) {
            if (l != null)
                l.cannotCreateFile(remotePath);
            conn.removeListener(protocol);
            close();

        } else if (s.equals("req data")) {
            if (checkCancel(conn))
                return;
            if (size == totalSize) {
                Message msg2;
                msg2 = new Message(protocol, "end");
                conn.send(msg2);
                if (l != null)
                    l.completed();
                return;
            }
            int size2;
            try {
                size2 = is.read(data);
            } catch (IOException e) {
                conn.send(new Message(protocol, "error"));
                return;
            }
            conn.send(protocol, data, size2);
            size += size2;
            if (l != null)
                l.transfered(size);

        } else if (s.equals("end ok")) {
            if (l != null)
                l.completed();
            conn.removeListener(protocol);
            close();

        } else if (s.equals("canceled")) {
            if (l != null)
                l.canceled();
            conn.removeListener(protocol);
            close();

        } else if (s.equals("error")) {
            if (l != null)
                l.ioError();
            conn.removeListener(protocol);
            close();
        }
    }

    public void received(Connection conn, byte[] buff, int size) {
    }
}
