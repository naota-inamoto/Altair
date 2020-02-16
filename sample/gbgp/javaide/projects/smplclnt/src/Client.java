
import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;

public class Client {
    public static void main(String argv[]) {
        if (argv.length != 2) {
            System.out.println("Usage: java Client <host> <port>");
            System.exit(0);
        }
        String host = argv[0];
        int port = (new Integer(argv[1])).intValue();

        InetAddress addr = null;
        try {
            addr = InetAddress.getByName(host);
        } catch (UnknownHostException e) {
            System.err.println(e.getMessage());
            e.printStackTrace();
            System.exit(-1);
        }
        MyClientConnection conn = null;
        try {
            conn = new MyClientConnection(addr, port);
        } catch (IOException e) {
            System.err.println(e.getMessage());
            e.printStackTrace();
            System.exit(-1);
        }
        conn.start();
        conn.send(new Message("Hello!"));
    }
}

class MyClientConnection extends ClientConnection {
    public MyClientConnection(InetAddress addr, int port) throws IOException {
        super(addr, port);
    }

    public void send(Message msg) {
        NetLog.debugLog("send: " + msg.toString());
        super.send(msg);
    }

    public void received(Message msg) {
        super.received(msg);
        String s = msg.getMessage();
        if (s.equals("Hello!")) {
            send(new Message("I love you."));
        } else if (s.equals("Me too.")) {
            send(new Message("Would marry with me?"));
        } else if (s.equals("Yes.")) {
            send(new Message("See you later."));
        } else if (s.equals("See you later.")) {
            close();
        }
    }

    public void received(Connection conn, byte[] buff, int size) {
    }
}
