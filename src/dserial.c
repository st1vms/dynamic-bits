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


static void SizeIncrementCheck(size_t *size_off, size_t buffer_size, unsigned char *bit_off){
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
    if (header_value == 0 || header_value > 8 || header_size == NO_HEADER || header_size > HEADER64_SIZE ||
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
                buffer_p[*size_off - 1] |= (1 << *bit_off);
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
            buffer_p[*size_off - 1] |= (1 << *bit_off);
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

static void ByteIncrementCheck(unsigned char **buffer, size_t *bit_count, size_t* m_bytes){
    if (buffer != NULL && *bit_count >= 8 && *m_bytes > 1)
    {
        *buffer += 1;
        *m_bytes -= 1;
        *bit_count = 0;
    }
}

unsigned char *DeserializeUInt8(unsigned char *buffer,
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

        ByteIncrementCheck(&buffer, bit_count, m_bytes);

        if (*m_bytes == 0 || NULL == buffer)
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
