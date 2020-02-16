
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FilePutServerAdapter implements ReceiveListener {
    protected String protocol;
    protected File f;
    protected FileOutputStream os;

    public FilePutServerAdapter(String protocol, String path) {
        this.protocol = protocol;
        f = new File(path);
    }

    public void create() throws IOException {
        os = new FileOutputStream(f);
    }

    public void close() {
        try {
            os.close();
        } catch (IOException e) {
        }
    }

    public void delete() {
        f.delete();
    }

    public void sendError(Connection conn, IOException e) {
        NetLog.log("FilePutServerAdapter::sendError");
        conn.removeListener(protocol);
        close();
        f.delete();
    }

    public void receiveError(Connection conn, ClassNotFoundException e) {
        NetLog.log("FilePutServerAdapter::receiveError(ClassNotFound)");
        conn.removeListener(protocol);
        close();
        f.delete();
    }

    public void receiveError(Connection conn, IOException e) {
        NetLog.log("FilePutServerAdapter::receiveError");
        conn.removeListener(protocol);
        close();
        f.delete();
    }

    public void received(Connection conn, Message msg) {
        NetLog.debugLog("received: " + msg.toString());
        String s = msg.getMessage();

        if (s.equals("cancel")) {
            conn.send(new Message(protocol, "canceled"));
            conn.removeListener(protocol);
            close();
            f.delete();

        } else if (s.equals("end")) {
            conn.send(new Message(protocol, "end ok"));
            conn.removeListener(protocol);
            close();

        } else if (s.equals("error")) {
            conn.send(new Message(protocol, "error"));
            conn.removeListener(protocol);
            close();
            f.delete();
        }
    }

    public void received(Connection conn, byte[] buff, int blockSize) {
        try {
            os.write(buff, 0, blockSize);
        } catch (IOException e) {
            conn.send(new Message(protocol, "error"));
            conn.removeListener(protocol);
            close();
            f.delete();
            return;
        }
        conn.send(new Message(protocol, "req data"));
    }
}
