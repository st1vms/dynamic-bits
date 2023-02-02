#include <stdlib.h>
#include "dserial.h"
#include "dpacket.h"
#include "dbits.h"

static unsigned char GetIntBitsize(Int64 v)
{
    if (v == 0)
    {
        return 1;
    }

    UInt64 u = 0;
    if (v < 0)
    {
        u = (UInt64)llabs(v);
    }
    else
    {
        u = (UInt64)v;
    }

    unsigned char bitsize = 0;
    while (u != 0)
    {
        bitsize++;
        u >>= 1;
    }
    return bitsize;
}

static unsigned char GetUIntBitsize(UInt64 v)
{
    if (v == 0)
    {
        return 1;
    }

    unsigned char bitsize = 0;
    while (v != 0)
    {
        bitsize++;
        v >>= 1;
    }
    return bitsize;
}

char SerializePacket(unsigned char *buffer, size_t buffer_size, dpacket_t packet, size_t *out_size)
{
    if (buffer == NULL || out_size == NULL || packet == NULL || packet->data_list.size == 0)
    {
        return 0;
    }

    *out_size = 0;

    unsigned char bit_off = 0;
    UInt8 header_bitsize = 0;

    // Serialize packet_id header
    if (!(header_bitsize = GetUIntBitsize(packet->packet_id)))
    {
        *out_size = 0;
        return 0;
    }

    // Serialize packet_id
    if (!SerializeNumericalHeader(header_bitsize, HEADER8_SIZE, buffer, buffer_size, out_size, &bit_off) ||
        !SerializeUInt(packet->packet_id, buffer, buffer_size, out_size, &bit_off))
    {
        *out_size = 0;
        return 0;
    }

    serializable_list_node_t *node = packet->data_list.first_node;
    for (size_t i = 0; node != NULL && i < packet->data_list.size; node = node->next_node, i++)
    {
        switch (node->stype)
        {
        case BOOLEAN_STYPE:
            // Serialize Boolean
            if (!SerializeBoolean(node->data.boolean_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case UINT8_STYPE:
            // Serialize UINT8 header
            if (!(header_bitsize = GetUIntBitsize(node->data.numerical_v.u8_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize UINT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER8_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeUInt(node->data.numerical_v.u8_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case UINT16_STYPE:
            // Serialize UINT8 header
            if (!(header_bitsize = GetUIntBitsize(node->data.numerical_v.u16_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize UINT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER16_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeUInt(node->data.numerical_v.u16_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case UINT32_STYPE:
            // Serialize UINT8 header
            if (!(header_bitsize = GetUIntBitsize(node->data.numerical_v.u32_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize UINT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER32_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeUInt(node->data.numerical_v.u32_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case UINT64_STYPE:
            // Serialize UINT8 header
            if (!(header_bitsize = GetUIntBitsize(node->data.numerical_v.u64_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize UINT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER64_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeUInt(node->data.numerical_v.u64_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case INT8_STYPE:
            // Serialize INT8 header
            if (!(header_bitsize = GetIntBitsize(node->data.numerical_v.i8_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize INT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER8_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeInt(node->data.numerical_v.i8_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case INT16_STYPE:
            // Serialize INT8 header
            if (!(header_bitsize = GetIntBitsize(node->data.numerical_v.i16_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize INT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER16_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeInt(node->data.numerical_v.i16_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case INT32_STYPE:
            // Serialize INT8 header
            if (!(header_bitsize = GetIntBitsize(node->data.numerical_v.i32_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize INT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER32_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeInt(node->data.numerical_v.i32_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        case INT64_STYPE:
            // Serialize INT8 header
            if (!(header_bitsize = GetIntBitsize(node->data.numerical_v.i64_v)))
            {
                *out_size = 0;
                return 0;
            }
            // Serialize INT8
            if (!SerializeNumericalHeader(header_bitsize, HEADER64_SIZE, buffer, buffer_size, out_size, &bit_off) ||
                !SerializeInt(node->data.numerical_v.i64_v, buffer, buffer_size, out_size, &bit_off))
            {
                *out_size = 0;
                return 0;
            }
            break;
        default:
            *out_size = 0;
            return 0;
        }
    }

    *out_size += 1;
    return 1;
}

char DeserializeBuffer(unsigned char *buffer, const size_t buffer_size, dpacket_t packet_out)
{

    if (packet_out == NULL || buffer == NULL || buffer_size == 0)
    {
        return 0;
    }

    packet_id_t packet_id;
    const int *packet_format;

    size_t n = buffer_size,
           format_size = 0,
           bit_count = 0;

    // Read packet ID
    UInt8 header_size = 0;
    if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER8_SIZE, 1, &header_size)) ||
        NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, header_size, 0, &packet_id)))
    {
        return 0;
    }

    // Get Packet Format using packet ID
    if (NULL == (packet_format = GetPacketFormat(packet_id, &format_size)) ||
        format_size == 0 ||
        !NewPacket(packet_out, packet_id))
    {
        return 0;
    }

    // Parse buffer using packet format, filling packet_out

    data_union_t data;

    for (; packet_format != NULL && format_size > 0; format_size--, packet_format++)
    {

        if (*packet_format == 0)
        {
            FreePacket(packet_out);
            return 0;
        }

        switch (*packet_format)
        {
        case BOOLEAN_STYPE:

            data.boolean_v = 0;
            if (NULL == (buffer = DeserializeBoolean(buffer, &n, &bit_count, &(data.boolean_v))) ||
                !AddSerializable(packet_out, BOOLEAN_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case UINT8_STYPE:

            data.numerical_v.u8_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER8_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, header_size, 0, &(data.numerical_v.u8_v))) ||
                !AddSerializable(packet_out, UINT8_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case UINT16_STYPE:

            data.numerical_v.u16_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER16_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeUInt16(buffer, &n, &bit_count, header_size, &(data.numerical_v.u16_v))) ||
                !AddSerializable(packet_out, UINT16_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case UINT32_STYPE:

            data.numerical_v.u32_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER32_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeUInt32(buffer, &n, &bit_count, header_size, &(data.numerical_v.u32_v))) ||
                !AddSerializable(packet_out, UINT32_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case UINT64_STYPE:

            data.numerical_v.u64_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER64_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeUInt64(buffer, &n, &bit_count, header_size, &(data.numerical_v.u64_v))) ||
                !AddSerializable(packet_out, UINT64_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case INT8_STYPE:

            data.numerical_v.i8_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER8_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeInt8(buffer, &n, &bit_count, header_size, &(data.numerical_v.i8_v))) ||
                !AddSerializable(packet_out, INT8_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case INT16_STYPE:

            data.numerical_v.i16_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER16_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeInt16(buffer, &n, &bit_count, header_size, &(data.numerical_v.i16_v))) ||
                !AddSerializable(packet_out, INT16_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case INT32_STYPE:

            data.numerical_v.i32_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER32_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeInt32(buffer, &n, &bit_count, header_size, &(data.numerical_v.i32_v))) ||
                !AddSerializable(packet_out, INT32_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        case INT64_STYPE:

            data.numerical_v.i64_v = 0;
            if (NULL == (buffer = DeserializeUInt8(buffer, &n, &bit_count, HEADER64_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeInt64(buffer, &n, &bit_count, header_size, &(data.numerical_v.i64_v))) ||
                !AddSerializable(packet_out, INT64_STYPE, data))
            {
                FreePacket(packet_out);
                return 0;
            }

            break;
        default:
            FreePacket(packet_out);
            return 0;
        }
    }

    return 1;
}
