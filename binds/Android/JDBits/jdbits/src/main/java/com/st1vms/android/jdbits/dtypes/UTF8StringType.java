package com.st1vms.android.jdbits.dtypes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.st1vms.android.jdbits.DTypes;

public class UTF8StringType extends DSerializable {

    private String s;

    public UTF8StringType(@NonNull String s){
        this.s = s;
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
}
