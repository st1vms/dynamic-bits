#ifndef __DBITS_H

#include "dtypes.h"
#include "dpacket.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Serialize `packet` into a byte buffer,
     * storing into `out_size` the total size of the buffer
     * @param packet Packet pointer allocated with ``NewPacket(id)`,
     * also having one or more serializables assigned,
     * and also registered using `RegisterPacket(id, format)`.
     * @param out_size Output reference to store the buffer size
     * @return unsigned char * e.g. Serialized byte buffer,
     * call free() on this reference after using it.
     */
    extern char SerializePacket(unsigned char *buffer, size_t buffer_size, dpacket_t packet, size_t *out_size);

    /**
     * @brief Deserialize buffer,
     * storing the packet structure into an output reference,
     * reading no more than `buffer_size` bytes,
     * up to `n_bytes`.
     *
     * @param buffer A constant unsigned char pointer, e.g the byte buffer
     * @param buffer_size Constant size of the buffer
     * @param n_bytes Number of bytes to read
     * @param packet_out dpacket_t destination pointer reference, NULL initialized
     * @return 1 on success, 0 in case of errors.
     */
    extern char DeserializeBuffer(unsigned char *buffer, const size_t buffer_size, dpacket_t packet_out);

#ifdef __cplusplus
}
#endif
#define __DBITS_H
#endif // __DBITS_H
