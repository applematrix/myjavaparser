#include "AccessFlag.h"

namespace myvm {

void accessFlagToString(uint16_t flag, string& outString) {
    if (flag & ACC_PUBLIC != 0) {
        outString.append("public|");
    }
    if ((flag & ACC_FINAL) != 0) {
        outString.append("final|");
    }
    if ((flag & ACC_SUPER) != 0) {
        outString.append("super|");
    }
    if ((flag & ACC_INTERFACE) != 0) {
        outString.append("interface|");
    }
    if ((flag & ACC_ABSTRACT) != 0) {
        outString.append("abstract|");
    }
    if ((flag & ACC_SYNTHETIC) != 0) {
        outString.append("synthetic|");
    }
    if ((flag & ACC_ANNOTATION) != 0) {
        outString.append("annotation|");
    }
    if ((flag & ACC_ENUM) != 0) {
        outString.append("enum|");
    }
    if ((flag & ACC_MODULE) != 0) {
        outString.append("module|");
    }
    if (outString.length() > 0) {
        outString.resize(outString.size() - 1);
    }
}

}