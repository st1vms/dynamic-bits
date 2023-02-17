package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class UInt64Type extends DSerializable {

    private long l;

    public UInt64Type(long l){
        this.setLongValue(l);
    }

    @Override
    public void setLongValue(long l) {
        if(l < 0){
            this.l = 0;
            return;
        }
        this.l = l;
    }

    @Override
    public long getLongValue() {
        return this.l;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.UINT64_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.UINT64_TYPE;}
}
