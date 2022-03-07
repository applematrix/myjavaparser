package com.hdz.myjavaparser;

import java.util.ArrayList;
import java.util.List;

public class JavaFileDiff {
    private List<String> missedClasses = new ArrayList<>();
    private List<String> newClasses = new ArrayList<>();

    public JavaFileDiff(JavaFileParser oldFile, JavaFileParser newFile) {
        oldFile.diff(newFile, missedClasses);
        newFile.diff(oldFile, newClasses);
    }

    public void writeMarkdown(ArrayList<MarkdownBlock> content) {
        if (missedClasses.size() == 0 && newClasses.size() == 0) {
            return;
        }
        MarkdownText filesChangedInfo = new MarkdownText("Java文件修改信息\n");
        filesChangedInfo.setHeadLevel(MarkdownUtils.HEADER_1);
        content.add(filesChangedInfo);

        ArrayList<String> title = new ArrayList<>();
        title.add("新增Java Class列表");
        title.add("删除Java Class列表");
        MarkdownTable table = new MarkdownTable(title);

        ArrayList<String> tableRow = new ArrayList<>();
        tableRow.add(MarkdownTable.tableItem(newClasses));
        tableRow.add(MarkdownTable.tableItem(missedClasses));
        table.addRow(tableRow);

        content.add(table);
    }
}
