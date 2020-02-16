
package altair.net.act;

import java.lang.*;
import java.io.*;
import java.net.*;

import altair.net.*;
import altair.net.act.*;

public class ActionServerConnection extends ServerConnection {
    public ActionServerConnection(ActionServerConnectionManager mgr, Socket s) {
        super(mgr, s);
    }

    public void received(Message msg) {
        if (msg instanceof Action) {
            Action action = (Action)msg;
            action.run(this);
            return;
        }
        super.received(msg);
    }
}
