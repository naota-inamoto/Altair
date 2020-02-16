
/* $Id: HTreeView.java,v 1.2 1999/01/29 11:18:18 inamoto Exp $ */

import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import java.util.Hashtable;

public class HTreeView extends JTree {
    protected DefaultTreeModel model;
    protected Hashtable hashtable;

    public HTreeView() {
        this(new DefaultTreeModel(new DefaultMutableTreeNode("")));
    }
    protected HTreeView(DefaultTreeModel model) {
        super(model);
        this.model = model;
        hashtable = new Hashtable();
    }
    protected DefaultMutableTreeNode createNode(String name) {
        DefaultMutableTreeNode node = new DefaultMutableTreeNode(name);
        hashtable.put(name, node);
        return node;
    }

    public DefaultMutableTreeNode getSelectedNode() {
        TreePath selPath = getSelectionPath();
        if (selPath != null)
            return (DefaultMutableTreeNode)selPath.getLastPathComponent();
        return null;
    }
    public DefaultMutableTreeNode getRootNode() {
        return (DefaultMutableTreeNode)model.getRoot();
    }

    public String getName(DefaultMutableTreeNode node) {
        return node.toString();
    }
    public DefaultMutableTreeNode getParent(DefaultMutableTreeNode node) {
        return (DefaultMutableTreeNode)node.getParent();
    }
    public DefaultMutableTreeNode getNext(DefaultMutableTreeNode node) {
        return (DefaultMutableTreeNode)node.getNextNode();
    }
    public DefaultMutableTreeNode getPrev(DefaultMutableTreeNode node) {
        return (DefaultMutableTreeNode)node.getPreviousNode();
    }
    public DefaultMutableTreeNode getFirstChild(DefaultMutableTreeNode node) {
        int count = node.getChildCount();
        if (count == 0)
            return null;
        return (DefaultMutableTreeNode)node.getFirstChild();
    }
    public DefaultMutableTreeNode getLastChild(DefaultMutableTreeNode node) {
        int count = node.getChildCount();
        if (count == 0)
            return null;
        return (DefaultMutableTreeNode)node.getLastChild();
    }

    public DefaultMutableTreeNode createChild(DefaultMutableTreeNode node, String name) {
        int index = node.getChildCount();
        DefaultMutableTreeNode n = createNode(name);
        model.insertNodeInto(n, node, index);
        return n;
    }
    public DefaultMutableTreeNode insert(DefaultMutableTreeNode node, String name) {
        DefaultMutableTreeNode parent = getParent(node);
        if (parent == null)
            return null;
        int index = parent.getIndex(node);
        DefaultMutableTreeNode n = createNode(name);
        model.insertNodeInto(n, parent, index);
        return n;
    }
    public DefaultMutableTreeNode append(DefaultMutableTreeNode node, String name) {
        DefaultMutableTreeNode parent = getParent(node);
        if (parent == null)
            return null;
        int index = parent.getIndex(node) + 1;
        DefaultMutableTreeNode n = createNode(name);
        model.insertNodeInto(n, parent, index);
        return n;
    }
    public void remove(DefaultMutableTreeNode node) {
        hashtable.remove(getName(node));
        model.removeNodeFromParent(node);
    }
    public void removeChildren(DefaultMutableTreeNode node) {
        DefaultMutableTreeNode n;
        while ((n = getFirstChild(node)) != null) {
            remove(n);
        }
    }

    public boolean isExpanded(DefaultMutableTreeNode node) {
        TreePath path = getPathFromNode(node);
        return super.isExpanded(path);
    }
    public boolean isCollapsed(DefaultMutableTreeNode node) {
        TreePath path = getPathFromNode(node);
        return super.isCollapsed(path);
    }
    public void expand(DefaultMutableTreeNode node) {
        TreePath path = getPathFromNode(node);
        expandPath(path);
    }
    public void collapse(DefaultMutableTreeNode node) {
        TreePath path = getPathFromNode(node);
        collapsePath(path);
    }
    public TreePath getPathFromNode(DefaultMutableTreeNode node) {
        return new TreePath(model.getPathToRoot(node));
    }

    public DefaultMutableTreeNode getNodeFromName(String name) {
        return (DefaultMutableTreeNode)hashtable.get(name);
    }
    public DefaultMutableTreeNode getNodeFromName1(DefaultMutableTreeNode node, String name) {
        DefaultMutableTreeNode n1 = getFirstChild(node);
        while (n1 != null) {
            String name1 = getName(n1);
            if (name1.equals(name))
                return n1;
            n1 = getNext(n1);
        }
        return null;
    }

    public void select(DefaultMutableTreeNode node) {
        setSelectionPath(getPathFromNode(node));
    }
};
