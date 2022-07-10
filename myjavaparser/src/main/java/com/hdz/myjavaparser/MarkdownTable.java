package com.hdz.myjavaparser;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class MarkdownTable extends MarkdownBlock {
    final private ArrayList<String> mColumnTitle;
    private final int mColumn;
    private ArrayList<ArrayList<String>> mTableContent = new ArrayList<>();

    public MarkdownTable(ArrayList<String> title) {
        mColumnTitle = title;
        mColumn = title.size();
    }

    public void addRow(ArrayList<String> row) {
        if (row.size() != mColumn) {
            throw new RuntimeException();
        }
        mTableContent.add(row);
    }

    @Override
    public byte[] getContent() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < mColumnTitle.size(); i ++) {
            sb.append(mColumnTitle.get(i)).append(" |");
        }
        sb.insert(0, "| ");
        sb.append("\n");
        for (int i = 0; i < mColumnTitle.size(); i ++) {
            sb.append("| ---- ");
        }
        sb.append(" |\n");

        for (int i = 0; i < mTableContent.size(); i++) {
            sb.append("| ");
            for (int j = 0; j < mTableContent.get(i).size(); j++) {
                sb.append(mTableContent.get(i).get(j)).append(" |");
            }
            sb.append("\n");
        }
        return sb.toString().getBytes();
    }

    public static String tableItem(Set<String> set) {
        if (set.size() == 0) {
            return "无";
        }
        ArrayList<String> list = new ArrayList<>();
        list.addAll(set);
        list.sort(String::compareTo);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            sb.append(list.get(i));
            if (i != list.size() - 1) {
                sb.append("<br>");
            }
        }
        return sb.toString();
    }

    public static String tableItem(List<String> list) {
        if (list.size() == 0) {
            return "无";
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            sb.append(list.get(i));
            if (i != list.size() - 1) {
                sb.append("<br>");
            }
        }
        return sb.toString();
    }
}
