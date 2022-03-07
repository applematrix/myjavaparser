package com.hdz.myjavaparser;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

public class JavaDirDiff {
    Set<String> missedFiles = new HashSet<>();
    Set<String> addedFiles = new HashSet<>();
    Set<String> changedFiles = new HashSet<>();

    JavaDirDiff(PackageParser old, PackageParser new_) {
        old.diffFiles(new_, missedFiles, changedFiles);
        new_.diffFiles(old, addedFiles, null);
    }

    public void writeMarkdown(ArrayList<MarkdownBlock> content) {
        MarkdownText filesChangedInfo = new MarkdownText("Java文件修改信息\n");
        filesChangedInfo.setHeadLevel(MarkdownUtils.HEADER_1);
        content.add(filesChangedInfo);

        ArrayList<String> title = new ArrayList<>();
        title.add("新增Java文件列表");
        title.add("删除Java文件列表");
        title.add("修改Java文件列表");
        MarkdownTable table = new MarkdownTable(title);

        ArrayList<String> tableRow = new ArrayList<>();
        tableRow.add(MarkdownTable.tableItem(addedFiles));
        tableRow.add(MarkdownTable.tableItem(missedFiles));
        tableRow.add(MarkdownTable.tableItem(changedFiles));
        table.addRow(tableRow);

        content.add(table);
    }
}
