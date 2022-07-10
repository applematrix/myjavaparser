package com.hdz.myjavaparser;

import com.github.javaparser.ast.body.FieldDeclaration;

public class ClassFieldChangedInfo {
    public static final int TYPE_MISSED = 1<<0;
    public static final int TYPE_NEW = 1<<1;
    public static final int TYPE_INITIALIZER_CHANGED = 1<<2;
    public static final int TYPE_VALUE_TYPE_CHANGED = 1<<3;
    public static final int TYPE_MODIFIER_CHANGED = 1<<4;

    private FieldDeclaration fieldDeclaration;
    private int changeType;

    public ClassFieldChangedInfo(FieldDeclaration field, int type) {
        fieldDeclaration = field;
        changeType = type;
    }
}
