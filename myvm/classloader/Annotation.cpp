/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "Annotation.h"
using namespace myvm;

namespace myvm
{

    ElementValue::~ElementValue()
    {
        if (valueType == ARRAY) {
            arrayValue.values->clear();
            delete arrayValue.values;
        }
    }

    bool ElementValue::load(shared_ptr<FileReader> &fileReader)
    {
        int status = fileReader->readUint8(tag);
        switch (tag)
        {
        case 'B': // byte
        case 'C': // char
        case 'D': // double
        case 'F': // float
        case 'I': // int
        case 'J': // long
        case 'S': // short
        case 'Z': // boolean
        case 's': // string
            return loadConstantValue(fileReader);
        case 'e':
            return loadEnumConstantValue(fileReader);
        case 'c':
            return loadClassInfoIndex(fileReader);
        case '@':
            return loadAnnotation(fileReader);
        case '[':
            return loadAnnotation(fileReader);
        }
        return false;
    }

    bool ElementValue::loadConstantValue(shared_ptr<FileReader> &fileReader)
    {
        valueType = CONSTANT_VALUE;
        int status = fileReader->readUint16(constValue);
        return true;
    }

    bool ElementValue::loadEnumConstantValue(shared_ptr<FileReader> &fileReader)
    {
        valueType = ENUM_VALUE;
        int status = fileReader->readUint16(enumConstantValue.typeNameIndex);
        status = fileReader->readUint16(enumConstantValue.constNameIndex);
        return false;
    }

    bool ElementValue::loadClassInfoIndex(shared_ptr<FileReader> &fileReader)
    {
        valueType = CLASS_INFO;
        int status = fileReader->readUint16(classInfoIndex);
        return true;
    }

    bool ElementValue::loadAnnotation(shared_ptr<FileReader> &fileReader)
    {
        valueType = ANNOTATION;
        auto annotation = Annotation::load(fileReader);
        if (annotation == nullptr)
        {
            return false;
        }
        annotationValue.reset(annotation);
        return true;
    }

    bool ElementValue::loadArray(shared_ptr<FileReader> &fileReader)
    {
        valueType = ARRAY;
        int status = fileReader->readUint16(arrayValue.count);
        arrayValue.values = new vector<unique_ptr<ElementValue>>();
        for (int i = 0; i < arrayValue.count; i++)
        {
            ElementValue *element = new ElementValue();
            if (element == nullptr) {
                return false;
            }
            if (!element->load(fileReader)) {
                delete element;
                return false;
            }
            arrayValue.values->push_back(unique_ptr<ElementValue>(element));
        }
        return true;
    }

    ////////////////////////////////////
    Annotation::Annotation(uint16_t type, uint16_t elements)
    {
        typeIndex = type;
        elementCount = elements;
    }

    /*static*/ Annotation *Annotation::load(shared_ptr<FileReader> &fileReader)
    {
        uint16_t type;
        int status = fileReader->readUint16(type);

        uint16_t pairs;
        status = fileReader->readUint16(pairs);
        auto annotation = new Annotation(type, pairs);
        if (annotation == nullptr)
        {
            return nullptr;
        }
        if (!annotation->loadPairs(fileReader))
        {
            delete annotation;
            return nullptr;
        }
        return annotation;
    }

    bool Annotation::loadPairs(shared_ptr<FileReader> &fileReader)
    {
        for (int i = 0; i < elementCount; i++)
        {
            uint16_t nameIndex;
            uint16_t type;
            int status = fileReader->readUint16(type);
            ElementPair *pair = new ElementPair(nameIndex);
            if (!pair->load(fileReader))
            {
                delete pair;
                return false;
            }
            elementPairs.push_back(pair);
        }
        return true;
    }
    ////////////////////////////////////

    RuntimeVisibleAnnotationsAttr::~RuntimeVisibleAnnotationsAttr() {
        annotations.clear();
    }

    void RuntimeVisibleAnnotationsAttr::initialize(shared_ptr<FileReader> &fileReader)
    {
        int status = fileReader->readUint16(annotationNums);
        if (status != 0)
        {
            return;
        }
        for (int i = 0; i < annotationNums; i++)
        {
            Annotation *annotation = Annotation::load(fileReader);
            if (annotation == nullptr)
            {
                annotations.clear();
                return;
            }
            annotations.push_back(unique_ptr<Annotation>(annotation));
        }
    }

}