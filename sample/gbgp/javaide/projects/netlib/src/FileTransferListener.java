
package altair.net.ft;

import java.lang.*;
import java.io.*;

import altair.net.*;
import altair.net.act.*;
import altair.net.ft.*;

public interface FileTransferListener {
    public void completed();
    public void canceled();
    public void totalFileSize(long size);
    public void transfered(long size);
    public void cannotCreateFile(String path);
    public void cannotOpenFile(String path);
    public void ioError();
}
