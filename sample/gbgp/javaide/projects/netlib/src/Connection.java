
package altair.net;

import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;

public interface Connection {
    public Socket getSocket();
    public void addListener(String protocol, ReceiveListener l);
    public void removeListener(String protocol);
    public void start();

    public void close();
    public void send(Message msg);
    public void send(String protocol, byte[] buff, int size);

    public void sendError(IOException e);
    public void receiveError(ClassNotFoundException e);
    public void receiveError(IOException e);

    public void received(Message msg);
    public void received(byte[] buff, int size);
}
