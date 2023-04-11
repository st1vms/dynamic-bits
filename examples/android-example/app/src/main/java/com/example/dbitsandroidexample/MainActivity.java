package com.example.dbitsandroidexample;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import com.st1vms.android.jdbits.DBits;
import com.st1vms.android.jdbits.DPacket;
import com.st1vms.android.jdbits.DTypes;
import com.st1vms.android.jdbits.dtypes.BooleanType;
import com.st1vms.android.jdbits.dtypes.DSerializable;
import com.st1vms.android.jdbits.dtypes.DoubleType;
import com.st1vms.android.jdbits.dtypes.Int8Type;
import com.st1vms.android.jdbits.dtypes.UInt8Type;
import com.st1vms.android.jdbits.dtypes.UTF8StringType;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    private String bytesToHexString(final byte[] bytes){
        StringBuilder sb = new StringBuilder();
        for(byte b : bytes){
            sb.append(String.format(Locale.getDefault(), "%02x", b));
        }
        return sb.toString();
    }

    private static final String LOG_TAG = "LOG";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Packet registration
        // All packets must be registered before any de/serialization work
        DBits.RegisterPacket(0, new DTypes[]{
                DTypes.BOOLEAN_TYPE,
                DTypes.DOUBLE_TYPE,
                DTypes.UINT8_TYPE,
                DTypes.INT8_TYPE,
                DTypes.UTF8_STRING_TYPE
        });

        UTF8StringType stringSerializable;
        try {
            stringSerializable = new UTF8StringType("I do desire we may be better strangers");
        }catch (UnsupportedEncodingException e){
            e.printStackTrace();
            return;
        }
        // A packet consists of an array of DSerializable
        // Each field has it's own DSerializable constructor
        final ArrayList<DSerializable> packet = new ArrayList<DSerializable>(0){
            {
                add(new BooleanType(true));
                add( new DoubleType(3.1415926));
                add(new UInt8Type((short)256));
                add(new Int8Type((byte)-128));
                add(stringSerializable);
            }
        };

        // Perform serialization into a byte array
        final byte[] buffer = DBits.SerializeDPacket(new DPacket(0, packet));
        if(buffer == null){
            // Serialization error
            return;
        }

        // Prints buffer
        Log.d(LOG_TAG, String.format("Serialized packet to -> %s",bytesToHexString(buffer)));


        // Deserialize buffer, reading no more than buffer.length bytes
        final DPacket resPacket = DBits.DeserializeBuffer(buffer, buffer.length);
        if(resPacket == null){
            // Deserialization error
            return;
        }

        // Check packet id and field count
        if(resPacket.packet_id != 0 || resPacket.fieldCount() != 5){
            // Malformed packet
            return;
        }

        BooleanType b = (BooleanType) resPacket.getField(0);
        DoubleType d = (DoubleType) resPacket.getField(1);
        UInt8Type u8 = (UInt8Type) resPacket.getField(2);
        Int8Type i8 = (Int8Type) resPacket.getField(3);
        UTF8StringType s = (UTF8StringType) resPacket.getField(4);
        // Extra null check
        if(null == b || null == d || null == u8 || null == i8 || null == s){
            return;
        }
        Log.d(LOG_TAG, String.format(Locale.getDefault(), "Packet boolean -> %b", b.getBoolValue()));
        Log.d(LOG_TAG, String.format(Locale.getDefault(), "Packet double -> %f",d.getDoubleValue()));
        Log.d(LOG_TAG, String.format(Locale.getDefault(), "Packet UINT8 -> %d", u8.getShortValue()));
        Log.d(LOG_TAG, String.format(Locale.getDefault(), "Packet INT8 -> %b", i8.getByteValue()));
        Log.d(LOG_TAG, String.format(Locale.getDefault(), "Packet String -> %s", s.getUTF8StringValue()));
    }
}