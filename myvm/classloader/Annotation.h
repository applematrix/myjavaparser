#ifndef _ANNOTATION_H_
#define _ANNOTATION_H_
#include <vector>
#include <memory>
#include "../common/utils.h"
#include "AttributeInfo.h"
using namespace myvm;
using namespace std;

namespace myvm {

class Annotation;

struct ElementValue {
    enum Type {
        CONSTANT_VALUE,
        ENUM_VALUE,
        CLASS_INFO,
        ANNOTATION,
        ARRAY,
    };
public:
    uint8_t tag;
    Type valueType;
    union {
        uint16_t constValue;
        uint16_t classInfoIndex;
        unique_ptr<Annotation> annotationValue;
        struct {
            uint16_t count;
            vector<unique_ptr<ElementValue>> *values; // in the union, so use the raw pointer
        } arrayValue;
        struct {
            uint16_t typeNameIndex;
            uint16_t constNameIndex;
        } enumConstantValue;
    };
    
    ElementValue() {}
    ~ElementValue();

    bool load(shared_ptr<FileReader> &fileReader);
    bool loadConstantValue(shared_ptr<FileReader> &fileReader);
    bool loadEnumConstantValue(shared_ptr<FileReader> &fileReader);
    bool loadClassInfoIndex(shared_ptr<FileReader> &fileReader);
    bool loadAnnotation(shared_ptr<FileReader> &fileReader);
    bool loadArray(shared_ptr<FileReader> &fileReader);
};

struct ElementPair {
public:
    ElementPair(){}
    ~ElementPair(){}
    uint16_t nameIndex;
    unique_ptr<ElementValue> value;

    ElementPair(uint16_t name) {
        nameIndex = name;
    }

    bool load(shared_ptr<FileReader> &fileReader) {
        value.reset(new ElementValue());
        return true;
    }
};

struct Annotation {
public:
    uint16_t typeIndex;
    uint16_t elementCount;
    vector<ElementPair*> elementPairs;

    Annotation(uint16_t type, uint16_t elements);
    static Annotation* load(shared_ptr<FileReader> &fileReader);
    bool loadPairs(shared_ptr<FileReader> &fileReader);
};

class RuntimeVisibleAnnotationsAttr: public AttributeInfo {
public:
    uint16_t annotationNums;
    vector<unique_ptr<Annotation>> annotations;
    // TODO:
    // annotation annotations[0];

    RuntimeVisibleAnnotationsAttr(uint16_t name, uint32_t len, shared_ptr<FileReader> fileReader)
        : AttributeInfo(name, len, ATTR_RUNTIME_VISIBLE_ANNOTATIONS) {
        initialize(fileReader);
    }

    ~RuntimeVisibleAnnotationsAttr();

    void initialize(shared_ptr<FileReader> &fileReader);
};


}




#endif