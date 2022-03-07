package com.hdz.myjavaparser;

import com.github.javaparser.ast.body.ClassOrInterfaceDeclaration;
import com.github.javaparser.ast.body.FieldDeclaration;
import com.github.javaparser.ast.body.MethodDeclaration;
import com.github.javaparser.ast.body.VariableDeclarator;

import java.util.ArrayList;

public class JavaDeclaredClass {
    public String className;
    public String fullName;
    public ClassOrInterfaceDeclaration clazz;
    public JavaDeclaredClass parent;
    public ArrayList<JavaDeclaredClass> children = new ArrayList<>();
    public ArrayList<FieldDeclaration> fields = new ArrayList<>();
    public ArrayList<MethodDeclaration> methods = new ArrayList<>();

    public JavaDeclaredClass(ClassOrInterfaceDeclaration clazz) {
        className = clazz.getName().toString();
        fullName = className;
        this.clazz = clazz;
    }

    public void setParent(JavaDeclaredClass parent) {
        this.parent = parent;
        updateFullName();
    }

    public void addChild(JavaDeclaredClass child) {
        children.add(child);
        child.updateFullName();
    }

    public void updateFullName() {
        if (parent != null) {
            fullName = parent.fullName + "." + className;
        }
        for (JavaDeclaredClass child : children) {
            child.updateFullName();
        }
    }

    public void addFieldDeclaration(FieldDeclaration declaration) {
        fields.add(declaration);
    }

    public void addMethodDeclaration(MethodDeclaration declaration) {
        methods.add(declaration);
    }

    public JavaClassDiff diff(JavaDeclaredClass other) {
        JavaClassDiff classDiff = new JavaClassDiff();
        if (!other.className.equals(className)) {
            throw new RuntimeException();
        }

        ArrayList<FieldDeclaration> missedFields = new ArrayList<>();
        for (FieldDeclaration field : fields) {
            if (field.getVariables().size() != 1) {
                continue;
            }
            VariableDeclarator declarator =field.getVariable(0);
            String fieldName = declarator.getNameAsString();
            //field.
        }

        return classDiff;
    }

    private void fieldDiff(JavaDeclaredClass other) {
        for (FieldDeclaration field : fields) {
            if (field.getVariables().size() != 1) {
                continue;
            }
            VariableDeclarator declarator =field.getVariable(0);
            String fieldName = declarator.getNameAsString();


            //field.
        }
    }

    public FieldDeclaration findFieldByName(String name) {
        for (FieldDeclaration field : fields) {
            if (field.getVariables().size() != 1) {
                continue;
            }
            VariableDeclarator declarator =field.getVariable(0);
            String fieldName = declarator.getNameAsString();
            if (fieldName.equals(name)) {
                return field;
            }
        }
        return null;
    }
}
