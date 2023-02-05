package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class Int16Type extends DSerializable {

    private short s;

    public Int16Type(short s){
        this.s = s;
    }

    @Override
    public void setShortValue(short s) {
        this.s = s;
    }

    @Override
    public short getShortValue() {
        return this.s;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.INT16_TYPE;
    }
}
