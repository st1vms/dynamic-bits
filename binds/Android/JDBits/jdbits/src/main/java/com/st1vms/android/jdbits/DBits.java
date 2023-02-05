package com.st1vms.android.jdbits;

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

    public static native @Nullable byte[] SerializeDPacket(DPacket packet);

    public static native DPacket DeserializeBuffer(byte[] buffer);
}
