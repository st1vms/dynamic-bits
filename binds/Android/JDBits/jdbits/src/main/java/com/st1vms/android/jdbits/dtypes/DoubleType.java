package com.st1vms.android.jdbits.dtypes;


import com.st1vms.android.jdbits.DTypes;

public class DoubleType extends DSerializable {

    private double d;

    public DoubleType(double d){
        this.d = d;
    }

    @Override
    public void setDoubleValue(double d) {
        this.d = d;
    }

    @Override
    public double getDoubleValue() {
        return this.d;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.DOUBLE_TYPE;
    }
}
