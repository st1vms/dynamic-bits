#ifndef __DPACKET_H

#define PACKET_TABLE_SIZE 1
#define MAX_PACKET_FIELDS 1000

#include "dtypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct serializable_list_node_t
    {
        struct serializable_list_node_t *next_node;
        serializable_type_t stype;
        data_union_t data;

    } serializable_list_node_t;

    typedef struct serializable_list_t
    {
        unsigned long int size;
        serializable_list_node_t *first_node;
    } serializable_list_t;

    typedef struct dpacket_struct_t
    {
        packet_id_t packet_id;
        serializable_list_t data_deque;
    } dpacket_struct_t;

    typedef dpacket_struct_t *dpacket_t;

    char NewPacket(dpacket_t packet_p, packet_id_t packet_id);

    char RegisterPacket(packet_id_t packet_id, int *packet_format, size_t format_size);

    int *GetPacketFormat(packet_id_t packet_id, size_t *out_size);

    void FreePacket(dpacket_t deque_p);

    char AddSerializable(dpacket_t dpacket_p, serializable_type_t stype, data_union_t datav);

#ifdef __cplusplus
}
#endif
#define __DPACKET_H
#endif // __DPACKET_H
