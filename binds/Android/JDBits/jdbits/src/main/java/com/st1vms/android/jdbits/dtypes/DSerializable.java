package com.st1vms.android.jdbits.dtypes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.st1vms.android.jdbits.DTypes;

import kotlin.NotImplementedError;

public abstract class DSerializable {

    void setBoolValue(boolean b){
        throw new NotImplementedError();
    }
    void setByteValue(byte b){
        throw new NotImplementedError();
    }

    void setShortValue(short s){
        throw new NotImplementedError();
    }

    void setIntValue(int i){
        throw new NotImplementedError();
    }

    void setLongValue(long l){
        throw new NotImplementedError();
    }

    void setDoubleValue(double l){
        throw new NotImplementedError();
    }

    void setUTF8StringValue(@NonNull String s){
        throw new NotImplementedError();
    }

    boolean getBoolValue(){
        throw new NotImplementedError();
    }
    byte getByteValue(){
        throw new NotImplementedError();
    }
    short getShortValue(){
        throw new NotImplementedError();
    }
    int getIntValue(){
        throw new NotImplementedError();
    }
    long getLongValue(){
        throw new NotImplementedError();
    }

    double getDoubleValue(){
        throw new NotImplementedError();
    }

    @Nullable String getUTF8StringValue(){
        throw new NotImplementedError();
    }

    DTypes getTypeEnum(){
        throw new NotImplementedError();
    }
}
