package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class UInt16Type extends DSerializable {

    private int i;

    public UInt16Type(int i){
        this.setIntValue(i);
    }

    @Override
    public void setIntValue(int i) {
        if(i < 0 || i > (1<<16)){
            this.i = 0;
            return;
        }
        this.i = i;
    }

    @Override
    public int getIntValue() {
        return this.i;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.UINT16_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.UINT16_TYPE;}
}
