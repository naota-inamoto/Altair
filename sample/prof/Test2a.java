
import javax.xml.transform.*;
import javax.xml.transform.sax.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.stream.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.ext.*;
import java.io.*;
import java.util.*;

public class Test2a implements ContentHandler {
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
        PrintWriter pw = new PrintWriter(new FileOutputStream("java_xml_parse2.txt"));
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

    void parse(String xml) throws Exception {
        String xsl = "<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>";
        xsl += "<xsl:template match=\"@*|*|text()|processing-instruction()\">";
        xsl += "<xsl:copy>";
        xsl += "<xsl:apply-templates select=\"@*|*|text()|processing-instruction()\"/>";
        xsl += "</xsl:copy>";
        xsl += "</xsl:template>";
        xsl += "</xsl:stylesheet>";
        TransformerFactory factory = TransformerFactory.newInstance();
        if (factory.getFeature(SAXSource.FEATURE)) {
            XMLReader reader = ((SAXTransformerFactory)factory).newXMLFilter(
                new StreamSource(new StringReader(xsl)));
            reader.setContentHandler(this);
            reader.parse(new InputSource(new StringReader(xml)));
        } else {
            throw new Exception("factory does not support SAX features!");
        }
    }

    public void startElement (String namespaceURI, String localName,
            String qName, Attributes attrs) throws SAXException {
    }
    public void endElement(String namespaceURI, String localName,
            String qName) throws SAXException {
    }
    public void characters (char ch[], int start, int length)
            throws SAXException {
    }

    public void startDocument () throws SAXException {}
    public void endDocument() throws SAXException {}
    public void ignorableWhitespace (char ch[], int start, int length) throws SAXException {}
    public void setDocumentLocator(Locator locator) {}
    public void startPrefixMapping(String prefix, String uri) throws SAXException {}
    public void endPrefixMapping(String prefix) throws SAXException {}
    public void processingInstruction(String target, String data) throws SAXException {}
    public void skippedEntity(String name) throws SAXException {}

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
