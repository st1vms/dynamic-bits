#ifndef __DSERIAL_H

#include "dtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    char SerializeNumericalHeader(UInt8 header_value,
                                  data_header_size_t header_size,
                                  unsigned char *buffer_p,
                                  size_t buffer_size,
                                  size_t *size_off,
                                  unsigned char *bit_off);

    char SerializeUInt(UInt64 uval,
                       unsigned char *buffer_p,
                       size_t buffer_size,
                       size_t *size_off,
                       unsigned char *bit_off);

    unsigned char *DeserializeUInt8(unsigned char *buffer,
                                    size_t *m_bytes,
                                    size_t *bit_count,
                                    unsigned_int_size_t uint_size,
                                    unsigned char is_header,
                                    UInt8 *out);

    unsigned char *DeserializeUInt16(unsigned char *buffer,
                                     size_t *m_bytes,
                                     size_t *bit_count,
                                     unsigned_int_size_t uint_size,
                                     UInt16 *out);

    unsigned char *DeserializeUInt32(unsigned char *buffer,
                                     size_t *m_bytes,
                                     size_t *bit_count,
                                     unsigned_int_size_t uint_size,
                                     UInt32 *out);

    unsigned char *DeserializeUInt64(unsigned char *buffer,
                                     size_t *m_bytes,
                                     size_t *bit_count,
                                     unsigned_int_size_t uint_size,
                                     UInt64 *out);
#ifdef __cplusplus
}
#endif
#define __DSERIAL_H
#endif // __DSERIAL_H
