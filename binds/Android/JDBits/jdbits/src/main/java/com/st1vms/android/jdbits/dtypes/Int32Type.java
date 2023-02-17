package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class Int32Type extends DSerializable {

    private int i;

    public Int32Type(int i){
       this.i = i;
    }

    @Override
    public void setIntValue(int i) {
        this.i = i;
    }

    @Override
    public int getIntValue() {
        return this.i;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.INT32_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.INT32_TYPE;}
}
