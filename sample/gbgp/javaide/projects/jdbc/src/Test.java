
import java.lang.*;
import java.sql.*;

public class Test {
    public static void main(String argv[]) {
        if (argv.length != 1) {
            System.err.println("usage: java Sql \"sql-statement\"");
            System.exit(-1);
        }

        String driver = "oracle.jdbc.driver.OracleDriver";
        String url = "jdbc:oracle:thin:@vega:1521:v8i815";
        String user = "inamoto";
        String password = "chiaki";

        try {
            Class.forName(driver);
        } catch (ClassNotFoundException ex) {
            System.err.print(ex.getMessage());
            ex.printStackTrace();
            System.err.print("�h���C�o�̓o�^�G���[: driver = ");
            System.err.println(driver);
            System.exit(-1);
        }

        Connection conn = null;
        try {
            conn = DriverManager.getConnection(url, user, password);
        } catch (SQLException ex) {
            System.err.print(ex.getMessage());
            ex.printStackTrace();
            System.err.print("�f�[�^�x�[�X�ւ̐ڑ��G���[: url, user, password = ");
            System.err.print(url);
            System.err.print(", ");
            System.err.print(user);
            System.err.print(", ");
            System.err.println(password);
            System.exit(-1);
        }

        Statement stmt = null;
        try {
            stmt = conn.createStatement();
        } catch (SQLException ex) {
            System.err.print(ex.getMessage());
            ex.printStackTrace();
            System.err.println("Statement�̍쐬�Ɏ��s");
            System.exit(-1);
        }

        String s = argv[0];
        String sql = null;
        boolean search = s.startsWith("-s");
        if (search) {
            sql = s.substring(3);
        } else {
            sql = s;
        }
        ResultSet rs = null;
        try {
            rs = stmt.executeQuery(sql);
        } catch (SQLException ex) {
            System.err.print(ex.getMessage());
            ex.printStackTrace();
            System.err.print("SQL�̎��s�Ɏ��s: sql = ");
            System.err.println(sql);
            System.exit(-1);
        }
        try {
            if (search) {
                while (rs.next()) {
                    try {
                        for (int i = 1;; i++) {
                            Object o = rs.getObject(i);
                            System.out.print(o.toString() + "\t");
                        }
                    } catch (SQLException ex2) {
                        System.out.print("\n");
                    }
                }
            }
        } catch (SQLException ex) {
            System.err.print(ex.getMessage());
            ex.printStackTrace();
            System.err.println("ResultSet�̑���Ɏ��s");
            System.exit(-1);
        }
    }
};
