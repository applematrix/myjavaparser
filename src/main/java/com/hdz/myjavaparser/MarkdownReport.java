package com.hdz.myjavaparser;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;

public class MarkdownReport {
    private final String mReportPath;
    private final CombinedPackageParser mCombinedPackageParser;
    private ArrayList<MarkdownBlock> mMarkdownContents = new ArrayList<>();

    public MarkdownReport(String reportPath, CombinedPackageParser combinedPackageParser) {
        mReportPath = reportPath;
        mCombinedPackageParser = combinedPackageParser;
    }

    public void generate() {
        generateTitle();
        generateDirDiffInfo();
        write();
    }

    private void generateTitle() {
        MarkdownText title = new MarkdownText(mCombinedPackageParser.getPackageName() + "\n");
        title.setHeadLevel(MarkdownUtils.TITLE);
        mMarkdownContents.add(title);
    }

    private void generateDirDiffInfo() {
        if (mCombinedPackageParser.mDirDiff != null) {
            mCombinedPackageParser.mDirDiff.writeMarkdown(mMarkdownContents);
        }

//        addFileListInfo("新增Java文件列表:\n", mCombinedPackageParser.addedFiles);
//        addFileListInfo("删除Java文件列表:\n", mCombinedPackageParser.missedFiles);
//        addFileListInfo("修改Java文件列表:\n", mCombinedPackageParser.changedFiles);
    }

    private void addFileListInfo(String type, Set<String> files) {
        MarkdownText fileListInfo = new MarkdownText(type);
        fileListInfo.setHeadLevel(MarkdownUtils.HEADER_2);
        mMarkdownContents.add(fileListInfo);

        if (files.isEmpty()) {
            MarkdownText filesChangedInfo = new MarkdownText("无\n");
            mMarkdownContents.add(filesChangedInfo);
            return;
        }

        MarkdownList mdList = new MarkdownList();
        ArrayList<String> list = new ArrayList<>();
        list.addAll(files);
        list.sort(String::compareTo);
        mdList.addItems(list);
        mMarkdownContents.add(mdList);
    }

    private void write() {
        try (FileOutputStream fileOutputStream = new FileOutputStream(mReportPath)) {
            for (MarkdownBlock block : mMarkdownContents) {
                block.write(fileOutputStream);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
