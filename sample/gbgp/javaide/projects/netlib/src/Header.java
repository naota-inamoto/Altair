
package altair.net;

import java.lang.*;
import java.io.*;

public class Header implements Serializable {
    public String prev;
    public String next;

    public Header() {
        prev = "holon";
        next = "altair";
    }
}
