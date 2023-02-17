package com.st1vms.android.jdbits.dtypes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.st1vms.android.jdbits.DTypes;

import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;

public class UTF8StringType extends DSerializable {

    private String s;

    public UTF8StringType(@NonNull String s) throws UnsupportedEncodingException{
        try{
           s.getBytes(StandardCharsets.UTF_8);
        }
        catch (Exception e){
            throw new UnsupportedEncodingException();
        }
        this.s = s;
    }

    public UTF8StringType(@NonNull byte[] bytes){
        this.s = new String(bytes, StandardCharsets.UTF_8);
    }

    @Override
    public void setUTF8StringValue(@NonNull String s) {
        this.s = s;
    }

    @Override
    public @Nullable String getUTF8StringValue() {
        return this.s;
    }

    @Override
    public DTypes getTypeEnum() {
        return DTypes.UTF8_STRING_TYPE;
    }

    public static DTypes getTypeEnumValue(){return DTypes.UTF8_STRING_TYPE;}
}
