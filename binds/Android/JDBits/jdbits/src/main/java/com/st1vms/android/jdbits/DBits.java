package com.st1vms.android.jdbits;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

public class DBits {

    static {
        System.loadLibrary("jdbits");
    }

    private static native void nativeInit();

    static {
        nativeInit();
    }

    public static native boolean RegisterPacket(int packet_id, final DTypes[] fieldTypeArray);

    public static native @Nullable byte[] SerializeDPacket(@NonNull DPacket packet);

    public static native @Nullable DPacket DeserializeBuffer(@NonNull byte[] buffer, int buffer_size);
}
