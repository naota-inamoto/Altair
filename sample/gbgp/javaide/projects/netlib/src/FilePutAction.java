
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FilePutAction extends Action {

    public FilePutAction(String protocol, String remotePath) {
        super(protocol);
        setArg(remotePath);
    }

    public void run(Connection conn) {
        String protocol = getMessage();
        String remotePath = getArg(0);
        FilePutServerAdapter fpa =
            new FilePutServerAdapter(protocol, remotePath);
        try {
            fpa.create();
        } catch (IOException e) {
            conn.send(new Message(protocol, "cannot create"));
            fpa.delete();
            return;
        }
        conn.addListener(protocol, fpa);
        conn.send(new Message(protocol, "req data"));
    }
}
