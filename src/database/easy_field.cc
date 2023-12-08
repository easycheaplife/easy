#include "easy_field.h"

namespace easy {


void EasyField::SetByteValue( const void* newValue, const size_t newSize, enum_field_types newType, easy_uint32 length ) {
    if (data.value)
        CleanUp();

    // This value stores raw bytes that have to be explicitly casted later
    if (newValue) {
        data.value = new char[newSize];
        memcpy(data.value, newValue, newSize);
        data.length = length;
    }
    data.type = newType;
    data.raw = true;
}

void EasyField::SetStructuredValue( char* newValue, enum_field_types newType ) {
    if (data.value)
        CleanUp();

    // This value stores somewhat structured data that needs function style casting
    if (newValue) {
        size_t size = strlen(newValue);
        data.value = new char [size+1];
        strcpy((char*)data.value, newValue);
        data.length = size;
    }

    data.type = newType;
    data.raw = false;
}

EasyField::EasyField() {
    data.value = NULL;
    data.type = MYSQL_TYPE_NULL;
    data.length = 0;
}

EasyField::~EasyField() {
    CleanUp();
}

}