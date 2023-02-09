#include <stdio.h>
#include <string.h>
#include <float.h>
#include "dbits.h"

#define SIZE_TEST 20

static int packet_format[SIZE_TEST] = {
    BOOLEAN_STYPE,
    UINT8_STYPE,
    UINT8_STYPE,
    UINT16_STYPE,
    UINT16_STYPE,
    UINT32_STYPE,
    UINT32_STYPE,
    UINT64_STYPE,
    UINT64_STYPE,
    DOUBLE_STYPE,
    DOUBLE_STYPE,
    INT8_STYPE,
    INT8_STYPE,
    INT16_STYPE,
    INT16_STYPE,
    INT32_STYPE,
    INT32_STYPE,
    INT64_STYPE,
    INT64_STYPE,
    UTF8_STRING_STYPE};

int main()
{
    if (!InitLocks())
    {
        return -1;
    }
    dpacket_struct_t packet;

    if (!RegisterPacket(0, packet_format, SIZE_TEST) ||
        !NewPacket(&packet, 0))
    {
        return -1;
    }

    if (!AddSerializable(&packet, BOOLEAN_STYPE, (data_union_t){.boolean_v = 1}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT8_STYPE, (data_union_t){.decimal_v.u8_v = 0}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT8_STYPE, (data_union_t){.decimal_v.u8_v = UINT8_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT16_STYPE, (data_union_t){.decimal_v.u16_v = 0}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT16_STYPE, (data_union_t){.decimal_v.u16_v = UINT16_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT32_STYPE, (data_union_t){.decimal_v.u32_v = 0}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT32_STYPE, (data_union_t){.decimal_v.u32_v = UINT32_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT64_STYPE, (data_union_t){.decimal_v.u64_v = 0}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UINT64_STYPE, (data_union_t){.decimal_v.u64_v = UINT64_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, DOUBLE_STYPE, (data_union_t){.double_v = DBL_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, DOUBLE_STYPE, (data_union_t){.double_v = DBL_MIN}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT8_STYPE, (data_union_t){.decimal_v.i8_v = INT8_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT8_STYPE, (data_union_t){.decimal_v.i8_v = INT8_MIN}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT16_STYPE, (data_union_t){.decimal_v.i16_v = INT16_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT16_STYPE, (data_union_t){.decimal_v.i16_v = INT16_MIN}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT32_STYPE, (data_union_t){.decimal_v.i32_v = INT32_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT32_STYPE, (data_union_t){.decimal_v.i32_v = INT32_MIN}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT64_STYPE, (data_union_t){.decimal_v.i64_v = INT64_MAX}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, INT64_STYPE, (data_union_t){.decimal_v.i64_v = INT64_MIN}))
    {
        FreePacket(&packet);
        return -1;
    }
    if (!AddSerializable(&packet, UTF8_STRING_STYPE, (data_union_t){.utf8_str_v.utf8_string = "I do desire we may be better strangers", .utf8_str_v.length = strlen( "I do desire we may be better strangers")}))
    {
        FreePacket(&packet);
        return -1;
    }

    size_t buffer_size = 0;
    unsigned char buffer[SIZE_TEST * 100] = {0};
    if (!SerializePacket(buffer, SIZE_TEST * 100, &packet, &buffer_size))
    {
        FreePacket(&packet);
        return -1;
    }
    FreePacket(&packet);

    printf("\nsize = %ld\n", buffer_size);
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("0x%.02x ", buffer[i]);
    }
    printf("\n");

    if (!DeserializeBuffer(buffer, buffer_size, &packet))
    {
        return -1;
    }

    serializable_list_node_t *tmp = packet.data_list.first_node;
    switch (packet.packet_id)
    {
    case 0:
        if (packet.data_list.size == SIZE_TEST && tmp != NULL)
        {
            for (size_t i = 0; i < SIZE_TEST; i++)
            {
                switch (tmp->stype)
                {
                case BOOLEAN_STYPE:
                    printf("\nB = %u\n", tmp->data.boolean_v);
                    break;
                case UINT8_STYPE:
                    printf("\nU8 = %u\n", tmp->data.decimal_v.u8_v);
                    break;
                case UINT16_STYPE:
                    printf("\nU16 = %u\n", tmp->data.decimal_v.u16_v);
                    break;
                case UINT32_STYPE:
                    printf("\nU32 = %u\n", tmp->data.decimal_v.u32_v);
                    break;
                case UINT64_STYPE:
                    printf("\nU64 = %llu\n", tmp->data.decimal_v.u64_v);
                    break;
                case INT8_STYPE:
                    printf("\nI8 = %d\n", tmp->data.decimal_v.i8_v);
                    break;
                case INT16_STYPE:
                    printf("\nI16 = %d\n", tmp->data.decimal_v.i16_v);
                    break;
                case INT32_STYPE:
                    printf("\nI32 = %d\n", tmp->data.decimal_v.i32_v);
                    break;
                case INT64_STYPE:
                    printf("\nI64 = %lld\n", tmp->data.decimal_v.i64_v);
                    break;
                case DOUBLE_STYPE:
                    printf("\nD = %lf\n", tmp->data.double_v);
                    break;
                case UTF8_STRING_STYPE:
                    printf("\nUTF8 (length = %lu) = %s\n", tmp->data.utf8_str_v.length, tmp->data.utf8_str_v.utf8_string);
                    break;
                default:
                    break;
                }
                tmp = tmp->next_node;
            }
        }
        break;
    }

    FreePacket(&packet);
    return 0;
}
