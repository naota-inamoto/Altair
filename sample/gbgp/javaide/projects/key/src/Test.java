
import java.io.*;
import java.util.*;

import java.security.*;
import java.security.spec.*;

public class Test {
	static final byte[] SEED = {2, 0, 0, 1, 0, 1, 1, 3};

	static byte[] private_key;
	static byte[] public_key;

	static String object = "inamoto";
	static SignedObject signedObject;

	public static void main(String[] args) {
		try {
			generate_key_pair();
			sign();
			verify();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void generate_key_pair() throws Exception {
		System.out.println((new Date()).toString() + " start generate_key_pair");

		KeyPairGenerator keyGen = KeyPairGenerator.getInstance("DSA", "SUN");

		SecureRandom random = SecureRandom.getInstance("SHA1PRNG", "SUN");
		random.setSeed(System.currentTimeMillis());
		keyGen.initialize(1024, random);

		KeyPair pair = keyGen.generateKeyPair();
		PrivateKey privateKey = pair.getPrivate();
		PublicKey publicKey = pair.getPublic();

		private_key = privateKey.getEncoded();
		public_key = publicKey.getEncoded();

		System.out.println("key pair generated.");
		System.out.println("private key length = " + private_key.length);
		System.out.println("public key length = " + public_key.length);

		System.out.println((new Date()).toString() + " end generate_key_pair");
	}

	public static void sign() throws Exception {
		System.out.println((new Date()).toString() + " start sign");

		PKCS8EncodedKeySpec privateKeySpec = new PKCS8EncodedKeySpec(private_key);
		KeyFactory keyFactory = KeyFactory.getInstance("DSA");
		PrivateKey privateKey = keyFactory.generatePrivate(privateKeySpec);

		Signature signingEngine = Signature.getInstance("SHA-1/DSA", "SUN");

		signedObject = new SignedObject(object, privateKey, signingEngine);

		System.out.println("content is signed.");

		System.out.println((new Date()).toString() + " end sign");
	}

	public static void verify() throws Exception {
		System.out.println((new Date()).toString() + " start verify");

		X509EncodedKeySpec publicKeySpec = new X509EncodedKeySpec(public_key);
		KeyFactory keyFactory = KeyFactory.getInstance("DSA");
		PublicKey publicKey = keyFactory.generatePublic(publicKeySpec);

		Signature verifyEngine = Signature.getInstance("SHA-1/DSA");

		boolean b = signedObject.verify(publicKey, verifyEngine);

		if (b) {
			System.out.println("signature verification OK.");
		} else {
			System.out.println("signature verification NG.");
		}

		System.out.println((new Date()).toString() + " end verify");
	}
}
