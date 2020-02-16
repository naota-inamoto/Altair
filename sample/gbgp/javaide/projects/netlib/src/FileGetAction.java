
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public class FileGetAction extends Action {

    public FileGetAction(String protocol, String remotePath) {
        super(protocol);
        setArg(remotePath);
    }

    public void run(Connection conn) {
        String protocol = getMessage();
        String remotePath = getArg(0);
        FileGetServerAdapter fga =
            new FileGetServerAdapter(protocol, remotePath);
        long size = fga.fileSize();
        try {
            fga.open();
        } catch (IOException e) {
            conn.send(new Message(protocol, "cannot open"));
            return;
        }
        conn.addListener(protocol, fga);
        Message msg = new Message(protocol, "opened");
        msg.setArg((new Long(size)).toString());
        conn.send(msg);
    }
}
