
import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;
import altair.net.act.*;

public class Server {
    public static void main(String[] argv) {
        if (argv.length != 1) {
            System.out.println("Usage: java Server <port>");
            System.exit(0);
        }
        int port = (new Integer(argv[0])).intValue();

        MyServerConnectionManager mgr = null;
        try {
            mgr = new MyServerConnectionManager(port);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }
        mgr.start();
    }
}

class MyServerConnectionManager extends ActionServerConnectionManager {
    public MyServerConnectionManager(int port) throws IOException {
        super(port);
    }

    public ServerConnection createServerConnection(Socket s) {
        MyServerConnection conn = new MyServerConnection(this, s);
        conn.start();
        return conn;
    }
}

class MyServerConnection extends ActionServerConnection {
    public MyServerConnection(MyServerConnectionManager mgr, Socket s) {
        super(mgr, s);
    }

    public void send(Message msg) {
        NetLog.debugLog("send: " + msg.toString());
        super.send(msg);
    }
}
