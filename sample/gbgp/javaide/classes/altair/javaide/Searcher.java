
package altair.javaide;

import java.lang.*;
import java.lang.reflect.*;
import java.beans.*;
import java.io.*;
import java.util.zip.*;

public class Searcher {
    private static final int CLASS = 1;
    private static final int ADD = 2;
    private static final int CLASSINFO = 3;
    private static final int MEMBER = 4;
    private static final int BEAN = 5;

    private static final int ALL = 1;
    private static final int DECLARED = 2;
    private static final int PUBLIC = 3;
    private static final int PUBLIC_PROTECTED = 4;
    private static final int NOT_STATIC = 5;
    private static final int STATIC = 6;
    private static final int FIELD = 7;
    private static final int CONSTRUCTOR = 8;
    private static final int METHOD = 9;
    private static final int PROPERTY = 10;
    private static final int EVENT = 11;

    private int cmd = 0;
    private String error = null;

    private String archiveName = null;
    private String className = null;
    private String prefix = null;

    private int declared = DECLARED;
    private int protection = ALL;
    private int function = ALL;
    private int kind = ALL;
    private int modif = ALL;

    public Searcher() {
    }

    public static void main(String argv[]) {
        Searcher s = new Searcher();
        if (s.option(argv) == false) {
            System.out.println("error: command syntax error");
            s.printUsage();
            System.exit(-1);
        }
        int c = s.cmd;
        if (c == CLASS) {
            s.printClassInfo();
        } else if (c == ADD) {
            s.printAddClassInfo();
        } else if (c == CLASSINFO) {
            s.printClassInfoInfo();
        } else if (c == MEMBER) {
            s.printMemberInfo();
        } else if (c == BEAN) {
            s.printBeanInfo();
        }
        String e = s.error;
        if (e != null) {
            System.out.println(e);
            System.exit(-1);
        } else {
            System.out.println("end:");
            System.exit(0);
        }
    }

    private void printUsage() {
        System.out.println("Usage: java altair.javaide.Searcher class <archivename> [startsWith <prefix>]");
        System.out.println("Usage; java altair.javaide.Searcher add <classname>");
        System.out.println("Usage; java altair.javaide.Searcher classinfo <classname>");
        System.out.println("Usage: java altair.javaide.Searcher member <classname>");
        System.out.println("    [declared { declared | all }]");
        System.out.println("    [protection { all | public | public_protected }]");
        System.out.println("    [function { all | not_static | static }]");
        System.out.println("    [kind { all | field | constructor | method }]");
        System.out.println("Usage: java altair.javaide.Searcher bean <classname>");
        System.out.println("    kind { all | method | property | event }");
    }

    private boolean option(String argv[]) {
        int len = argv.length;
        int i = 0;
        if (len == 0) return false;
        String s = argv[i++];
        if (s.equals("class")) {
            cmd = CLASS;
            if (len < 2) return false;
            archiveName = argv[i++];
            for (;;) {
                if (i == len) return true;
                if (i+1 == len) return false;
                s = argv[i++];
                if (s.equals("startsWith")) {
                    prefix = argv[i++];
                } else {
                    return false;
                }
            }
        } else if (s.equals("add")) {
            cmd = ADD;
            if (len != 2) return false;
            className = argv[i++];
        } else if (s.equals("classinfo")) {
            cmd = CLASSINFO;
            if (len != 2) return false;
            className = argv[i++];
        } else if (s.equals("member")) {
            cmd = MEMBER;
            if (len < 2) return false;
            className = argv[i++];
            for (;;) {
                if (i == len) return true;
                if (i+1 == len) return false;
                s = argv[i++];
                if (s.equals("declared")) {
                    s = argv[i++];
                    if (s.equals("all")) {
                        declared = ALL;
                    } else if (s.equals("declared")) {
                        declared = DECLARED;
                    } else {
                        return false;
                    }
                } else if (s.equals("protection")) {
                    s = argv[i++];
                    if (s.equals("all")) {
                        protection = ALL;
                    } else if (s.equals("public")) {
                        protection = PUBLIC;
                    } else if (s.equals("public_protected")) {
                        protection = PUBLIC_PROTECTED;
                    } else {
                        return false;
                    }
                } else if (s.equals("function")) {
                    s = argv[i++];
                    if (s.equals("all")) {
                        function = ALL;
                    } else if (s.equals("not_static")) {
                        function = NOT_STATIC;
                    } else if (s.equals("static")) {
                        function = STATIC;
                    } else {
                        return false;
                    }
                } else if (s.equals("kind")) {
                    s = argv[i++];
                    if (s.equals("all")) {
                        kind = ALL;
                    } else if (s.equals("field")) {
                        kind = FIELD;
                    } else if (s.equals("constructor")) {
                        kind = CONSTRUCTOR;
                    } else if (s.equals("method")) {
                        kind = METHOD;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        } else if (s.equals("bean")) {
            cmd = BEAN;
            if (len < 2) return false;
            className = argv[i++];
            for (;;) {
                if (i == len) return true;
                if (i+1 == len) return false;
                s = argv[i++];
                if (s.equals("kind")) {
                    s = argv[i++];
                    if (s.equals("all")) {
                        kind = ALL;
                    } else if (s.equals("method")) {
                        kind = METHOD;
                    } else if (s.equals("property")) {
                        kind = PROPERTY;
                    } else if (s.equals("event")) {
                        kind = EVENT;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
        return true;
    }

    private void printClassInfo() {
        FileInputStream fis = null;
        try {
            fis = new FileInputStream(archiveName);
        } catch (FileNotFoundException ex) {
            error = "error: file " + archiveName + " not found.";
            return;
        }
        ZipInputStream zis = new ZipInputStream(fis);
        ZipEntry entry; String name;
        try {
            while ((entry = zis.getNextEntry()) != null) {
                if (entry.isDirectory()) continue;
                name = entry.getName();
                if (!name.endsWith(".class")) continue;
                StringBuffer sb = new StringBuffer();
                for (int i = 0;; i++) {
                    char c = name.charAt(i);
                    if (c == '.') break;
                    if (c == '/') sb.append('.');
                    else sb.append(c);
                }
                name = new String(sb);
                if (prefix != null && !name.startsWith(prefix)) continue;
                Class cls;
                try {
                    cls = Class.forName(name);
                    modif = cls.getModifiers();
                    if (!isPublic()) continue;
                    printSuperClasses(cls, true);
                // } catch (ClassNotFoundException ex) {
                // } catch (NoSuchFieldError ex) {
                } catch (Throwable ex) {
                }
            }
        } catch (IOException ex) {
            error = "error: IOException.";
            return;
        }
    }

    private void printSuperClasses(Class c, boolean term) {
        Class s = c.getSuperclass();
        if (s != null) printSuperClasses(s, false);
        if (term) {
            if (!c.isInterface()) {
                System.out.print("class ");
            } else {
                System.out.print("interface ");
            }
        }
        System.out.println(c.getName());
    }

    private void printAddClassInfo() {
        Class cls = null;
        try {
            cls = Class.forName(className);
            modif = cls.getModifiers();
            printSuperClasses(cls, true);
        // } catch (ClassNotFoundException ex) {
        //     error = "error: class " + className + " not found.";
        //     return;
        // } catch (NoSuchFieldError ex) {
        //     error = "error: no such field in class " + className;
        //     return;
        } catch (Throwable ex) {
        }
    }

    private void printClassInfoInfo() {
        Class cls = null;
        try {
            cls = Class.forName(className);
        // } catch (ClassNotFoundException ex) {
        //     error = "error: class " + className + " not found.";
        //     return;
        } catch (Throwable ex) {
        }
        String s = "modifier: ";
        modif = cls.getModifiers();
        if (isPublic())
            s += "public ";
        if (isAbstract())
            s += "abstract ";
        if (isFinal())
            s += "final ";
        System.out.println(s);
        s = "interface:";
        boolean first = true;
        Class interfaces[] = cls.getInterfaces();
        int len = interfaces.length;
        for (int i = 0; i < len; i++) {
            Class if1 = interfaces[i];
            if (first) {
                first = false;
                s += " ";
            } else {
                s += ", ";
            }
            s += if1.getName();
        }
        System.out.println(s);
    }

    private void printMemberInfo() {
        Class cls = null;
        try {
            cls = Class.forName(className);
        // } catch (ClassNotFoundException ex) {
        //     error = "error: class " + className + " not found.";
        //     return;
        } catch (Throwable ex) {
        }
        Field fieldList[], field;
        Constructor constructorList[], constructor;
        Method methodList[], method;
        if (declared == DECLARED) {
            fieldList = cls.getDeclaredFields();
            constructorList = cls.getDeclaredConstructors();
            methodList = cls.getDeclaredMethods();
        } else {
            fieldList = cls.getFields();
            constructorList = cls.getConstructors();
            methodList = cls.getMethods();
        }
        int length, index; StringBuffer sb; String s;
        // field
        if (kind == ALL || kind == FIELD) {
            length = fieldList.length;
            for (index = 0; index < length; index++) {
                field = fieldList[index];
                modif = field.getModifiers();
                if (protection == PUBLIC) {
                    if (!isPublic()) continue;
                } else if (protection == PUBLIC_PROTECTED) {
                    if (!isPublic() && !isProtected()) continue;
                }
                if (function == NOT_STATIC) {
                    if (isStatic()) continue;
                } else if (function == STATIC) {
                    if (!isStatic()) continue;
                }
                sb = new StringBuffer("field: ");
                if (isPublic()) {
                    sb.append("public ");
                } else if (isProtected()) {
                    sb.append("protected ");
                } else if (isPrivate()) {
                    sb.append("private ");
                }
                if (isStatic()) sb.append("static ");
                if (isFinal()) sb.append("final ");
                if (isTransient()) sb.append("transient ");
                if (isVolatile()) sb.append("volatile ");
                Class c1 = field.getType();
                int dim = arrayDimension(c1);
                sb.append(componentType(c1).getName());
                for (int i = 0; i < dim; i++) sb.append("[]");
                sb.append(" ");
                sb.append(field.getName());
                sb.append(";");
                s = new String(sb);
                System.out.println(s);
            }
        }
        // constructor
        if (kind == ALL || kind == CONSTRUCTOR) {
            length = constructorList.length;
            for (index = 0; index < length; index++) {
                constructor = constructorList[index];
                modif = constructor.getModifiers();
                if (protection == PUBLIC) {
                    if (!isPublic()) continue;
                } else if (protection == PUBLIC_PROTECTED) {
                    if (!isPublic() && !isProtected()) continue;
                }
                if (function == NOT_STATIC) {
                    if (isStatic()) continue;
                } else if (function == STATIC) {
                    if (!isStatic()) continue;
                }
                sb = new StringBuffer("constructor: ");
                if (isPublic()) {
                    sb.append("public ");
                } else if (isProtected()) {
                    sb.append("protected ");
                } else if (isPrivate()) {
                    sb.append("private ");
                }
                if (isAbstract()) sb.append("abstract ");
                sb.append(constructor.getName());
                makeArgList(sb, constructor.getParameterTypes());
                makeExceptionList(sb, constructor.getExceptionTypes());
                sb.append(";");
                s = new String(sb);
                System.out.println(s);
            }
        }
        // method
        if (kind == ALL || kind == METHOD) {
            length = methodList.length;
            for (index = 0; index < length; index++) {
                method = methodList[index];
                modif = method.getModifiers();
                if (protection == PUBLIC) {
                    if (!isPublic()) continue;
                } else if (protection == PUBLIC_PROTECTED) {
                    if (!isPublic() && !isProtected()) continue;
                }
                if (function == NOT_STATIC) {
                    if (isStatic()) continue;
                } else if (function == STATIC) {
                    if (!isStatic()) continue;
                }
                printMethod("method: ", method);
            }
        }
    }

    private void makeArgList(StringBuffer sb, Class argList[]) {
        int index, length = argList.length;
        boolean first = true;
        sb.append("(");
        for (index = 0; index < length; index++) {
            if (first) first = false;
            else sb.append(", ");
            Class c1 = argList[index];
            int dim = arrayDimension(c1);
            sb.append(componentType(c1).getName());
            for (int i = 0; i < dim; i++) sb.append("[]");
        }
        sb.append(")");
    }

    private void makeExceptionList(StringBuffer sb, Class expList[]) {
        int index, length = expList.length;
        boolean first = true;
        for (index = 0; index < length; index++) {
            if (first) { sb.append(" throws "); first = false; }
            else sb.append(", ");
            Class c1 = expList[index];
            int dim = arrayDimension(c1);
            sb.append(componentType(c1).getName());
            for (int i = 0; i < dim; i++) sb.append("[]");
        }
    }

    private void printBeanInfo() {
        Class cls = null;
        try {
            cls = Class.forName(className);
        // } catch (ClassNotFoundException ex) {
        //     error = "error: class " + className + " not found.";
        //     return;
        } catch (Throwable ex) {
        }
        BeanInfo info = null;
        try {
            info = Introspector.getBeanInfo(cls);
        } catch (IntrospectionException ex) {
            error = "error: class " + className + " is not Bean.";
            return;
        }
        MethodDescriptor mds[] = info.getMethodDescriptors(), md;
        PropertyDescriptor pds[] = info.getPropertyDescriptors(), pd;
        EventSetDescriptor eds[] = info.getEventSetDescriptors(), ed;
        int index, length, index2, length2; StringBuffer sb; String s;
        Method method;
        Method getter, setter;
        Method add, remove, listener[], l;
        if (kind == ALL || kind == METHOD) {
            length = mds.length;
            for (index = 0; index < length; index++) {
                md = mds[index];
                method = md.getMethod();
                printMethod("method: ", md.getMethod());
            }
        }
        if (kind == ALL || kind == PROPERTY) {
            length = pds.length;
            for (index = 0; index < length; index++) {
                pd = pds[index];
                getter = pd.getReadMethod();
                setter = pd.getWriteMethod();
                if (getter != null) {
                    printMethod("getter: ", getter);
                }
                if (setter != null) {
                    printMethod("setter: ", setter);
                }
            }
        }
        if (kind == ALL || kind == EVENT) {
            length = eds.length;
            for (index = 0; index < length; index++) {
                ed = eds[index];
                add = ed.getAddListenerMethod();
                remove = ed.getRemoveListenerMethod();
                listener = ed.getListenerMethods();
                // addListenerMethod
                printMethod("addListener: ", add);
                // removeListenerMethod
                printMethod("removeListener: ", remove);
                // listenerMethods
                int len = listener.length;
                for (int i = 0; i < len; i++) {
                    l = listener[i];
                    printMethod("  ", l);
                }
            }
        }
    }

    private boolean isPublic() {
        return Modifier.isPublic(modif);
    }

    private boolean isProtected() {
        return Modifier.isProtected(modif);
    }

    private boolean isPrivate() {
        return Modifier.isPrivate(modif);
    }

    private boolean isAbstract() {
        return Modifier.isAbstract(modif);
    }

    private boolean isFinal() {
        return Modifier.isFinal(modif);
    }

    private boolean isStatic() {
        return Modifier.isStatic(modif);
    }

    private boolean isTransient() {
        return Modifier.isTransient(modif);
    }

    private boolean isVolatile() {
        return Modifier.isVolatile(modif);
    }

     private boolean isSynchronized() {
        return Modifier.isSynchronized(modif);
    }

    private boolean isNative() {
        return Modifier.isNative(modif);
    }

    private Class componentType(Class cls) {
        if (!cls.isArray())
            return cls;
        return componentType(cls.getComponentType());
    }

    private int arrayDimension(Class cls) {
        if (!cls.isArray())
            return 0;
        return arrayDimension(cls.getComponentType()) + 1;
    }

    private void printMethod(String pre, Method method) {
        modif = method.getModifiers();
        StringBuffer sb = new StringBuffer(pre);
        if (isPublic()) {
            sb.append("public ");
        } else if (isProtected()) {
            sb.append("protected ");
        } else if (isPrivate()) {
            sb.append("private ");
        }
        if (isAbstract()) sb.append("abstract ");
        if (isStatic()) sb.append("static ");
        if (isFinal()) sb.append("final ");
        if (isSynchronized()) sb.append("synchronized ");
        if (isNative()) sb.append("native ");
        Class c1 = method.getReturnType();
        int dim = arrayDimension(c1);
        sb.append(componentType(c1).getName());
        sb.append(" ");
        sb.append(method.getName());
        makeArgList(sb, method.getParameterTypes());
        for (int i = 0; i < dim; i++) sb.append("[]");
        makeExceptionList(sb, method.getExceptionTypes());
        sb.append(";");
        String s = new String(sb);
        System.out.println(s);
    }
}
