
package altair.net;

import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.*;

import altair.net.*;

public class ServerConnectionManager {
    private ServerSocket ssocket;
    private ConnectionHandler h = null;
    private Thread t = null;

    private Vector connections;

    public ServerConnectionManager(int port) throws IOException {
        ssocket = new ServerSocket(port);
        connections = new Vector();
    }

    public ServerSocket getServerSocket() {
        return ssocket;
    }

    public void start() {
        if (t != null)
            return;
        h = new ConnectionHandler(this);
        t = new Thread(h);
        t.start();
    }

    public void close() {
        if (h != null)
            h.stop();
        try {
            ssocket.close();
        } catch (IOException e) {
            NetLog.log(e);
        }
    }

    public synchronized void addConnection(ServerConnection conn) {
        connections.addElement(conn);
    }

    public synchronized void removeConnection(ServerConnection conn) {
        int index = connections.indexOf(conn);
        connections.remove(index);
    }

    // accept
    public ServerConnection createServerConnection(Socket s) {
        ServerConnection conn = new ServerConnection(this, s);
        conn.start();
        return conn;
    }

    // listener
    public void connected(ServerConnection conn) {
        NetLog.debugLog("connected");
    }

    public void closing(ServerConnection conn) {
        NetLog.debugLog("closing");
    }

    public void acceptError(IOException e) {
        NetLog.log("acceptError");
        NetLog.log(e);
    }
}

class ConnectionHandler implements Runnable {
    private ServerConnectionManager mgr;
    private boolean cont;

    public ConnectionHandler(
            ServerConnectionManager mgr) {
        this.mgr = mgr;
        cont = true;
    }

    public void stop() {
        cont = false;
    }

    public void run() {
        while (cont) {
            Socket s = null;
            try {
                s = mgr.getServerSocket().accept();
            } catch (IOException e) {
                mgr.acceptError(e);
                continue;
            }
            ServerConnection conn = mgr.createServerConnection(s);
            mgr.connected(conn);
        }
    }
}
