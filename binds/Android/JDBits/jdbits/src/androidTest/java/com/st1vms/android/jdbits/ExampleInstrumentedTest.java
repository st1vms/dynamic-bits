package com.st1vms.android.jdbits;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;


import com.st1vms.android.jdbits.dtypes.BooleanType;
import com.st1vms.android.jdbits.dtypes.DSerializable;
import com.st1vms.android.jdbits.dtypes.DoubleType;
import com.st1vms.android.jdbits.dtypes.Int16Type;
import com.st1vms.android.jdbits.dtypes.Int32Type;
import com.st1vms.android.jdbits.dtypes.Int64Type;
import com.st1vms.android.jdbits.dtypes.Int8Type;
import com.st1vms.android.jdbits.dtypes.UInt16Type;
import com.st1vms.android.jdbits.dtypes.UInt32Type;
import com.st1vms.android.jdbits.dtypes.UInt64Type;
import com.st1vms.android.jdbits.dtypes.UInt8Type;
import com.st1vms.android.jdbits.dtypes.UTF8StringType;

import java.util.ArrayList;
import java.util.Locale;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Test
    public void useAppContext() {
        // Context of the app under test.
        DTypes[] template = new DTypes[]{
                DTypes.UINT8_TYPE,
                DTypes.UINT16_TYPE,
                DTypes.UINT32_TYPE,
                DTypes.UINT64_TYPE,
                DTypes.INT8_TYPE,
                DTypes.INT16_TYPE,
                DTypes.INT32_TYPE,
                DTypes.INT64_TYPE,
                DTypes.BOOLEAN_TYPE,
                DTypes.DOUBLE_TYPE,
                DTypes.UTF8_STRING_TYPE
        };

        if(!DBits.RegisterPacket(0, template)){
            return;
        }


        DPacket packet = new DPacket(0, new ArrayList<DSerializable>(){
            {
                add(new UInt8Type((short) 42));
                add(new UInt16Type((short) 160));
                add(new UInt32Type((int) 4002));
                add(new UInt64Type((long) 41812));
                add(new Int8Type((byte) -42));
                add(new Int16Type((short) -160));
                add(new Int32Type((int) -4002));
                add(new Int64Type((long) -41812));
                add(new BooleanType(false));
                add(new DoubleType((double) 160.051));
                add(new UTF8StringType("I do desire we may be better strangers"));
            }
        });

        final byte[] buf = DBits.SerializeDPacket(packet);
        if(buf != null){

            StringBuilder s = new StringBuilder();
            for(byte b: buf){
                s.append(String.format(Locale.getDefault(), "0x%02x ", b));
            }
            Log.d("TAG",String.format(Locale.getDefault(),
                    "\nSerialized to %d bytes: %s", buf.length, s));
        }else{
            Log.d("TAG","Buffer is null");
        }
    }
}