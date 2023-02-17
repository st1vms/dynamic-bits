package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class Int64Type extends DSerializable {

    private long l;

    public Int64Type(long l){
        this.l = l;
    }

    @Override
    public void setLongValue(long l) {
        this.l = l;
    }

    @Override
    public long getLongValue() {
        return this.l;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.INT64_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.INT64_TYPE;}
}
