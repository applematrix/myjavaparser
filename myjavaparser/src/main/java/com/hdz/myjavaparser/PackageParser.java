package com.hdz.myjavaparser;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class PackageParser {
    private final static String JAVA_SUFIX = ".java";
    private final String mPackageRoot;

    private List<String> mAllJavaFiles = new ArrayList<>();
    private Map<String, JavaFileParser> mAllParsedJavaFiles = new HashMap<>();

    public PackageParser(String packagePath) {
        mPackageRoot = packagePath;
        traverseDir(mPackageRoot);
        mAllJavaFiles.sort(String::compareTo);
    }

    public void diffFiles(PackageParser other, Set<String> missedFiles, Set<String> changedFiles) {
        String otherRoot = other.getRootPath();
        List<String> otherJavaFiles = other.getFiles();
        for (String myJavaFile : mAllJavaFiles) {
            String relativePath = getRelativePath(myJavaFile);
            String otherPath = otherRoot + File.separator + relativePath;
            int index = Collections.binarySearch(otherJavaFiles, otherPath);
            if (index != -1) {
                if (changedFiles != null) {
                    long myFileSize = new File(myJavaFile).length();
                    long otherFileSize = new File(otherPath).length();
                    if (myFileSize != otherFileSize) {
                        changedFiles.add(relativePath);
                    }
                }
            } else {
                missedFiles.add(relativePath);
            }
        }
    }

    public void diffJavaFiles(PackageParser other) {
        for (String path : mAllParsedJavaFiles.keySet()) {
            JavaFileParser myJavaFile = mAllParsedJavaFiles.get(path);
            String theirPath = other.toAbsolutePath(getRelativePath(path));
            JavaFileParser theirJavaFile = other.mAllParsedJavaFiles.get(theirPath);


            //myJavaFile.diff(theirJavaFile);
        }
    }

    public List<String> getFiles() {
        return mAllJavaFiles;
    }

    public String getRootPath() {
        return mPackageRoot;
    }

    private String getRelativePath(String path) {
        return path.substring(mPackageRoot.length() + 1);
    }

    private String toAbsolutePath(String path) {
        return mPackageRoot + File.separator + path;
    }

    private void traverseDir(String dir) {
        File root = new File(dir);
        String[] list = root.list();
        if (list == null || list.length == 0) {
            return;
        }
        for (String file : list) {
            try {
                String path = root.getCanonicalPath() + File.separator + file;
                if (new File(path).isDirectory()) {
                    traverseDir(path);
                }
                if (!path.endsWith(JAVA_SUFIX)) {
                    continue;
                }
                mAllJavaFiles.add(path);
                mAllParsedJavaFiles.put(path, new JavaFileParser(path));
                System.out.println("put " + path);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
