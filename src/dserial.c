#include "dtypes.h"

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

static void SizeIncrementCheck(size_t *size_off, size_t buffer_size, unsigned char *bit_off)
{

    if (*bit_off == 8 && *size_off < buffer_size)
    {
        *bit_off = 0;
        *size_off += 1;
    }
}

char SerializeNumericalHeader(UInt8 header_value,
                              data_header_size_t header_size,
                              unsigned char *buffer_p,
                              size_t buffer_size,
                              size_t *size_off,
                              unsigned char *bit_off)
{
    if (header_value == 0 || header_size == NO_HEADER || header_size > HEADER64_SIZE ||
        NULL == buffer_p || NULL == size_off || NULL == bit_off || *bit_off > 8 || buffer_size == 0)
    {
        return 0;
    }

    header_value -= 1;

    for (int i = 0; i < header_size; i++)
    {

        SizeIncrementCheck(size_off, buffer_size, bit_off);

        if (header_value != 0)
        {
            if (header_value & 1)
            {
                buffer_p[*size_off] |= (1 << *bit_off);
            }
            header_value >>= 1;
        }

        *bit_off += 1;
    }

    return 1;
}

char SerializeUInt(UInt64 uval,
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

    SizeIncrementCheck(size_off, buffer_size, bit_off);

    if (uval == 0)
    {
        *bit_off += 1;
        return 1;
    }

    while (uval != 0)
    {
        SizeIncrementCheck(size_off, buffer_size, bit_off);

        if (uval & 1)
        {
            buffer_p[*size_off] |= (1 << *bit_off);
        }
        uval >>= 1;
        *bit_off += 1;
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

static void ByteIncrementCheck(unsigned char **buffer, size_t *bit_count, size_t *m_bytes)
{
    if (buffer != NULL && bit_count != NULL && m_bytes != NULL &&
        *bit_count >= 8 && *m_bytes > 0)
    {
        *buffer += 1;
        *m_bytes -= 1;
        *bit_count = 0;
    }
}

static unsigned char *IncrementUnsignedInteger(unsigned char *buffer,
                                               size_t *m_bytes,
                                               size_t *bit_count,
                                               unsigned char int_bitsize,
                                               uintptr_t *out)
{
    if (out == NULL || !DeserializeArgCheck(buffer, m_bytes, bit_count, int_bitsize))
    {
        return NULL;
    }

    for (unsigned char bit_pos = 0;
         bit_pos < int_bitsize && buffer != NULL && bit_count != NULL;
         bit_pos++)
    {
        ByteIncrementCheck(&buffer, bit_count, m_bytes);

        if (NULL == buffer || bit_count == NULL || out == NULL)
        {
            return NULL;
        }

        if (*buffer & __BIT_MASKS[*bit_count])
        {
            *out += (1ULL << bit_pos);
        }
        *bit_count += 1;
    }
    return buffer;
}

unsigned char *DeserializeUInt8(unsigned char *buffer,
                                size_t *m_bytes,
                                size_t *bit_count,
                                unsigned_int_size_t uint_size,
                                unsigned char is_header,
                                UInt8 *out)
{
    UInt8 *v = out;
    if (v == NULL)
    {
        return NULL;
    }
    *v = is_header ? 1 : 0;
    return IncrementUnsignedInteger(buffer, m_bytes, bit_count, uint_size, (uintptr_t *)v);
}

unsigned char *DeserializeUInt16(unsigned char *buffer,
                                 size_t *m_bytes,
                                 size_t *bit_count,
                                 unsigned_int_size_t uint_size,
                                 UInt16 *out)
{
    UInt16 *v = out;
    if (v == NULL)
    {
        return NULL;
    }
    *v = 0;
    return IncrementUnsignedInteger(buffer, m_bytes, bit_count, uint_size, (uintptr_t *)v);
}

unsigned char *DeserializeUInt32(unsigned char *buffer,
                                 size_t *m_bytes,
                                 size_t *bit_count,
                                 unsigned_int_size_t uint_size,
                                 UInt32 *out)
{
    UInt32 *v = out;
    if (v == NULL)
    {
        return NULL;
    }
    *v = 0;
    return IncrementUnsignedInteger(buffer, m_bytes, bit_count, uint_size, (uintptr_t *)v);
}

unsigned char *DeserializeUInt64(unsigned char *buffer,
                                 size_t *m_bytes,
                                 size_t *bit_count,
                                 unsigned_int_size_t uint_size,
                                 UInt64 *out)
{
    UInt64 *v = out;
    if (v == NULL)
    {
        return NULL;
    }
    *v = 0;
    return IncrementUnsignedInteger(buffer, m_bytes, bit_count, uint_size, (uintptr_t *)v);
}
