#pragma once
#include <cstdint>

enum AttributeType {
    invalid_type,
    byte_type,           /* BYTE */
    unsigned_byte_type,  /* UNSIGNED_BYTE */
    short_type,          /* SHORT */
    unsigned_short_type, /* UNSIGNED_SHORT */
    unsigned_int_type,   /* UNSIGNED_INT */
    float_type,          /* FLOAT */
    max_enum_type
};

struct Attribute {
    uint32_t offset;
    AttributeType type;
    uint8_t count;
    bool normalize;
};

struct VertexLayout {
    Attribute attributes[8];
    uint32_t stride;
    uint8_t attributeCount;

    // maybe need create hash for operator ==
    bool operator==(const VertexLayout& other) const {
        if (stride != other.stride || attributeCount != other.attributeCount) 
            return false;
        for(int i = 0; i < attributeCount; ++i) {
            if (
                attributes[i].offset != other.attributes[i].offset ||
                attributes[i].type != other.attributes[i].type ||
                attributes[i].count != other.attributes[i].count ||
                attributes[i].normalize != other.attributes[i].normalize
            ) return false;
        }
        return true;
    }

};