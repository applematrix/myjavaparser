package com.hdz.myjavaparser;

public class MarkdownText extends MarkdownBlock {
    private final String mContent;
    private int mHeadLevel;
    private int mBold;
    private int mItalic;

    public MarkdownText(String content) {
        mContent = content;
    }

    public void setHeadLevel(int headLevel) {
        mHeadLevel = headLevel;
    }

    @Override
    public byte[] getContent() {
        StringBuilder sb = new StringBuilder(mContent);
        if (mHeadLevel != 0) {
            sb.insert(0, " ");
            for (int i = 0; i < mHeadLevel; i++) {
                sb = sb.insert(0, "#");
            }
        }
        sb.append("\n");
        return sb.toString().getBytes();
    }
}
