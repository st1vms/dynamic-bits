package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class Int8Type extends DSerializable {

    private byte b;

    public Int8Type(byte b){
        this.b = b;
    }

    @Override
    public void setByteValue(byte b) {
        this.b = b;
    }

    @Override
    public byte getByteValue() {
        return this.b;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.INT8_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.INT8_TYPE;}
}
