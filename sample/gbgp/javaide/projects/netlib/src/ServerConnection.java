
package altair.net;

import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.*;

import altair.net.*;

public class ServerConnection implements Connection {
    private ServerConnectionManager mgr;
    private Socket socket;
    private ObjectOutputStream os = null;
    private int writeCount = 0;
    private ReceiveHandler h = null;
    private Thread t = null;

    private Hashtable listeners = new Hashtable();

    public ServerConnection(ServerConnectionManager mgr, Socket socket) {
        this.mgr = mgr;
        this.socket = socket;
    }

    public Socket getSocket() {
        return socket;
    }

    public void addListener(String protocol, ReceiveListener l) {
        listeners.put(protocol, l);
    }

    public void removeListener(String protocol) {
        listeners.remove(protocol);
    }

    public void start() {
        if (t != null)
            return;
        mgr.addConnection(this);
        h = new ReceiveHandler(this, listeners);
        t = new Thread(h);
        t.start();
    }

    public void close() {
        if (h != null) {
            h.stop();
        }
        h = null;
        if (mgr != null) {
            mgr.closing(this);
            mgr.removeConnection(this);
        }
        mgr = null;
        if (socket != null) {
            try {
                socket.close();
            } catch (IOException e) {
                NetLog.log(e);
            }
        }
        socket = null;
    }

    public void send(Message msg) {
        try {
            if (os == null)
                os = new ObjectOutputStream(socket.getOutputStream());
            os.write(Message.OBJECT);
            os.writeObject(msg);
            if (++writeCount >= 1024) {
                writeCount = 0;
                os.reset();
            }
        } catch (IOException e) {
            ErrorListener el = (ErrorListener)listeners.get(msg.getProtocol());
            if (el != null)
                el.sendError(this, e);
            sendError(e);
        } 
    }

    public void send(String protocol, byte[] buff, int size) {
        try {
            if (os == null)
                os = new ObjectOutputStream(socket.getOutputStream());
            os.write(Message.BYTE_ARRAY);
            os.writeObject(new Header());
            os.writeUTF(protocol);
            os.write(size >> 24);
            os.write(size >> 16);
            os.write(size >> 8);
            os.write(size);
            os.write(buff, 0, size);
            os.flush();
        } catch (IOException e) {
            ErrorListener el = (ErrorListener)listeners.get(protocol);
            if (el != null)
                el.sendError(this, e);
            sendError(e);
        }
    }

    // listener
    public void sendError(IOException e) {
        NetLog.log("ServerConnection::sendError");
        NetLog.log(e);
        close();
    }

    public void receiveError(ClassNotFoundException e) {
        NetLog.log("ServerConnection::receiveError(ClassNotFound)");
        NetLog.log(e);
        send(new Message("close connection"));
        close();
    }

    public void receiveError(IOException e) {
        NetLog.log("ServerConnection::receiveError");
        NetLog.log(e);
        close();
    }

    public void received(Message msg) {
        NetLog.debugLog("received: " + msg.toString());
        String s = msg.getMessage();
        if (s.equals("close connection")) {
            send(new Message("close connection"));
            close();
        }
    }

    public void received(Connection conn, byte[] buff, int size) {
    }

    public void received(byte[] buff, int size) {
    }
}
