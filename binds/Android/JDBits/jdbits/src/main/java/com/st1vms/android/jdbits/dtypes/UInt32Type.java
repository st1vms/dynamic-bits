package com.st1vms.android.jdbits.dtypes;

import com.st1vms.android.jdbits.DTypes;

public class UInt32Type extends DSerializable {

    private int i;

    public UInt32Type(int i){
        this.setIntValue(i);
    }

    @Override
    public void setIntValue(int i) {
        if(i < 0){
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
        return DTypes.UINT32_TYPE;
    }
}
