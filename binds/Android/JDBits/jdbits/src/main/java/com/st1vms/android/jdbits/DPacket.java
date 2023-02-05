package com.st1vms.android.jdbits;

import com.st1vms.android.jdbits.dtypes.DSerializable;

import java.util.ArrayList;

public class DPacket {

    public final int packet_id;
    private final ArrayList<DSerializable> fieldsArray;

    public DPacket(int packet_id) {
        this.packet_id = packet_id;
        this.fieldsArray = new ArrayList<>(0);
    }

    public DPacket(int packet_id, ArrayList<DSerializable> s) {
        this.packet_id = packet_id;
        this.fieldsArray = s;
    }

    public boolean addField(DSerializable serializable){
        return this.fieldsArray.add(serializable);
    }

    public DSerializable getField(int i){
        return this.fieldsArray.get(i);
    }

    public int fieldCount(){
        return this.fieldsArray.size();
    }
}
