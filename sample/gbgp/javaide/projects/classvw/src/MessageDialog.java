
/* $Id: MessageDialog.java,v 1.2 1999/01/29 11:18:18 inamoto Exp $ */

import javax.swing.JFrame;
import javax.swing.JPanel;

import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JButton;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.Dimension;
import java.awt.Point;

public class MessageDialog extends JDialog {
    public MessageDialog(JFrame f, String msg) {
        super(f, "", true);
        JLabel label = new JLabel(msg);

        JPanel panel = new JPanel();
        JButton ok = new JButton("OK");
        panel.setLayout(new FlowLayout());
        panel.add(ok);

        getContentPane().setLayout(new BorderLayout());
        getContentPane().add("Center", label);
        getContentPane().add("South", panel);

        setSize(new Dimension(300, 100));
        setLocation(new Point(100, 100));

        ok.addActionListener( new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                dispose();
            }
        } );
    }
};
