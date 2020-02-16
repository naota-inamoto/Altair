
package altair.net.act;

import java.lang.*;
import java.io.*;

import altair.net.*;

public class Action extends Message {
    public Action(String message) {
        super(message);
    }

    public void run(Connection conn) {
    }
}
