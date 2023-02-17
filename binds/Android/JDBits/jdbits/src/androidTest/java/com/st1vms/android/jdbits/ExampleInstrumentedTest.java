package com.st1vms.android.jdbits;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;


import com.st1vms.android.jdbits.DBits;
import com.st1vms.android.jdbits.DPacket;
import com.st1vms.android.jdbits.DTypes;
import com.st1vms.android.jdbits.dtypes.BooleanType;
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

import java.io.UnsupportedEncodingException;
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
    public void useAppContext() throws UnsupportedEncodingException {
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


        DPacket packet = new DPacket(0, new ArrayList<>(){
            {
                add(new UInt8Type((short) 42));
                add(new UInt16Type((short) 160));
                add(new UInt32Type(4002));
                add(new UInt64Type(41812));
                add(new Int8Type((byte) -42));
                add(new Int16Type((short) -160));
                add(new Int32Type(-4002));
                add(new Int64Type(-41812));
                add(new BooleanType(false));
                add(new DoubleType(160.051));
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

            try {

                DPacket p = DBits.DeserializeBuffer(buf, buf.length);
                if(p == null){
                    Log.d("TAG","Deserialize Error!");
                    return;
                }
                UInt8Type u8 = (UInt8Type) p.getField(0);
                Log.d("TAG",String.format(Locale.getDefault(), "U8 -> %d",u8.getShortValue()));

                UInt16Type u16 = (UInt16Type) p.getField(1);
                Log.d("TAG",String.format(Locale.getDefault(), "U16 -> %d",u16.getIntValue()));

                UInt32Type u32 = (UInt32Type) p.getField(2);
                Log.d("TAG",String.format(Locale.getDefault(), "U32 -> %d",u32.getIntValue()));

                UInt64Type u64 = (UInt64Type) p.getField(3);
                Log.d("TAG",String.format(Locale.getDefault(), "U64 -> %d",u64.getLongValue()));

                Int8Type i8 = (Int8Type) p.getField(4);
                Log.d("TAG",String.format(Locale.getDefault(), "I8 -> %d",i8.getByteValue()));

                Int16Type i16 = (Int16Type) p.getField(5);
                Log.d("TAG",String.format(Locale.getDefault(), "I16 -> %d",i16.getShortValue()));

                Int32Type i32 = (Int32Type) p.getField(6);
                Log.d("TAG",String.format(Locale.getDefault(), "I32 -> %d",i32.getIntValue()));

                Int64Type i64 = (Int64Type) p.getField(7);
                Log.d("TAG",String.format(Locale.getDefault(), "I64 -> %d",i64.getLongValue()));

                BooleanType b = (BooleanType) p.getField(8);
                Log.d("TAG",String.format(Locale.getDefault(), "B -> %b",b.getBoolValue()));

                DoubleType d = (DoubleType) p.getField(9);
                Log.d("TAG",String.format(Locale.getDefault(), "D -> %f",d.getDoubleValue()));

                UTF8StringType str = (UTF8StringType) p.getField(10);
                Log.d("TAG",String.format(Locale.getDefault(), "UTF8 -> %s",str.getUTF8StringValue()));

            }catch (Exception e){
                e.printStackTrace();
            }

        }else{
            Log.d("TAG","Buffer is null");
        }
    }
}