package com.hdz.myjavaparser;

import java.util.ArrayList;
import java.util.List;

public class MarkdownList extends MarkdownBlock{
    private ArrayList<String> mListItems = new ArrayList<>();

    public MarkdownList() {
    }

    public void addItem(String item) {
        mListItems.add(item);
    }

    public void addItems(List<String> items) {
        if (items == null) {
            return;
        }
        mListItems.addAll(items);
    }

    @Override
    public byte[] getContent() {
        StringBuilder sb = new StringBuilder();
        for (String item : mListItems) {
            sb.append("- ").append(item).append("\n");
        }
        return sb.toString().getBytes();
    }
}
