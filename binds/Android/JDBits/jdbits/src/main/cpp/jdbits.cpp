#include <jni.h>
#include <string>
#include "dbits.h"

static const size_t LOCAL_BUFFER_SIZE = MAX_STRING_LENGTH + (MAX_PACKET_FIELDS * 80);
static jclass dpacket_cls;
static jfieldID dpacket_id_field;
static jmethodID dpacket_constructor;
static jmethodID dpacket_serials_getter;
static jmethodID dpacket_serials_setter;
static jmethodID dpacket_serials_count;

static jclass serializable_cls;
static jmethodID serializable_field_type_getter;

static jmethodID serializable_byte_getter;
static jmethodID serializable_short_getter;
static jmethodID serializable_int_getter;
static jmethodID serializable_long_getter;
static jmethodID serializable_bool_getter;
static jmethodID serializable_double_getter;
static jmethodID serializable_utf8_string_getter;

static jclass dtypes_cls;
static jmethodID dtypes_getter;

static jclass uint8_cls;
static jmethodID uint8_constructor;

static jclass uint16_cls;
static jmethodID uint16_constructor;

static jclass uint32_cls;
static jmethodID uint32_constructor;

static jclass uint64_cls;
static jmethodID uint64_constructor;

static jclass int8_cls;
static jmethodID int8_constructor;

static jclass int16_cls;
static jmethodID int16_constructor;

static jclass int32_cls;
static jmethodID int32_constructor;

static jclass int64_cls;
static jmethodID int64_constructor;

static jclass bool_cls;
static jmethodID bool_constructor;

static jclass double_cls;
static jmethodID double_constructor;

static jclass utf8_string_cls;
static jmethodID utf8_string_constructor;


extern "C"
JNIEXPORT void JNICALL
Java_com_st1vms_android_jdbits_DBits_nativeInit(JNIEnv *env, jclass clazz) {

    InitLocks();

    jclass _dpacket_cls = env->FindClass("com/st1vms/android/jdbits/DPacket");
    dpacket_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_dpacket_cls));
    dpacket_id_field = env->GetFieldID(dpacket_cls, "packet_id", "I");
    dpacket_constructor = env->GetMethodID(dpacket_cls, "<init>", "(I)V");
    dpacket_serials_getter = env->GetMethodID(dpacket_cls, "getField",
                                              "(I)Lcom/st1vms/android/jdbits/dtypes/DSerializable;");
    dpacket_serials_setter = env->GetMethodID(dpacket_cls, "addField",
                                              "(Lcom/st1vms/android/jdbits/dtypes/DSerializable;)Z");
    dpacket_serials_count = env->GetMethodID(dpacket_cls, "fieldCount", "()I");

    jclass _serializable_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/DSerializable");
    serializable_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_serializable_cls));
    serializable_field_type_getter = env->GetMethodID(serializable_cls, "getTypeEnum",
                                                      "()Lcom/st1vms/android/jdbits/DTypes;");
    serializable_byte_getter = env->GetMethodID(serializable_cls, "getByteValue", "()B");
    serializable_short_getter = env->GetMethodID(serializable_cls, "getShortValue", "()S");
    serializable_int_getter = env->GetMethodID(serializable_cls, "getIntValue", "()I");
    serializable_long_getter = env->GetMethodID(serializable_cls, "getLongValue", "()J");
    serializable_bool_getter = env->GetMethodID(serializable_cls, "getBoolValue", "()Z");
    serializable_double_getter = env->GetMethodID(serializable_cls, "getDoubleValue", "()D");
    serializable_utf8_string_getter = env->GetMethodID(serializable_cls, "getUTF8StringValue",
                                                       "()Ljava/lang/String;");

    jclass _dtypes_cls = env->FindClass("com/st1vms/android/jdbits/DTypes");
    dtypes_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_dtypes_cls));
    dtypes_getter = env->GetMethodID(dtypes_cls, "getTypeID", "()I");

    jclass _uint8_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/UInt8Type");
    uint8_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_uint8_cls));
    uint8_constructor = env->GetMethodID(uint8_cls, "<init>", "(S)V");

    jclass _uint16_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/UInt16Type");
    uint16_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_uint16_cls));
    uint16_constructor = env->GetMethodID(uint16_cls, "<init>", "(I)V");

    jclass _uint32_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/UInt32Type");
    uint32_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_uint32_cls));
    uint32_constructor = env->GetMethodID(uint32_cls, "<init>", "(I)V");

    jclass _uint64_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/UInt64Type");
    uint64_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_uint64_cls));
    uint64_constructor = env->GetMethodID(uint64_cls, "<init>", "(J)V");

    jclass _int8_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/Int8Type");
    int8_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_int8_cls));
    int8_constructor = env->GetMethodID(int8_cls, "<init>", "(B)V");

    jclass _int16_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/Int16Type");
    int16_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_int16_cls));
    int16_constructor = env->GetMethodID(int16_cls, "<init>", "(S)V");

    jclass _int32_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/Int32Type");
    int32_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_int32_cls));
    int32_constructor = env->GetMethodID(int32_cls, "<init>", "(I)V");

    jclass _int64_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/Int64Type");
    int64_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_int64_cls));
    int64_constructor = env->GetMethodID(int64_cls, "<init>", "(J)V");

    jclass _bool_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/BooleanType");
    bool_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_bool_cls));
    bool_constructor = env->GetMethodID(bool_cls, "<init>", "(Z)V");

    jclass _double_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/DoubleType");
    double_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_double_cls));
    double_constructor = env->GetMethodID(double_cls, "<init>", "(D)V");

    jclass _utf8_string_cls = env->FindClass("com/st1vms/android/jdbits/dtypes/UTF8StringType");
    utf8_string_cls = reinterpret_cast<jclass>(env->NewGlobalRef(_utf8_string_cls));
    utf8_string_constructor = env->GetMethodID(utf8_string_cls, "<init>", "([B)V");

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_st1vms_android_jdbits_DBits_RegisterPacket(JNIEnv *env, jclass clazz, jint packet_id, jobjectArray field_type_array) {
    jsize array_size = env->GetArrayLength(field_type_array);
    if (array_size == 0 || array_size > MAX_PACKET_FIELDS) {
        return 0;
    }

    int field_types[array_size];

    for (int i = 0; i < array_size; i++) {
        field_types[i] = env->CallIntMethod(
                env->GetObjectArrayElement(field_type_array, i),
                dtypes_getter);
    }
    return RegisterPacket(packet_id, field_types, array_size) != 0;
}


extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_st1vms_android_jdbits_DBits_SerializeDPacket(JNIEnv *env, jclass clazz, jobject packet) {

    int packet_id = env->GetIntField(packet, dpacket_id_field);

    jsize fieldListSize = env->CallIntMethod(packet, dpacket_serials_count);
    if (fieldListSize == 0 || fieldListSize > MAX_PACKET_FIELDS) {
        return nullptr;
    }

    dpacket_struct_t dpacket;
    if (!NewPacket(&dpacket, packet_id)) {
        return nullptr;
    }
    data_union_t dataUnion;

    jstring stringValue;
    jobject stypeEnum;
    jobject serializable;
    for (jsize i = 0; i < fieldListSize; i++) {
        serializable = env->CallObjectMethod(packet, dpacket_serials_getter, i);
        stypeEnum = env->CallObjectMethod(serializable, serializable_field_type_getter);
        switch (env->CallIntMethod(stypeEnum, dtypes_getter)) {
            case UINT8_STYPE:
                dataUnion.decimal_v.u8_v = static_cast<uint8_t>(env->CallShortMethod(serializable,
                                                                serializable_short_getter));
                if (!AddSerializable(&dpacket, UINT8_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT16_STYPE:
                dataUnion.decimal_v.u16_v = static_cast<uint16_t>(env->CallIntMethod(serializable,
                                                                 serializable_int_getter));
                if (!AddSerializable(&dpacket, UINT16_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT32_STYPE:
                dataUnion.decimal_v.u32_v = static_cast<uint32_t>(env->CallIntMethod(serializable,
                                                               serializable_int_getter));
                if (!AddSerializable(&dpacket, UINT32_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT64_STYPE:
                dataUnion.decimal_v.u64_v = static_cast<uint64_t>(env->CallLongMethod(serializable,
                                                                serializable_long_getter));
                if (!AddSerializable(&dpacket, UINT64_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT8_STYPE:
                dataUnion.decimal_v.i8_v = env->CallByteMethod(serializable,
                                                               serializable_byte_getter);
                if (!AddSerializable(&dpacket, INT8_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT16_STYPE:
                dataUnion.decimal_v.i16_v = env->CallShortMethod(serializable,
                                                                 serializable_short_getter);
                if (!AddSerializable(&dpacket, INT16_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT32_STYPE:
                dataUnion.decimal_v.i32_v = env->CallIntMethod(serializable,
                                                               serializable_int_getter);
                if (!AddSerializable(&dpacket, INT32_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT64_STYPE:
                dataUnion.decimal_v.i64_v = env->CallLongMethod(serializable,
                                                                serializable_long_getter);
                if (!AddSerializable(&dpacket, INT64_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case DOUBLE_STYPE:
                dataUnion.double_v = env->CallDoubleMethod(serializable,
                                                           serializable_double_getter);
                if (!AddSerializable(&dpacket, DOUBLE_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case BOOLEAN_STYPE:
                if (env->CallBooleanMethod(serializable, serializable_bool_getter)) {
                    dataUnion.boolean_v = 1;
                } else {
                    dataUnion.boolean_v = 0;
                }

                if (!AddSerializable(&dpacket, BOOLEAN_STYPE, dataUnion)) {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UTF8_STRING_STYPE:
                stringValue = (jstring) env->CallObjectMethod(serializable,
                                                              serializable_utf8_string_getter);
                jboolean isCopy;
                const char *stringP = env->GetStringUTFChars(
                        stringValue, &isCopy);

                size_t stringSize = env->GetStringLength(stringValue);

                if(stringSize == 0 || stringSize >= MAX_STRING_LENGTH){
                    env->ReleaseStringUTFChars(stringValue, stringP);
                    return nullptr;
                }
                if (!AddUTF8StringSerializable(&dpacket,
                                               reinterpret_cast<const unsigned char *>(stringP),
                                               stringSize)) {
                    env->ReleaseStringUTFChars(stringValue, stringP);
                    FreePacket(&dpacket);
                    return nullptr;
                }
                env->ReleaseStringUTFChars(stringValue, stringP);
                break;
        }
    }

    jsize buffer_size = 0;
    unsigned char buf[LOCAL_BUFFER_SIZE];
    memset(buf, 0, LOCAL_BUFFER_SIZE);
    if (!SerializePacket(buf, LOCAL_BUFFER_SIZE, &dpacket,
                         reinterpret_cast<size_t *>(&buffer_size)) ||
        buffer_size >= LOCAL_BUFFER_SIZE) {
        FreePacket(&dpacket);
        return nullptr;
    }
    FreePacket(&dpacket);

    jbyteArray outArray = env->NewByteArray(buffer_size);
    env->SetByteArrayRegion(outArray, 0, buffer_size, reinterpret_cast<jbyte *>(buf));

    return outArray;
}

void UCharArrayToCharArray(const unsigned char uarr[], char out[], size_t arr_size){
    for(size_t i = 0; i < arr_size; i++){
        out[i] = static_cast<char>(uarr[i]);
    }
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_st1vms_android_jdbits_DBits_DeserializeBuffer(JNIEnv *env, jclass clazz, jbyteArray buffer,
                                                       jint buffer_size) {

    if(buffer == nullptr || buffer_size == 0 || buffer_size > LOCAL_BUFFER_SIZE ||
        buffer_size > env->GetArrayLength(buffer)){
        return nullptr;
    }

    unsigned char buf[buffer_size];
    env->GetByteArrayRegion(buffer, 0, buffer_size, reinterpret_cast<jbyte*>(buf));

    dpacket_struct_t dpacket;
    if(!DeserializeBuffer(buf, buffer_size, &dpacket)){
        return nullptr;
    }

    jobject dpacket_out = env->NewObject(dpacket_cls, dpacket_constructor, dpacket.packet_id);
    if(dpacket_out == nullptr){
        FreePacket(&dpacket);
        return nullptr;
    }

    jbyteArray stringBytes;
    serializable_list_node_t * tmp = dpacket.data_list.first_node;
    for(size_t i = 0; tmp != nullptr && i < dpacket.data_list.size; tmp = tmp->next_node, i++){

        switch (tmp->stype) {
            case UINT8_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                               env->NewObject(uint8_cls, uint8_constructor, tmp->data.decimal_v.u8_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT16_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(uint16_cls, uint16_constructor, tmp->data.decimal_v.u16_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT32_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(uint32_cls, uint32_constructor,
                                                         static_cast<jint>(tmp->data.decimal_v.u32_v))))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UINT64_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(uint64_cls, uint64_constructor,
                                                          static_cast<jlong>(tmp->data.decimal_v.u64_v))))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT8_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(int8_cls, int8_constructor, tmp->data.decimal_v.i8_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT16_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(int16_cls, int16_constructor, tmp->data.decimal_v.i16_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT32_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(int32_cls, int32_constructor, tmp->data.decimal_v.i32_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case INT64_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(int64_cls, int64_constructor, tmp->data.decimal_v.i64_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
               break;
            case DOUBLE_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(double_cls, double_constructor, tmp->data.double_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case BOOLEAN_STYPE:
                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(bool_cls, bool_constructor, tmp->data.boolean_v)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case UTF8_STRING_STYPE:
                stringBytes = env->NewByteArray(static_cast<jsize>(tmp->data.utf8_str_v.length));
                if(stringBytes == nullptr){
                    FreePacket(&dpacket);
                    return nullptr;
                }
                env->SetByteArrayRegion(stringBytes, 0,
                                        static_cast<jsize>(tmp->data.utf8_str_v.length),
                                        reinterpret_cast<const jbyte*>(tmp->data.utf8_str_v.utf8_string));

                if(!env->CallBooleanMethod(dpacket_out, dpacket_serials_setter,
                                           env->NewObject(utf8_string_cls, utf8_string_constructor, stringBytes)))
                {
                    FreePacket(&dpacket);
                    return nullptr;
                }
                break;
            case NO_TYPE:
                FreePacket(&dpacket);
                return nullptr;
        }
    }

    FreePacket(&dpacket);
    return dpacket_out;
}
