
/* $Id: AskDialog.java,v 1.2 1999/01/29 11:18:18 inamoto Exp $ */

import javax.swing.JFrame;
import javax.swing.JPanel;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.JTextField;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Dimension;
import java.awt.Point;

public class AskDialog extends JDialog {
    boolean flag;
    JTextField textField;

    public AskDialog(JFrame f, String msg, String str)
    {
        super(f, "ClassView", true);

        JPanel panel1 = new JPanel();
        JLabel label = new JLabel(msg);
        textField = new JTextField(str);
        panel1.setLayout(new BorderLayout());
        panel1.add("Center", label);
        panel1.add("South", textField);

        JPanel panel2 = new JPanel();
        JButton ok = new JButton("OK");
        JButton cancel = new JButton("Cancel");
        panel2.setLayout(new FlowLayout());
        panel2.add(ok);
        panel2.add(cancel);

        getContentPane().setLayout(new BorderLayout());
        getContentPane().add("Center", panel1);
        getContentPane().add("South", panel2);

        setSize(new Dimension(300, 100));
        setLocation(new Point(100, 100));

        ok.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                flag = true;
                setVisible(false);
            }
        } );

        cancel.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                flag = false;
                setVisible(false);
            }
        } );
    }
    public boolean isOK() { return flag; }
    public String getName() { return textField.getText(); }
};
