
package altair.net;

import java.lang.*;
import java.io.*;

import altair.net.*;

public interface ReceiveListener extends ErrorListener {
    public void received(Connection conn, Message msg);
    public void received(Connection conn, byte[] buff, int size);
}
