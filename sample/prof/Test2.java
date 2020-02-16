
import org.w3c.dom.*;
import org.xml.sax.*;
import javax.xml.parsers.*;
import java.io.*;
import java.util.*;

public class Test2 {
    SAXParseLogger logger;
    DocumentBuilderFactory factory;
    DocumentBuilder builder;
    int count;

    public static void main(String[] args) throws Exception  {
        File file = new File("test.xml");
        FileInputStream in = new FileInputStream(file);
        int len = (int)file.length();
        byte[] buf = new byte[len];
        in.read(buf);
        in.close();
        String xml = new String(buf, "SJIS");

        Test2 test = new Test2();
        test.init();
        PrintWriter pw = new PrintWriter(new FileOutputStream("java_xml_parse.txt"));
        long t1 = System.currentTimeMillis();
        test.test_parse(xml);
        long t2 = System.currentTimeMillis();
        pw.println("parse * " + test.count + " times: " + (t2-t1) + "msec");
        pw.close();
    }

    void init() throws Exception {
        factory = DocumentBuilderFactory.newInstance();
        factory.setValidating(false);
        builder = factory.newDocumentBuilder();
        logger = new SAXParseLogger();
        builder.setErrorHandler(logger);
        count = 100;
    }

    void test_parse(String xml) throws Exception {
        for (int i = 0; i < count; i++) {
            parse(xml);
        }
    }

    Document parse(String xml) throws Exception {
        InputSource source = new InputSource(new StringReader(xml));
        source.setSystemId("");
        logger.reset();
        Document doc = (Document)builder.parse(source);
        return doc;
    }

    class SAXParseLogger implements ErrorHandler {
        Vector errors = new Vector();
        public synchronized void warning(SAXParseException e) {
            errors.addElement(new SAXParseError("warning", e));
        }
        public synchronized void error(SAXParseException e) {
            errors.addElement(new SAXParseError("error", e));
        }
        public synchronized void fatalError(SAXParseException e) {
            errors.addElement(new SAXParseError("fatalError", e));
        }
        public Vector getErrors() {
            return (Vector)errors.clone();
        }
        public synchronized void reset() {
            errors = new Vector();
        }
    }

    class SAXParseError {
        String id;
        SAXParseException ex;
        SAXParseError(String id, SAXParseException ex) {
            this.id = id;
            this.ex = ex;
        }
    }
}
