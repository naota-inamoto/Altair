
/* $Id: HClassView.java,v 1.2 1999/01/29 11:18:18 inamoto Exp $ */

import javax.swing.UIManager;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import HTreeView;
import javax.swing.tree.DefaultMutableTreeNode;

import MessageDialog;
import AskDialog;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FileDialog;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import java.io.File;
import java.io.FileInputStream;
import java.io.DataInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class HClassView {
    protected JFrame frame;
    protected boolean app;
    protected HTreeView tree;

    protected DefaultMutableTreeNode find_node;
    protected String find_string;

    public HClassView(boolean ap) {
        try {
	    UIManager.setLookAndFeel("javax.swing.plaf.windows.WindowsLookAndFeel");
	} catch(Exception ex) {
        }

        app = ap;
        tree = new HTreeView();

        find_node = tree.getRootNode();
        find_string = null;

        JScrollPane sp = new JScrollPane();
        sp.setPreferredSize(new Dimension(400, 500));
        sp.getViewport().add(tree);

        JPanel panel = new JPanel(true);
        panel.setLayout(new BorderLayout());
        panel.add("Center", sp);

        frame = new JFrame("ClassBrowser");
        frame.getContentPane().add("Center", panel);

        JMenuBar menuBar = new JMenuBar();
        JMenu menu;
        JMenuItem menuItem;

        menu = new JMenu("File");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Java Core Package"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (load(".", "java.txt") == false) {
                    messageDialog("loadに失敗しました。");
                    return;
                }
                tree.expand(tree.getRootNode());
            }
        } );
        menuItem = menu.add(new JMenuItem("Swing (JFC) Package"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (load(".", "swing.txt") == false) {
                    messageDialog("loadに失敗しました。");
                    return;
                }
                tree.expand(tree.getRootNode());
            }
        } );
        menuItem = menu.add(new JMenuItem("Load"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                FileDialog dialog = new FileDialog(frame, "class list file", FileDialog.LOAD);
                dialog.show();
                String dir = dialog.getDirectory();
                String name = dialog.getFile();
                if (dir == null || name == null) return;
                if (load(dir, name) == false) {
                    messageDialog("loadに失敗しました。");
                    return;
                }
                int テスト = 1;
                tree.expand(tree.getRootNode());
            }
        } );
        menuItem = menu.add(new JMenuItem("Exit"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (app) {
                    System.exit(0);
                } else {
                    frame.dispose();
                }
            }
        } );

        menu = new JMenu("Edit");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Find"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                AskDialog dialog = new AskDialog(frame, "Input Search String", "");
                dialog.show();
                if (dialog.isOK()) {
                    find_string = dialog.getName();
                    dialog.dispose();
                } else {
                    dialog.dispose();
                    return;
                }
                for (find_node = tree.getRootNode();
                        find_node != null; find_node = tree.getNext(find_node)) {
                    String str = find_node.toString();
                    int index = str.indexOf(find_string);
                    if (index >= 0)
                        break;
                }
                if (find_node != null) {
                    tree.select(find_node);
                } else {
                    messageDialog("最後まで検索しました。");
                    find_node = tree.getRootNode();
                }
            }
        } );
        menuItem = menu.add(new JMenuItem("Find Next"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (find_string == null) {
                    messageDialog("Findを先に行なってください。");
                    return;
                }
                for (find_node = tree.getNext(find_node);
                        find_node != null; find_node = tree.getNext(find_node)) {
                    String str = find_node.toString();
                    int index = str.indexOf(find_string);
                    if (index >= 0)
                        break;
                }
                if (find_node != null) {
                    tree.select(find_node);
                } else {
                    messageDialog("最後まで検索しました。");
                    find_node = tree.getRootNode();
                }
            }
        } );
        menuItem = menu.add(new JMenuItem("Copy"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                // TODO:
                messageDialog("not implement yet");
            }
        } );

        menu = new JMenu("View");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Show Member Info"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                DefaultMutableTreeNode node = tree.getSelectedNode();
                if (node == null)
                    return;
                new HMemberView(tree.getName(node));
            }
        } );
        menuItem = menu.add(new JMenuItem("Show Class Info"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                DefaultMutableTreeNode node = tree.getSelectedNode();
                if (node == null)
                    return;
                new HClassInfoView(tree.getName(node));
            }
        } );
        menuItem = menu.add(new JMenuItem("Add Class"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                AskDialog dialog = new AskDialog(frame, "Input ClassName", "");
                dialog.show();
                String name;
                if (dialog.isOK()) {
                    name = dialog.getName();
                    dialog.dispose();
                } else {
                    dialog.dispose();
                    return;
                }
                if (addClass(name) == null) {
                    messageDialog("クラスが見つかりません。");
                }
            }
        } );

        menu = new JMenu("Test");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Test"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                // ### TODO:
                messageDialog("Test");
            }
        } );
        frame.setJMenuBar(menuBar);

        frame.addWindowListener( new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                if (app) {
                    System.exit(0);
                } else {
                    frame.dispose();
                }
            }
        } );

        frame.pack();
        frame.show();
    }

    public static void main(String argv[]) {
        new HClassView(true);
    }
    public void messageDialog(String msg) {
        MessageDialog dialog = new MessageDialog(frame, msg);
        dialog.show();
    }

    public DefaultMutableTreeNode addClass(String name) {
        DefaultMutableTreeNode node = tree.getNodeFromName(name);
        if (node != null) {
            tree.expand(node);
            return node;
        }
        HApiSearcher search = new HApiSearcher();
        if (search.addClass(name) == false)
            return null;
        search.selectClass(name);
        String sname = search.superclassName();
        if (sname == null) {
            node = tree.getRootNode();
        } else {
            node = addClass(sname);
        }
        DefaultMutableTreeNode node2 = tree.createChild(node, name);
        tree.expand(node);
        tree.select(node2);
        return node2;
    }

    public boolean load(String dir, String name) {
        File f = new File(dir, name);
        DataInputStream is;
        try {
            is = new DataInputStream(new FileInputStream(f));
        } catch (FileNotFoundException ex) {
            return false;
        }
        return load1(tree.getRootNode(), is);
    }
    public boolean load1(DefaultMutableTreeNode node, DataInputStream is) {
        DefaultMutableTreeNode node2 = null;
        try {
            byte a[] = new byte[1024];
            while (true) {
                int len = 0;
                byte b = is.readByte();
                if (b == ' ' || b == '\t' || b == '\n' || b == '\r')
                    continue;
                while (true) {
                    a[len++] = b;
                    b = is.readByte();
                    if (b == ' ' || b == '\t' || b == '\n' || b == '\r')
                        break;
                }
                String str = new String(a, 0, len);
                if (str.equals("{")) {
                    if (node2 == null)
                        return false;
                    if (load1(node2, is) == false)
                        return false;
                } else if (str.equals("}")) {
                    return true;
                } else {
                    node2 = tree.getNodeFromName1(node, str);
                    if (node2 == null)
                        node2 = tree.createChild(node, str);
                }
            }
        } catch (IOException ex) {
            return true;
        }
    }
};

class HMemberView {
    protected JFrame frame;
    protected HTreeView tree;
    protected String clsName;

    protected DefaultMutableTreeNode find_node;
    protected String find_string;

    protected boolean declared_only;
    protected boolean show_public;
    protected boolean show_protected;
    protected boolean show_private;
    protected boolean show_not_static;
    protected boolean show_static;
    protected boolean show_field;
    protected boolean show_constructor;
    protected boolean show_method;
    protected boolean show_class;

    public HMemberView(String className) {
        clsName = className;

        declared_only = true;
        show_public = true;
        show_protected = true;
        show_private = true;
        show_not_static = true;
        show_static = true;
        show_field = true;
        show_constructor = true;
        show_method = true;
        show_class = true;

        tree = new HTreeView();

        find_node = tree.getRootNode();
        find_string = null;

        JScrollPane sp = new JScrollPane();
        sp.setPreferredSize(new Dimension(400, 500));
        sp.getViewport().add(tree);

        JPanel panel = new JPanel(true);
        panel.setLayout(new BorderLayout());
        panel.add("Center", sp);

        frame = new JFrame(className);
        frame.getContentPane().add("Center", panel);

        JMenuBar menuBar = new JMenuBar();
        JMenu menu, menu2;
        JMenuItem menuItem;

        menu = new JMenu("File");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Close"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                frame.dispose();
            }
        } );

        menu = new JMenu("Edit");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Find"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                AskDialog dialog = new AskDialog(frame, "Input Search String", "");
                dialog.show();
                if (dialog.isOK()) {
                    find_string = dialog.getName();
                    dialog.dispose();
                } else {
                    dialog.dispose();
                    return;
                }
                for (find_node = tree.getRootNode();
                        find_node != null; find_node = tree.getNext(find_node)) {
                    String str = find_node.toString();
                    int index = str.indexOf(find_string);
                    if (index >= 0)
                        break;
                }
                if (find_node != null) {
                    tree.select(find_node);
                } else {
                    messageDialog("最後まで検索しました。");
                    find_node = tree.getRootNode();
                }
            }
        } );
        menuItem = menu.add(new JMenuItem("Find Next"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (find_string == null) {
                    messageDialog("Findを先に行なってください。");
                    return;
                }
                for (find_node = tree.getNext(find_node);
                        find_node != null; find_node = tree.getNext(find_node)) {
                    String str = find_node.toString();
                    int index = str.indexOf(find_string);
                    if (index >= 0)
                        break;
                }
                if (find_node != null) {
                    tree.select(find_node);
                } else {
                    messageDialog("最後まで検索しました。");
                    find_node = tree.getRootNode();
                }
            }
        } );

        menu = new JMenu("View");
        menuBar.add(menu);

        menu.add(menu2 = new JMenu("Declared"));
        menuItem = menu2.add(new JMenuItem("Declared only (default)"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                declared_only = true;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("All"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                declared_only = false;
                showInfo();
            }
        } );
        
        menu.add(menu2 = new JMenu("Protection"));
        menuItem = menu2.add(new JMenuItem("public only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_public = true;
                show_protected = false;
                show_private = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("protected only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_public = false;
                show_protected = true;
                show_private = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("public, protected only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_public = true;
                show_protected = true;
                show_private = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("All (default)"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_public = true;
                show_protected = true;
                show_private = true;
                showInfo();
            }
        } );
        
        menu.add(menu2 = new JMenu("Static"));
        menuItem = menu2.add(new JMenuItem("not static only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_static = false;
                show_not_static = true;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("static only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_static = true;
                show_not_static = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("All (default)"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_static = true;
                show_not_static = true;
                showInfo();
            }
        } );

        menu.add(menu2 = new JMenu("Kind"));
        menuItem = menu2.add(new JMenuItem("field only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_field = true;
                show_constructor = false;
                show_method = false;
                show_class = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("constructor only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_field = false;
                show_constructor = true;
                show_method = false;
                show_class = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("method only"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_field = false;
                show_constructor = false;
                show_method = true;
                show_class = false;
                showInfo();
            }
        } );
        menuItem = menu2.add(new JMenuItem("All (default)"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                show_field = true;
                show_constructor = true;
                show_method = true;
                show_class = true;
                showInfo();
            }
        } );

        menuItem = menu.add(new JMenuItem("Jump to Source"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                messageDialog("not implement yet");
            }
        } );
        frame.setJMenuBar(menuBar);

        frame.addWindowListener( new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                frame.dispose();
            }
        } );

        showInfo();

        frame.pack();
        frame.show();
    }

    public void messageDialog(String msg) {
        MessageDialog dialog = new MessageDialog(frame, msg);
        dialog.show();
    }

    public void showInfo() {
        HApiSearcher search = new HApiSearcher();
        search.addClass(clsName);
        search.selectClass(clsName);

        DefaultMutableTreeNode root = tree.getRootNode();
        tree.removeChildren(root);

        boolean result, result2, first;

      if (declared_only) {
        // field
        if (show_field) for (result = search.searchFirstDeclaredField();
                result;
                result = search.searchNextDeclaredField()) {
            String str = "field: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            } else {
                if (!show_private)
                    continue;
            }
            if (search.isStatic()) {
                if (!show_static)
                    continue;
                str += "static ";
            } else {
                if (!show_not_static)
                    continue;
            }
            if (search.isFinal()) {
                str += "final ";
            }
            if (search.isTransient()) {
                str += "transient ";
            }
            if (search.isVolatile()) {
                str += "volatile ";
            }
            str += search.fieldTypeName();
            str += " ";
            str += search.fieldName();
            int ad = search.fieldArrayDimension();
            for (int i = 0; i < ad; i++) {
                str += "[]";
            }
            str += ";";
            tree.createChild(root, str);
        }

        // constructor
        if (show_constructor) for (result = search.searchFirstDeclaredConstructor();
                result;
                result = search.searchNextDeclaredConstructor()) {
            String str = "constructor: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            } else {
                if (!show_private)
                    continue;
            }
            if (!show_not_static)
                continue;
            str += search.constructorName();
            str += "(";
            result2 = search.searchFirstArgument();
            first = true;
            while (result2) {
                if (first) {
                    first = false;
                } else {
                    str += ", ";
                }
                str += search.argTypeName();
                int ad = search.argArrayDimension();
                for (int i = 0; i < ad; i++) {
                    str += "[]";
                }
                result2 = search.searchNextArgument();
            }
            str += ");";
            tree.createChild(root, str);
        }

        // method
        if (show_method) for (result = search.searchFirstDeclaredMethod();
                result;
                result= search.searchNextDeclaredMethod()) {
            String str = "method: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            } else {
                if (!show_private)
                    continue;
            }
            if (search.isAbstract()) {
                str += "abstract ";
            }
            if (search.isStatic()) {
                if (!show_static)
                    continue;
                str += "static ";
            } else {
                if (!show_not_static)
                    continue;
            }
            if (search.isFinal()) {
                str += "final ";
            }
            if (search.isSynchronized()) {
                str += "synchronized ";
            }
            if (search.isNative()) {
                str += "native ";
            }
            str += search.methodReturnTypeName();
            str += " ";
            str += search.methodName();
            str += "(";
            result2 = search.searchFirstArgument();
            first = true;
            while (result2) {
                if (first) {
                    first = false;
                } else {
                    str += ", ";
                }
                str += search.argTypeName();
                int ad = search.argArrayDimension();
                for (int i = 0; i < ad; i++) {
                    str += "[]";
                }
                result2 = search.searchNextArgument();
            }
            str += ")";
            int ad = search.methodReturnTypeArrayDimension();
            for (int i = 0; i < ad; i++) {
                str += "[]";
            }
            str += ";";
            tree.createChild(root, str);
        }

        // class
        if (show_class) for (result = search.searchFirstDeclaredClass();
                result;
                result = search.searchNextDeclaredClass()) {
            String str = "inner class: ";
            if (search.isPublic()) {
                str += "public ";
            }
            if (search.isAbstract()) {
                str += "abstract ";
            }
            if (search.isFinal()) {
                str += "final ";
            }
            str += search.className();
            str += ";";
            tree.createChild(root, str);
        }

      } else { // if (declared_only)
        // field
        if (show_field) for (result = search.searchFirstField();
                result;
                result = search.searchNextField()) {
            String str = "field: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            }
            if (search.isStatic()) {
                if (!show_static)
                    continue;
                str += "static ";
            } else {
                if (!show_not_static)
                    continue;
            }
            if (search.isFinal()) {
                str += "final ";
            }
            if (search.isTransient()) {
                str += "transient ";
            }
            if (search.isVolatile()) {
                str += "volatile ";
            }
            str += search.fieldTypeName();
            str += " ";
            str += search.fieldName();
            int ad = search.fieldArrayDimension();
            for (int i = 0; i < ad; i++) {
                str += "[]";
            }
            str += ";";
            tree.createChild(root, str);
        }

        // constructor
        if (show_constructor) for (result = search.searchFirstConstructor();
                result;
                result = search.searchNextConstructor()) {
            String str = "constructor: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            }
            if (!show_not_static)
                continue;
            str += search.constructorName();
            str += "(";
            result2 = search.searchFirstArgument();
            first = true;
            while (result2) {
                if (first) {
                    first = false;
                } else {
                    str += ", ";
                }
                str += search.argTypeName();
                int ad = search.argArrayDimension();
                for (int i = 0; i < ad; i++) {
                    str += "[]";
                }
                result2 = search.searchNextArgument();
            }
            str += ");";
            tree.createChild(root, str);
        }

        // method
        if (show_method) for (result = search.searchFirstMethod();
                result;
                result= search.searchNextMethod()) {
            String str = "method: ";
            if (search.isPublic()) {
                if (!show_public)
                    continue;
                str += "public ";
            } else if (search.isProtected()) {
                if (!show_protected)
                    continue;
                str += "protected ";
            } else if (search.isPrivate()) {
                if (!show_private)
                    continue;
                str += "private ";
            }
            if (search.isAbstract()) {
                str += "abstract ";
            }
            if (search.isStatic()) {
                if (!show_static)
                    continue;
                str += "static ";
            } else {
                if (!show_not_static)
                    continue;
            }
            if (search.isFinal()) {
                str += "final ";
            }
            if (search.isSynchronized()) {
                str += "synchronized ";
            }
            if (search.isNative()) {
                str += "native ";
            }
            str += search.methodReturnTypeName();
            str += " ";
            str += search.methodName();
            str += "(";
            result2 = search.searchFirstArgument();
            first = true;
            while (result2) {
                if (first) {
                    first = false;
                } else {
                    str += ", ";
                }
                str += search.argTypeName();
                int ad = search.argArrayDimension();
                for (int i = 0; i < ad; i++) {
                    str += "[]";
                }
                result2 = search.searchNextArgument();
            }
            str += ")";
            int ad = search.methodReturnTypeArrayDimension();
            for (int i = 0; i < ad; i++) {
                str += "[]";
            }
            str += ";";
            tree.createChild(root, str);
        }

        // class
        if (show_class) for (result = search.searchFirstClass();
                result;
                result = search.searchNextClass()) {
            String str = "inner class: ";
            if (search.isPublic()) {
                str += "public ";
            }
            if (search.isAbstract()) {
                str += "abstract ";
            }
            if (search.isFinal()) {
                str += "final ";
            }
            str += search.className();
            str += ";";
            tree.createChild(root, str);
        }
      } // if (declared_only)

        tree.expand(root);
    }
};

class HClassInfoView {
    protected JFrame frame;
    protected HTreeView tree;
    protected String clsName;

    public HClassInfoView(String className) {
        clsName = className;

        tree = new HTreeView();

        JScrollPane sp = new JScrollPane();
        sp.setPreferredSize(new Dimension(400, 500));
        sp.getViewport().add(tree);

        JPanel panel = new JPanel(true);
        panel.setLayout(new BorderLayout());
        panel.add("Center", sp);

        frame = new JFrame(className);
        frame.getContentPane().add("Center", panel);

        JMenuBar menuBar = new JMenuBar();
        JMenu menu, menu2;
        JMenuItem menuItem;
        menu = new JMenu("File");
        menuBar.add(menu);
        menuItem = menu.add(new JMenuItem("Close"));
        menuItem.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                frame.dispose();
            }
        } );
        frame.setJMenuBar(menuBar);

        frame.addWindowListener( new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                frame.dispose();
            }
        } );

        showInfo();

        frame.pack();
        frame.show();
    }
    public void showInfo() {
        HApiSearcher search = new HApiSearcher();
        search.addClass(clsName);
        search.selectClass(clsName);

        DefaultMutableTreeNode root = tree.getRootNode();
        tree.removeChildren(root);

        String str;

        // class or interface
        if (search.isInterface())
            str = "This is interface";
        else
            str = "This is class";
        tree.createChild(root, str);

        // modifier
        str = "modifier: ";
        if (search.isPublic())
            str += "public ";
        if (search.isAbstract())
            str += "abstract ";
        if (search.isFinal())
            str += "final ";
        tree.createChild(root, str);

        // super class
        str = "super class: ";
        if (search.superclass() != null)
            str += search.superclassName();
        tree.createChild(root, str);

        // interface
        for (boolean result = search.searchFirstInterface();
                result;
                result = search.searchNextInterface()) {
            str = "interface: ";
            str += search.interfaceTypeName();
            tree.createChild(root, str);
        }
        tree.expand(root);
    }
};
