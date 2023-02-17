package com.st1vms.android.jdbits.dtypes;


import com.st1vms.android.jdbits.DTypes;

public class BooleanType extends DSerializable {

    private boolean b;

    public BooleanType(boolean b){
        this.b = b;
    }

    @Override
    public void setBoolValue(boolean b) {
        this.b = b;
    }

    @Override
    public boolean getBoolValue() {
        return this.b;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.BOOLEAN_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.BOOLEAN_TYPE;}
}
