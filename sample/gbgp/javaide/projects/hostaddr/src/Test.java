
import java.io.*;
import java.net.*;

public class Test {
	public static void usage() {
		System.out.println("Usage:");
		System.out.println("getLocalHost");
		System.out.println("getByName host <host>");
		System.out.println("getAllByName host <host>");
		System.out.println("quit");
	}

	public static void main(String[] args) {
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		while (true) {
			System.out.print("> ");
			String s = null;
			try {
				s = in.readLine();
			} catch (IOException e) {
				e.printStackTrace();
				break;
			}
			if (s.equals("")) continue;
			StreamTokenizer st = new StreamTokenizer(new StringReader(s));
			int flag;
			try {
				flag = st.nextToken();
			} catch (IOException e) {
				e.printStackTrace();
				break;
			}
			if (flag == StreamTokenizer.TT_EOF || st.sval == null) {
				usage(); continue;
			}
			String cmd = st.sval;

			String host = null;
			boolean error = false;
			while (true) {
				try {
					flag = st.nextToken();
				} catch (IOException e) {
					e.printStackTrace();
					break;
				}
				if (flag == StreamTokenizer.TT_EOF) break;
				s = st.sval;
				if (s.equals("host")) {
					try {
						flag = st.nextToken();
					} catch (IOException e) {
						e.printStackTrace();
						break;
					}
					if (flag == StreamTokenizer.TT_EOF || st.sval == null) {
						error = true; break;
					}
					host = st.sval;
				}
			}
			if (error) { usage(); continue; }
			if (cmd.equals("quit")) {
				System.exit(0);
			} else if (cmd.equals("getLocalHost")) {
				InetAddress address = null;
				try {
					address = InetAddress.getLocalHost();
				} catch (UnknownHostException e) {
					e.printStackTrace();
				}
				System.out.println("hostname = " + address.getHostName());
				System.out.println("hostaddress = " + address.getHostAddress());
			} else if (cmd.equals("getByName")) {
				if (host == null) {
					usage(); continue;
				}
				InetAddress address = null;
				try {
					address = InetAddress.getByName(host);
				} catch (UnknownHostException e) {
					e.printStackTrace();
				}
				System.out.println("hostname = " + address.getHostName());
				System.out.println("hostaddress = " + address.getHostAddress());
			} else if (cmd.equals("getAllByName")) {
				if (host == null) {
					usage(); continue;
				}
				InetAddress address[] = null;
				try {
					address = InetAddress.getAllByName(host);
				} catch (UnknownHostException e) {
					e.printStackTrace();
				}
				for (int i = 0; i < address.length; i++) {
					System.out.println("hostname = " + address[i].getHostName());
					System.out.println("hostaddress = " + address[i].getHostAddress());
				}
			} else {
				usage(); continue;
			}
		}
    }
}
