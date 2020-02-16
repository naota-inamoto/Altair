
package altair.net;

import java.lang.*;
import java.io.*;

import altair.net.*;

public interface ErrorListener {
    public void sendError(Connection conn, IOException e);
    public void receiveError(Connection conn, ClassNotFoundException e);
    public void receiveError(Connection conn, IOException e);
}
