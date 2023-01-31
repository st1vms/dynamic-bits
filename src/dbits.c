#include <math.h>
#include <stdlib.h>
#include "dserial.h"
#include "dbits.h"

static unsigned_int_size_t GetUIntBitsize(UInt8 v, data_header_size_t header_size)
{
    unsigned int bitsize = (unsigned int)floor(log2(v)) + 1;
    if (bitsize > (unsigned int)exp2(header_size))
    {
        return 0;
    }
    return bitsize;
}

char SerializePacket(unsigned char *buffer, size_t buffer_size, dpacket_t packet, size_t *out_size)
{
    if (buffer == NULL || out_size == NULL || packet == NULL || packet->data_deque.size == 0)
    {
        return 0;
    }

    *out_size = 1;

    unsigned char bit_off = 0;
    unsigned int header_bitsize = 0;

    // Serialize packet_id header
    if (!(header_bitsize = GetUIntBitsize(packet->packet_id, HEADER8_SIZE)))
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

    serializable_list_node_t *node = packet->data_deque.first_node;
    for (size_t i = 0; node != NULL && i < packet->data_deque.size; node = node->next_node, i++)
    {
        switch (node->stype)
        {
        case UINT8_STYPE:
            // Serialize UINT8 header
            if (!(header_bitsize = GetUIntBitsize(node->data.numerical_v.u8_v, HEADER8_SIZE)))
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
        default:
            *out_size = 0;
            return 0;
        }
    }

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

    for (; packet_format != NULL && format_size > 0; format_size--)
    {

        if (*packet_format == 0)
        {
            FreePacket(packet_out);
            return 0;
        }

        switch (*packet_format)
        {
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
        default:
            FreePacket(packet_out);
            return 0;
        }
    }

    return 1;
}
