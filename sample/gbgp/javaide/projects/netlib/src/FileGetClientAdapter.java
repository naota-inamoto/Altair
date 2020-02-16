
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FileGetClientAdapter implements ReceiveListener {
    protected String remotePath;
    protected String localPath;
    protected ErrorListener el = null;
    protected FileTransferListener l = null;

    protected String protocol;
    protected File f;
    protected FileOutputStream os;
    protected long size = 0;
    protected long totalSize;

    protected boolean cancel_flag = false;

    public FileGetClientAdapter(String remotePath, String localPath) {
        this.remotePath = remotePath;
        this.localPath = localPath;
    }

    public void addErrorListener(ErrorListener el) {
        this.el = el;
    }

    public void addFileTransferListener(FileTransferListener l) {
        this.l = l;
    }

    public void sendRequest(Connection conn) {
        f =  new File(localPath);
        try {
            os = new FileOutputStream(f);
        } catch (IOException e) {
            if (l != null)
                l.cannotCreateFile(localPath);
            return;
        }
        protocol = "file get action";
        Action action = new FileGetAction(protocol, remotePath);
        conn.addListener(protocol, this);
        conn.send(action);
    }

    public void close() {
        try {
            os.close();
        } catch (IOException e) {
        }
        os = null;
    }

    public void cancel() {
        cancel_flag = true;
    }

    public boolean checkCancel(Connection conn) {
        if (cancel_flag == false)
            return false;
        if (l != null)
            l.canceled();
        conn.send(new Message(protocol, "cancel"));
        conn.removeListener(protocol);
        close();
        f.delete();
        return true;
    }

    public void sendError(Connection conn, IOException e) {
        NetLog.log("FileGetClientAdapter::sendError");
        conn.removeListener(protocol);
        close();
        f.delete();
        if (el != null)
            el.sendError(conn, e);
    }

    public void receiveError(Connection conn, ClassNotFoundException e) {
        NetLog.log("FileGetClientAdapter::receiveError(ClassNotFound)");
        conn.removeListener(protocol);
        close();
        f.delete();
        if (el != null)
            el.receiveError(conn, e);
    }

    public void receiveError(Connection conn, IOException e) {
        NetLog.log("FileGetClientAdapter::receiveError");
        conn.removeListener(protocol);
        close();
        f.delete();
        if (el != null)
            el.receiveError(conn, e);
    }

    public void received(Connection conn, Message msg) {
        NetLog.debugLog("received: " + msg.toString());
        String s = msg.getMessage();

        if (s.equals("cannot open")) {
            if (l != null)
                l.cannotOpenFile(remotePath);
            conn.removeListener(protocol);
            close();
            f.delete();

        } else if (s.equals("opened")) {
            String ss = msg.getArg(0);
            long size = (new Long(ss)).longValue();
            totalSize = size;
            if (l != null)
                l.totalFileSize(size);
            if (checkCancel(conn))
                return;
            conn.send(new Message(protocol, "req data"));

        } else if (s.equals("error")) {
            conn.removeListener(protocol);
            close();
            f.delete();
        }
    }

    public void received(Connection conn, byte[] buff, int blockSize) {
        if (os == null)
            return;

        try {
            os.write(buff, 0, blockSize);
        } catch (IOException e) {
            conn.send(new Message(protocol, "cancel"));
            conn.removeListener(protocol);
            close();
            f.delete();
            return;
        }
        size += blockSize;
        if (l != null)
            l.transfered(size);
        if (checkCancel(conn))
            return;
        if (size < totalSize) {
            //conn.send(new Message(protocol, "req data"));
        } else {
            if (l != null)
                l.completed();
            conn.removeListener(protocol);
            close();
        }
    }
}
