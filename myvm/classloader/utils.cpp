#include "utils.h"

const char* INDENTS[] = {
    "",
    "    ",
    "        ",
    "            ",
    "                ",
    "                    ",
};

const char* indent(uint16_t depth) {
    return INDENTS[depth];
}