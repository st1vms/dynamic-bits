#include <stdlib.h>
#include <string.h>
#include "dpacket.h"

static int _PACKET_TABLE[PACKET_TABLE_SIZE][MAX_PACKET_FIELDS];

static void TryDeallocateNodeString(serializable_list_node_t *node_p)
{
    if (node_p != NULL)
    {
        if (node_p->data.utf8_str_v.utf8_string != NULL)
        {
            free(node_p->data.utf8_str_v.utf8_string);
        }
        node_p->data.utf8_str_v.length = 0;
    }
}

static unsigned char IsValidList(serializable_list_t list_p)
{
    return ((list_p.size > 0 && NULL == list_p.first_node) ||
            (list_p.size == 0 && NULL != list_p.first_node))
               ? 0
               : 1;
}

void FreePacket(dpacket_t packet)
{
    if (packet != NULL && IsValidList(packet->data_list))
    {
        serializable_list_node_t *tmp = packet->data_list.first_node;
        while (tmp != NULL)
        {
            packet->data_list.first_node = tmp->next_node;
            if (tmp->stype == UTF8_STRING_STYPE)
            {
                TryDeallocateNodeString(tmp);
            }
            free(tmp);
            tmp = packet->data_list.first_node;
        }
        packet->data_list.first_node = NULL;
        packet->data_list.size = 0;
    }
}

char RegisterPacket(packet_id_t packet_id, int *packet_format, size_t format_size)
{

    const size_t y = PACKET_TABLE_SIZE, z = MAX_PACKET_FIELDS;
    if (packet_format == NULL || packet_id > PACKET_TABLE_SIZE || format_size > MAX_PACKET_FIELDS)
    {
        return 0;
    }

    memset(_PACKET_TABLE[packet_id], 0, MAX_PACKET_FIELDS);

    for (size_t i = 0; i < format_size; packet_format++, i++)
    {
        if (packet_format == NULL)
        {
            memset(_PACKET_TABLE[packet_id], 0, MAX_PACKET_FIELDS);
            return 0;
        }
        _PACKET_TABLE[packet_id][i] = *packet_format;
    }

    return 1;
}

int *GetPacketFormat(packet_id_t packet_id, size_t *out_size)
{
    if (packet_id > PACKET_TABLE_SIZE || out_size == NULL)
    {
        return NULL;
    }

    *out_size = 0;
    for (size_t i = 0; i < MAX_PACKET_FIELDS; i++)
    {
        if (_PACKET_TABLE[packet_id][i] == 0)
        {
            break;
        }
        *out_size += 1;
    }

    return *out_size > 0 ? _PACKET_TABLE[packet_id] : NULL;
}

char NewPacket(dpacket_t packet_p, packet_id_t packet_id)
{
    if (packet_p == NULL || packet_id > UINT8_MAX)
    {
        return 0;
    }
    packet_p->data_list = (serializable_list_t){
        .size = 0,
        .first_node = NULL};
    packet_p->packet_id = packet_id;

    return 1;
}

char AddSerializable(dpacket_t dpacket_p, serializable_type_t stype, data_union_t datav)
{
    if (dpacket_p == NULL ||
        !IsValidList(dpacket_p->data_list) ||
        stype <= NO_TYPE ||
        stype > UTF8_STRING_STYPE)
    {
        return 0;
    }

    serializable_list_node_t *node_p = NULL;
    node_p = (serializable_list_node_t *)malloc(sizeof(serializable_list_node_t));
    if (node_p == NULL)
    {
        return 0;
    }

    node_p->stype = stype;
    node_p->data = datav;
    node_p->next_node = NULL;

    if (NULL == dpacket_p->data_list.first_node && dpacket_p->data_list.size == 0)
    {
        dpacket_p->data_list.first_node = node_p;
        dpacket_p->data_list.size += 1;
        return 1;
    }
    else if (dpacket_p->data_list.size > 0 && dpacket_p->data_list.first_node != NULL)
    {
        serializable_list_node_t *tmp = dpacket_p->data_list.first_node;
        for (size_t i = 0; i < dpacket_p->data_list.size - 1; i++)
        {
            tmp = tmp->next_node;
        }

        if (tmp->next_node != NULL)
        {
            return 0;
        }

        tmp->next_node = node_p;
        dpacket_p->data_list.size += 1;
        return 1;
    }

    return 0;
}
