#include <math.h>
#include <stdlib.h>
#include "dbits.h"

static const unsigned char __BIT_MASKS[8] = {
    (0x1),
    (0x2),
    (0x4),
    (0x8),
    (0x10),
    (0x20),
    (0x40),
    (0x80),
};

static unsigned_int_size_t GetUIntBitsize(UInt8 v, data_header_size_t header_size)
{
    unsigned int bitsize = (unsigned int)floor(log2(v)) + 1;
    if (bitsize > (unsigned int)exp2(header_size))
    {
        return 0;
    }
    return bitsize;
}

static char SerializeNumericalHeader(UInt8 header_value,
                                     data_header_size_t header_size,
                                     unsigned char *buffer_p,
                                     size_t buffer_size,
                                     size_t *size_off,
                                     unsigned char *bit_off)
{
    if (header_value == 0 || header_value > 8 || header_size == NO_HEADER || header_size > HEADER64_SIZE ||
        NULL == buffer_p || NULL == size_off || NULL == bit_off || *bit_off > 8 || buffer_size == 0)
    {
        return 0;
    }

    header_value -= 1;

    for (int i = 0; i < header_size; i++)
    {

        if (*bit_off == 8 && *size_off < buffer_size)
        {
            *bit_off = 0;
            *size_off += 1;
        }

        if (header_value != 0)
        {
            if (header_value & 1)
            {
                buffer_p[*size_off - 1] |= (1 << *bit_off);
            }
            header_value >>= 1;
        }

        *bit_off += 1;
    }

    return 1;
}

static char SerializeUInt(UInt64 uval,
                          unsigned char *buffer_p,
                          size_t buffer_size,
                          size_t *size_off,
                          unsigned char *bit_off)
{

    if (NULL == buffer_p || buffer_size == 0 ||
        NULL == size_off || NULL == bit_off || *bit_off > 8)
    {
        return 0;
    }

    if (*bit_off == 8)
    {
        *bit_off = 0;
        *size_off += 1;
    }

    if (uval == 0)
    {
        *bit_off += 1;
        return 1;
    }

    while (uval != 0)
    {

        if (*bit_off == 8 && *size_off < buffer_size)
        {
            *bit_off = 0;
            *size_off += 1;
        }

        if (uval & 1)
        {
            buffer_p[*size_off - 1] |= (1 << *bit_off);
        }
        uval >>= 1;
        *bit_off += 1;
    }

    return 1;
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

static char DeserializeArgCheck(const unsigned char *buffer,
                                size_t *m_bytes,
                                size_t *bit_count,
                                int bitsize)
{
    if (buffer == NULL || bit_count == NULL ||
        bitsize == 0 || m_bytes == NULL || *m_bytes == 0)
    {
        return 0;
    }
    return 1;
}

static unsigned char *DeserializeUInt(unsigned char *buffer,
                                      size_t *m_bytes,
                                      size_t *bit_count,
                                      unsigned_int_size_t uint_size,
                                      unsigned char is_header,
                                      UInt8 *out)
{
    if (out == NULL || !DeserializeArgCheck(buffer, m_bytes, bit_count, uint_size))
    {
        return NULL;
    }

    unsigned char bit_off = 0;
    *out = 0;
    while (uint_size > 0 && buffer != NULL)
    {

        if (*bit_count == 8 && *m_bytes > 0)
        {
            buffer++;
            *m_bytes -= 1;
            *bit_count = 0;
        }
        else if (*m_bytes == 0)
        {
            return NULL;
        }

        if (NULL == buffer)
        {
            return NULL;
        }

        if (*buffer & __BIT_MASKS[*bit_count])
        {
            *out += __BIT_MASKS[bit_off];
        }

        bit_off += 1;
        *bit_count += 1;
        uint_size--;
    }

    if (is_header)
    {
        *out += 1;
    }

    return buffer;
}

char DeserializeBuffer(unsigned char *buffer, const size_t buffer_size, const size_t n_bytes, dpacket_t packet_out)
{

    if (packet_out == NULL || buffer == NULL || buffer_size == 0 ||
        n_bytes == 0 || n_bytes > buffer_size)
    {
        return 0;
    }

    packet_id_t packet_id;
    const int *packet_format;

    size_t n = n_bytes,
           format_size = 0,
           bit_count = 0;

    // Read packet ID
    UInt8 header_size = 0;
    if (NULL == (buffer = DeserializeUInt(buffer, &n, &bit_count, HEADER8_SIZE, 1, &header_size)) ||
        NULL == (buffer = DeserializeUInt(buffer, &n, &bit_count, header_size, 0, &packet_id)))
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
            if (NULL == (buffer = DeserializeUInt(buffer, &n, &bit_count, HEADER8_SIZE, 1, &header_size)) ||
                NULL == (buffer = DeserializeUInt(buffer, &n, &bit_count, header_size, 0, &(data.numerical_v.u8_v))) ||
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
