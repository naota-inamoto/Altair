
/* $Id: HApiSearcher.java,v 1.2 1999/01/29 11:18:18 inamoto Exp $ */

import java.lang.Class;
import java.lang.ClassNotFoundException;

import java.lang.reflect.Field;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import java.util.Hashtable;

public class HApiSearcher {
    protected static Hashtable hashtable = null;

    protected HApiInfo selInfo;

    protected Field selFields[];
    protected Constructor selConstructors[];
    protected Method selMethods[];
    protected Class selClasses[];

    protected Field selDeclaredFields[];
    protected Constructor selDeclaredConstructors[];
    protected Method selDeclaredMethods[];
    protected Class selDeclaredClasses[];

    protected Class selInterfaces[];

    protected int index;

    protected int fieldMaxIndex;
    protected int constructorMaxIndex;
    protected int methodMaxIndex;
    protected int classMaxIndex;

    protected int declaredFieldMaxIndex;
    protected int declaredConstructorMaxIndex;
    protected int declaredMethodMaxIndex;
    protected int declaredClassMaxIndex;

    protected int ifMaxIndex;

    protected Field field1;
    protected Constructor constructor1;
    protected Method method1;
    protected Class class1;
    protected int mod;

    protected Class argList[];
    protected int argIndex, argMaxIndex;
    protected Class arg1;

    protected Class expList[];
    protected int expIndex, expMaxIndex;
    protected Class exp1;

    protected Class if1; 

    public HApiSearcher() {
        if (hashtable == null)
            hashtable = new Hashtable();
    }
    public HApiSearcher(String name) {
        if (hashtable == null)
            hashtable = new Hashtable();
        selectClass(name);
    }
    public boolean addClass(String clsName) {
        if (hashtable == null)
            hashtable = new Hashtable();
        Class cls;
        try {
           cls = Class.forName(clsName);
        } catch (ClassNotFoundException ex) {
           return false;
        }
        selInfo = (HApiInfo)hashtable.get(cls);
        if (selInfo == null) {
            selInfo = new HApiInfo(cls, hashtable);
            hashtable.put(cls, selInfo);
        }
        return selectClass1(cls);
    }
    public boolean selectClass(String clsName) {
        Class cls;
        try {
            cls = Class.forName(clsName);
        } catch(ClassNotFoundException ex) {
            return false;
        }
        selInfo = (HApiInfo)hashtable.get(cls);
        if (selInfo == null)
            return false;
        return selectClass1(cls);
    }
    protected boolean selectClass1(Class cls) {
        selFields = selInfo.fields();
        selConstructors = selInfo.constructors();
        selMethods = selInfo.methods();
        selClasses = selInfo.classes();

        selDeclaredFields = selInfo.declaredFields();
        selDeclaredConstructors = selInfo.declaredConstructors();
        selDeclaredMethods = selInfo.declaredMethods();
        selDeclaredClasses = selInfo.declaredClasses();

        selInterfaces = selInfo.interfaces();

        fieldMaxIndex = 0;
        constructorMaxIndex = 0;
        methodMaxIndex = 0;
        classMaxIndex = 0;

        declaredFieldMaxIndex = 0;
        declaredConstructorMaxIndex = 0;
        declaredMethodMaxIndex = 0;
        declaredClassMaxIndex = 0;

        ifMaxIndex = 0;

        if (selFields != null)
            fieldMaxIndex = selFields.length;
        if (selConstructors != null)
            constructorMaxIndex = selConstructors.length;
        if (selMethods != null)
            methodMaxIndex = selMethods.length;
        if (selClasses != null)
            classMaxIndex = selClasses.length;

        if (selDeclaredFields != null)
            declaredFieldMaxIndex = selDeclaredFields.length;
        if (selDeclaredConstructors != null)
            declaredConstructorMaxIndex = selDeclaredConstructors.length;
        if (selDeclaredMethods != null)
            declaredMethodMaxIndex = selDeclaredMethods.length;
        if (selDeclaredClasses != null)
            declaredClassMaxIndex = selDeclaredClasses.length;

        if (selInterfaces != null)
            ifMaxIndex = selInterfaces.length;

        mod = cls.getModifiers();

        return true;
    }
    public boolean searchFirstField() {
        index = -1;
        return searchNextField();
    }
    public boolean searchNextField() {
        index++;
        if (index < fieldMaxIndex) {
            field1 = selFields[index];
            mod = field1.getModifiers();
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstMethod() {
        index = -1;
        return searchNextMethod();
    }
    public boolean searchNextMethod() {
        index++;
        if (index < methodMaxIndex) {
            method1 = selMethods[index];
            mod = method1.getModifiers();
            argList = method1.getParameterTypes();
            argMaxIndex = 0;
            if (argList != null)
                argMaxIndex = argList.length;
            expList = method1.getExceptionTypes();
            expMaxIndex = 0;
            if (expList != null)
                expMaxIndex = expList.length;
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstConstructor() {
        index = -1;
        return searchNextConstructor();
    }
    public boolean searchNextConstructor() {
        index++;
        if (index < constructorMaxIndex) {
            constructor1 = selConstructors[index];
            mod = constructor1.getModifiers();
            argList = constructor1.getParameterTypes();
            argMaxIndex = 0;
            if (argList != null)
                argMaxIndex = argList.length;
            expList = constructor1.getExceptionTypes();
            expMaxIndex = 0;
            if (expList != null)
                expMaxIndex = expList.length;
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstClass() {
        index = -1;
        return searchNextClass();
    }
    public boolean searchNextClass() {
        index++;
        if (index < classMaxIndex) {
            class1 = selClasses[index];
            mod = class1.getModifiers();
            return true;
        } else {
            return false;
        }
    }

    public boolean searchFirstDeclaredField() {
        index = -1;
        return searchNextDeclaredField();
    }
    public boolean searchNextDeclaredField() {
        index++;
        if (index < declaredFieldMaxIndex) {
            field1 = selDeclaredFields[index];
            mod = field1.getModifiers();
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstDeclaredMethod() {
        index = -1;
        return searchNextDeclaredMethod();
    }
    public boolean searchNextDeclaredMethod() {
        index++;
        if (index < declaredMethodMaxIndex) {
            method1 = selDeclaredMethods[index];
            mod = method1.getModifiers();
            argList = method1.getParameterTypes();
            argMaxIndex = 0;
            if (argList != null)
                argMaxIndex = argList.length;
            expList = method1.getExceptionTypes();
            expMaxIndex = 0;
            if (expList != null)
                expMaxIndex = expList.length;
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstDeclaredConstructor() {
        index = -1;
        return searchNextDeclaredConstructor();
    }
    public boolean searchNextDeclaredConstructor() {
        index++;
        if (index < declaredConstructorMaxIndex) {
            constructor1 = selDeclaredConstructors[index];
            mod = constructor1.getModifiers();
            argList = constructor1.getParameterTypes();
            argMaxIndex = 0;
            if (argList != null)
                argMaxIndex = argList.length;
            expList = constructor1.getExceptionTypes();
            expMaxIndex = 0;
            if (expList != null)
                expMaxIndex = expList.length;
            return true;
        } else {
            return false;
        }
    }
    public boolean searchFirstDeclaredClass() {
        index = -1;
        return searchNextDeclaredClass();
    }
    public boolean searchNextDeclaredClass() {
        index++;
        if (index < declaredClassMaxIndex) {
            class1 = selDeclaredClasses[index];
            mod = class1.getModifiers();
            return true;
        } else {
            return false;
        }
    }

    public boolean isPublic() {
        return Modifier.isPublic(mod);
    }
    public boolean isProtected() {
        return Modifier.isProtected(mod);
    }
    public boolean isPrivate() {
        return Modifier.isPrivate(mod);
    }
    public boolean isAbstract() {
        return Modifier.isAbstract(mod);
    }
    public boolean isFinal() {
        return Modifier.isFinal(mod);
    }
    public boolean isStatic() {
        return Modifier.isStatic(mod);
    }
    public boolean isTransient() {
        return Modifier.isTransient(mod);
    }
    public boolean isVolatile() {
        return Modifier.isVolatile(mod);
    }
    public boolean isSynchronized() {
        return Modifier.isSynchronized(mod);
    }
    public boolean isNative() {
        return Modifier.isNative(mod);
    }

    public String fieldTypeName() {
        return fieldType().getName();
    }
    public Class fieldType() {
        return componentType(field1.getType());
    }
    public String fieldName() {
        return field1.getName();
    }
    public int fieldArrayDimension() {
        return arrayDimension(field1.getType());
    }

    public String methodReturnTypeName() {
        return methodReturnType().getName();
    }
    public Class methodReturnType() {
        return componentType(method1.getReturnType());
    }
    public String methodName() {
        return method1.getName();
    }
    public int methodReturnTypeArrayDimension() {
        return arrayDimension(method1.getReturnType());
    }

    public String constructorName() {
        return constructor1.getName();
    }

    public boolean searchFirstArgument() {
        argIndex = -1;
        return searchNextArgument();
    }
    public boolean searchNextArgument() {
        argIndex++;
        if (argIndex < argMaxIndex) {
            arg1 = argList[argIndex];
            return true;
        } else {
            return false;
        }
    }
    public String argTypeName() {
        return argType().getName();
    }
    public Class argType() {
        return componentType(arg1);
    }
    public int argArrayDimension() {
        return arrayDimension(arg1);
    }

    public boolean searchFirstException() {
        expIndex = -1;
        return searchNextException();
    }
    public boolean searchNextException() {
        expIndex++;
        if (expIndex < expMaxIndex) {
            exp1 = expList[expIndex];
            return true;
        } else {
            return false;
        }
    }
    public String expTypeName() {
        return exp1.getName();
    }
    public Class expType() {
        return exp1;
    }

    public String superclassName() {
        return selInfo.superclassName();
    }
    public Class superclass() {
        return selInfo.superclass();
    }
    public boolean isInterface() {
        return selInfo.isInterface();
    }

    public String className() {
        return class1.getName();
    }

    public boolean searchFirstInterface() {
        index = -1;
        return searchNextInterface();
    }
    public boolean searchNextInterface() {
        index++;
        if (index < ifMaxIndex) {
            if1 = selInterfaces[index];
            return true;
        } else {
            return false;
        }
    }
    public String interfaceTypeName() {
        return interfaceType().getName();
    }
    public Class interfaceType() {
        return if1;
    }

    public Class componentType(Class cls) {
        if (!cls.isArray())
            return cls;
        return componentType(cls.getComponentType());
   }
   public int arrayDimension(Class cls) {
        if (!cls.isArray())
            return 0;
        return arrayDimension(cls.getComponentType()) + 1;
   }
};

class HApiInfo extends Object {
    protected Class class1;

    protected Field fieldList[];
    protected Constructor constructorList[];
    protected Method methodList[];
    protected Class classList[];

    protected Field declaredFieldList[];
    protected Constructor declaredConstructorList[];
    protected Method declaredMethodList[];
    protected Class declaredClassList[];

    protected Class interfaceList[];

    public HApiInfo(Class cls, Hashtable tbl) {
        class1 = cls;
        fieldList = cls.getFields();
        constructorList = cls.getConstructors();
        methodList = cls.getMethods();
        classList = cls.getClasses();
        declaredFieldList = cls.getDeclaredFields();
        declaredConstructorList = cls.getDeclaredConstructors();
        declaredMethodList = cls.getDeclaredMethods();
        declaredClassList = cls.getDeclaredClasses();
        interfaceList = cls.getInterfaces();
        if (classList != null) {
            for (int i = 0; i < classList.length; i++) {
                    Class cls2 = classList[i];
                    HApiInfo info = (HApiInfo)tbl.get(cls2);
                    if (info == null) {
                        info = new HApiInfo(cls2, tbl);
                        tbl.put(cls2, info);
                    }
            }
        }
    }
    public String superclassName() {
        Class super1 = class1.getSuperclass();
        if (super1 == null)
            return null;
        return super1.getName();
    }
    public Class superclass() {
        return class1.getSuperclass();
    }
    public boolean isInterface() {
        return class1.isInterface();
    }

    public Field[] fields() { return fieldList; }
    public Constructor[] constructors() { return constructorList; }
    public Method[] methods() { return methodList; }
    public Class[] classes() { return classList; }

    public Field[] declaredFields() { return declaredFieldList; }
    public Constructor[] declaredConstructors() { return declaredConstructorList; }
    public Method[] declaredMethods() { return declaredMethodList; }
    public Class[] declaredClasses() { return declaredClassList; }

    public Class[] interfaces() { return interfaceList; }
};