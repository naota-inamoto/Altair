
import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;

public class Server {
    public static void main(String argv[]) {
        if (argv.length != 1) {
            System.out.println("Usage: java Server <port>");
            System.exit(0);
        }
        int port = (new Integer(argv[0])).intValue();

        MyServerConnectionManager mgr = null;
        try {
            mgr = new MyServerConnectionManager(port);
        } catch (IOException e) {
            System.err.println(e.getMessage());
            e.printStackTrace();
            System.exit(-1);
        }
        mgr.start();
    }
}

class MyServerConnectionManager extends ServerConnectionManager {
    public MyServerConnectionManager(int port) throws IOException {
        super(port);
    }

    public ServerConnection createServerConnection(Socket s) {
        MyServerConnection conn = new MyServerConnection(this, s);
        conn.start();
        return conn;
    }
}

class MyServerConnection extends ServerConnection {
    public MyServerConnection(MyServerConnectionManager mgr, Socket s) {
        super(mgr, s);
    }

    public void send(Message msg) {
        NetLog.debugLog("send: " + msg.toString());
        super.send(msg);
    }

    public void received(Message msg) {
        super.received(msg);
        String s = msg.getMessage();
        if (s.equals("Hello!")) {
            send(new Message("Hello!"));
        } else if (s.equals("I love you.")) {
            send(new Message("Me too."));
        } else if (s.equals("Would marry with me?")) {
            send(new Message("Yes."));
        } else if (s.equals("See you later.")) {
            send(new Message("See you later."));
            close();
        }
    }

    public void received(Connection conn, byte[] buff, int size) {
    }
}
