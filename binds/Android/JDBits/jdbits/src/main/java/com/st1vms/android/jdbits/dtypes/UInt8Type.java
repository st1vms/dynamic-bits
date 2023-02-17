package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class UInt8Type extends DSerializable {

    private short s;

    public UInt8Type(short s){
        this.setShortValue(s);
    }

    @Override
    public void setShortValue(short s) {
        if(s < 0 || s > (1<<8)){
            this.s = 0;
            return;
        }
        this.s = s;
    }

    @Override
    public short getShortValue() {
        return this.s;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.UINT8_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.UINT8_TYPE;}
}
