package com.hdz.myjavaparser;

import com.github.javaparser.StaticJavaParser;
import com.github.javaparser.ast.CompilationUnit;
import com.github.javaparser.ast.Node;
import com.github.javaparser.ast.body.ClassOrInterfaceDeclaration;
import com.github.javaparser.ast.body.FieldDeclaration;
import com.github.javaparser.ast.body.MethodDeclaration;
import com.github.javaparser.ast.visitor.VoidVisitorAdapter;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class JavaFileParser {
    private final String mJavaFilePath;
    private String mClassName;
    private String mPackageName;
    private Map<String, JavaDeclaredClass> mDeclaredClasses = new HashMap<>();

    public JavaFileParser(String path) {
        mJavaFilePath = path;
    }

    public String getClassName() {
        return mClassName;
    }

    public String getPackageName() {
        return mPackageName;
    }

    public void analyze() {
        mDeclaredClasses.clear();
        try {
            CompilationUnit cu = StaticJavaParser.parse(new File(mJavaFilePath));
            VoidVisitorAdapter<?> classDeclarationVisitor = new ClassDeclarationVisitor();
            classDeclarationVisitor.visit(cu, null);

            VoidVisitorAdapter<?> fieldDeclarationVisitor = new FieldAndMethodDeclarationVisitor();
            fieldDeclarationVisitor.visit(cu, null);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public void diff(JavaFileParser other, List<String> missedClasses) {
        for (JavaDeclaredClass clazz : mDeclaredClasses.values()) {
            if (!other.mDeclaredClasses.containsKey(clazz.className)) {
                missedClasses.add(clazz.className);
                System.out.println("Miss " + clazz.fullName);
            }
        }
    }

    private class ClassDeclarationVisitor extends VoidVisitorAdapter<Void> {
        @Override
        public void visit(ClassOrInterfaceDeclaration n, Void arg) {
            super.visit(n, arg);
            String className = n.getNameAsString();
            JavaDeclaredClass classItem = new JavaDeclaredClass(n);
            if (!mDeclaredClasses.containsKey(className)) {
                mDeclaredClasses.put(className, classItem);
            }

            Optional<Node> node = n.getParentNode();
            if (node.isPresent()) {
                Node node1 = node.get();
                if (node1 instanceof  ClassOrInterfaceDeclaration) {
                    ClassOrInterfaceDeclaration parentClass = (ClassOrInterfaceDeclaration)node1;
                    JavaDeclaredClass parent = mDeclaredClasses.get(parentClass.getNameAsString());
                    if (parent != null) {
                        parent.addChild(classItem);
                    } else {
                        parent = new JavaDeclaredClass(parentClass);
                        parent.addChild(classItem);
                        mDeclaredClasses.put(parentClass.getNameAsString(), parent);
                    }
                    classItem.setParent(parent);
                    System.out.println(n.getName() + "-->" + classItem.fullName);
                } else {
                    System.out.println(n.getName() + "--> null");
                }
            } else {
                System.out.println(n.getName() + "--> unknown");
            }
        }
    }

    private class FieldAndMethodDeclarationVisitor extends VoidVisitorAdapter<Void> {
        @Override
        public void visit(FieldDeclaration n, Void arg) {
            super.visit(n, arg);
            Optional<Node> node = n.getParentNode();
            if (!node.isPresent()) {
                return;
            }
            Node node1 = node.get();
            if (node1 instanceof  ClassOrInterfaceDeclaration) {
                ClassOrInterfaceDeclaration container = (ClassOrInterfaceDeclaration)node1;
                JavaDeclaredClass declaredClass = mDeclaredClasses.get(container.getNameAsString());
                if (declaredClass != null) {
                    declaredClass.addFieldDeclaration(n);
                }
                //System.out.println(n.toString()+ " is field of " + declaredClass.fullName);
            } else {
                System.out.println(n +  "--> null");
            }
        }

        @Override
        public void visit(MethodDeclaration n, Void arg) {
            super.visit(n, arg);
            Optional<Node> node = n.getParentNode();
            if (!node.isPresent()) {
                return;
            }
            Node node1 = node.get();
            if (node1 instanceof  ClassOrInterfaceDeclaration) {
                ClassOrInterfaceDeclaration container = (ClassOrInterfaceDeclaration)node1;
                JavaDeclaredClass declaredClass = mDeclaredClasses.get(container.getNameAsString());
                if (declaredClass != null) {
                    declaredClass.addMethodDeclaration(n);
                }
                //System.out.println(n.toString()+ " is method of " + declaredClass.fullName);
            } else {
                System.out.println(n +  "--> null");
            }
        }
    }

    public class JavaFileParserDiff {

    }
}
