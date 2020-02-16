
package altair.net;

import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.*;

import altair.net.*;
import altair.net.*;

public class ReceiveHandler implements Runnable {
    private Connection conn;
    private boolean cont = true;

    private Hashtable listeners;

    public ReceiveHandler(Connection conn, Hashtable listeners) {
        this.conn = conn;
        this.listeners = listeners;
    }

    public void stop() {
        cont = false;
    }

    public void run() {
        String protocol = "";
        ObjectInputStream is = null;
        byte[] buff = new byte[65536];
        try {
            is = new ObjectInputStream(conn.getSocket().getInputStream());
        } catch (IOException e) {
            ErrorListener el = (ErrorListener)listeners.get(protocol);
            if (el != null)
                el.receiveError(conn, e);
            conn.receiveError(e);
            return;
        }
        while (cont) {
            int tag = 0;
            Message msg = null;
            try {
                tag = is.read();
            } catch (IOException e) {
                conn.receiveError(e);
                return;
            }
            if (tag == Message.OBJECT) {
                try {
                    msg = (Message)is.readObject();
                } catch (ClassNotFoundException e) {
                    ErrorListener el = (ErrorListener)listeners.get(protocol);
                    if (el != null)
                        el.receiveError(conn, e);
                    conn.receiveError(e);
                    return;
                } catch (IOException e) {
                    ErrorListener el = (ErrorListener)listeners.get(protocol);
                    if (el != null)
                        el.receiveError(conn, e);
                    conn.receiveError(e);
                    return;
                }
                protocol = msg.getProtocol();
                ReceiveListener l = (ReceiveListener)listeners.get(protocol);
                if (l !=  null) {
                    l.received(conn, msg);
                } else {
                    conn.received(msg);
                }

            } else if (tag == Message.BYTE_ARRAY) {
                try {
                    is.readObject();
                    protocol = is.readUTF();
                    int size = 0;
                    int offset = 0;
                    size = (size << 8) + is.read();
                    size = (size << 8) + is.read();
                    size = (size << 8) + is.read();
                    size = (size << 8) + is.read();
                    int size2 = size;
                    while (size2 > 0) {
                        int len = is.read(buff, offset, size2);
                        size2 -= len;
                        offset += len;
                    }
                    ReceiveListener l = (ReceiveListener)listeners.get(protocol);
                    if (l !=  null) {
                        l.received(conn, buff, size);
                    } else {
                        conn.received(buff, size);
                    }
                } catch (ClassNotFoundException e) {
                    ErrorListener el = (ErrorListener)listeners.get(protocol);
                    if (el != null)
                        el.receiveError(conn, e);
                    conn.receiveError(e);
                    return;
                } catch (IOException e) {
                    ErrorListener el = (ErrorListener)listeners.get(protocol);
                    if (el != null)
                        el.receiveError(conn, e);
                    conn.receiveError(e);
                    return;
                }
            }
        }
    }
}
