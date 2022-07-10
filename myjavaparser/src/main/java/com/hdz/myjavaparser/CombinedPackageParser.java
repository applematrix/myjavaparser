package com.hdz.myjavaparser;

import java.util.HashSet;
import java.util.Set;

public class CombinedPackageParser {
    private PackageParser mOldPackageParser;
    private PackageParser mNewPackageParser;

    Set<String> missedFiles = new HashSet<>();
    Set<String> addedFiles = new HashSet<>();
    Set<String> changedFiles = new HashSet<>();

    public JavaDirDiff mDirDiff;

    public CombinedPackageParser(String oldPackagePath, String newPackagePath) {
        mOldPackageParser = new PackageParser(oldPackagePath);
        mNewPackageParser = new PackageParser(newPackagePath);
    }

    private void analyzeDir() {
        mOldPackageParser.diffFiles(mNewPackageParser, missedFiles, changedFiles);
        mNewPackageParser.diffFiles(mOldPackageParser, addedFiles, null);

        mDirDiff = new JavaDirDiff(mOldPackageParser, mNewPackageParser);
    }

    public void analyze() {
        analyzeDir();
        analyzeCode();
        // TODO:
        generateReport("/home/lenovo/test.md");
    }

    public String getPackageName() {
        return "Analyze Report -- com.android.os\n";
    }

    public void analyzeCode() {
        mOldPackageParser.diffJavaFiles(mNewPackageParser);
    }

    private void generateReport(String filePath) {
        MarkdownReport report = new MarkdownReport(filePath, this);
        report.generate();
    }
}
