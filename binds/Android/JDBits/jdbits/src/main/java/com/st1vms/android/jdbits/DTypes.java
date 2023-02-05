package com.st1vms.android.jdbits;

public enum DTypes {

    UINT8_TYPE(0x1),
    UINT16_TYPE(0x2),
    UINT32_TYPE(0x3),
    UINT64_TYPE(0x4),
    INT8_TYPE(0x5),
    INT16_TYPE(0x6),
    INT32_TYPE(0x7),
    INT64_TYPE(0x8),
    DOUBLE_TYPE(0x9),
    BOOLEAN_TYPE(0xa),
    UTF8_STRING_TYPE(0xb);

    private final int typeID;
    DTypes(int typeID){
        this.typeID = typeID;
    }

    public int getTypeID(){
        return this.typeID;
    }
}
