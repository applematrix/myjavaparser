package com.hdz.myjavaparser;

import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class MarkdownBlock {
    private MarkdownBlock mParent;
    private ArrayList<MarkdownBlock> mChildren = new ArrayList<>();

    public MarkdownBlock getParent() {
        return mParent;
    }

    public byte[] getContent() {
        return "".getBytes();
    }

    void write(FileOutputStream fos) {
        try {
            fos.write(getContent());
            fos.write("\n".getBytes());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
