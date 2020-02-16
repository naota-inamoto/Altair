
package altair.net.act;

import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;
import altair.net.act.*;

public class ActionServerConnectionManager extends ServerConnectionManager {
    public ActionServerConnectionManager(int port) throws IOException {
        super(port);
    }

    public ServerConnection createServerConnection(Socket s) {
        ActionServerConnection conn = new ActionServerConnection(this, s);
        conn.start();
        return conn;
    }
}
