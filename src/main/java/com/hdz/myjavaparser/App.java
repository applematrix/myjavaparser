package com.hdz.myjavaparser;

import com.github.javaparser.StaticJavaParser;
import com.github.javaparser.ast.CompilationUnit;
import com.github.javaparser.ast.Node;
import com.github.javaparser.ast.body.FieldDeclaration;
import com.github.javaparser.ast.body.MethodDeclaration;
import com.github.javaparser.ast.visitor.VoidVisitorAdapter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Optional;

/**
 * Hello world!
 *
 */
public class App 
{
    private static final String TEST_PATH = "/home/lenovo/WORKING_DIRECTORY/frameworks/base/services/core/java/com/android/server/pm/PackageManagerService.java";

    private static final String OLD_PATH = "/home/lenovo/javaparser_files/android_11_release";
    private static final String NEW_PATH = "/home/lenovo/javaparser_files/android_12_release";


    public static void main( String[] args )
    {
        testJavaFileParser();
    }

    private static void testJavaFileParser() {
        CombinedPackageParser combinedPackageParser = new CombinedPackageParser(OLD_PATH, NEW_PATH);
        combinedPackageParser.analyze();

        JavaFileParser parser = new JavaFileParser(TEST_PATH);
        parser.analyze();
    }
}
