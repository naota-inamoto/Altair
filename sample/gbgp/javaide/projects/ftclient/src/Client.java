
import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class Client {
    public static void printUsage() {
        System.out.println("get <remotePath> <localPath>");
        System.out.println("put <localPath> <remotePath>");
        System.out.println("cancel");
        System.out.println("quit");
    }

    public static void main(String[] argv) {
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
            e.printStackTrace();
            System.exit(-1);
        }
        MyClientConnection conn = null;
        try {
            conn = new MyClientConnection(addr, port);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }
        conn.start();

        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        String s;
        try {
            FileGetClientAdapter fga = null;
            FilePutClientAdapter fpa = null;
            while (true) {
                System.out.print("file transfer> ");
                s = in.readLine();
                if (s.equals("quit")) {
                    break;
                }
                StreamTokenizer st = new StreamTokenizer(new StringReader(s));
                if (st.nextToken() == StreamTokenizer.TT_EOF) {
                    continue;
                }
                if (st.sval.equals("cancel")) {
                    if (fga != null) fga.cancel();
                    if (fpa != null) fpa.cancel();
                } else if (st.sval.equals("get")) {
                    if (st.nextToken() == StreamTokenizer.TT_EOF) {
                        printUsage(); continue;
                    }
                    String remotePath = st.sval;
                    if (st.nextToken() == StreamTokenizer.TT_EOF) {
                        printUsage(); continue;
                    }
                    String localPath = st.sval;
                    fga = new FileGetClientAdapter(remotePath, localPath);
                    fga.addErrorListener(new MyErrorAdapter());
                    fga.addFileTransferListener(new MyFileTransferAdapter());
                    fga.sendRequest(conn);
                } else if (st.sval.equals("put")) {
                    if (st.nextToken() == StreamTokenizer.TT_EOF) {
                        printUsage(); continue;
                    }
                    String localPath = st.sval;
                    if (st.nextToken() == StreamTokenizer.TT_EOF) {
                        printUsage(); continue;
                    }
                    String remotePath = st.sval;
                    fpa = new FilePutClientAdapter(localPath, remotePath);
                    fpa.addErrorListener(new MyErrorAdapter());
                    fpa.addFileTransferListener(new MyFileTransferAdapter());
                    fpa.sendRequest(conn);
                } else {
                    printUsage(); continue;
                }
            }
        } catch (IOException e) {
            System.err.println(e.getMessage());
            e.printStackTrace();
            System.exit(-1);
        }
        conn.send(new Message("close connection"));
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
}

class MyErrorAdapter implements ErrorListener {
    public MyErrorAdapter() {
    }

    public void sendError(Connection conn, IOException e) {
        NetLog.debugLog("MyErrorListener::sendError");
    }

    public void receiveError(Connection conn, ClassNotFoundException e) {
        NetLog.debugLog("MyErrorListener::receiveError(ClassNotFound)");
    }

    public void receiveError(Connection conn, IOException e) {
        NetLog.debugLog("MyErrorListener::receiveError");
    }
}

class MyFileTransferAdapter implements FileTransferListener {
    private long size = 0;

    public MyFileTransferAdapter() {
    }

    public void completed() {
        NetLog.log((new Date()).toString() + ": file transfer completed.");
    }

    public void canceled() {
        NetLog.debugLog("file transfer canceled.");
    }

    public void totalFileSize(long size) {
        this.size = size;
        NetLog.log((new Date()).toString() + ": file total size = " + (new Long(size)).toString());
    }

    public void transfered(long size) {
        double total = (double)this.size;
        double transfer = (double)size;
        int progress = (int)(transfer * 100 / total);
        NetLog.debugLog("file transfered: " + (new Integer(progress)).toString() + "%");
    }

    public void cannotCreateFile(String path) {
        NetLog.log("cannot create file: " + path);
    }

    public void cannotOpenFile(String path) {
        NetLog.log("cannot open file: " + path);
    }

    public void ioError() {
        NetLog.log("io error");
    }
}
