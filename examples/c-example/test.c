#include <stdio.h>
#include <string.h>
#include <dbits.h>

#define PACKET_ONE_ID 0   // Packet one numerical ID
#define PACKET_ONE_SIZE 1 // Packet one field count

// Packet one format array
static int packet_one_format[PACKET_ONE_SIZE] = {
    BOOLEAN_STYPE};

#define PACKET_TWO_ID 1   // Packet two numerical ID
#define PACKET_TWO_SIZE 3 // Packet two field count

// Packet two format array
static int packet_two_format[PACKET_TWO_SIZE] = {
    DOUBLE_STYPE,
    UTF8_STRING_STYPE,
    INT64_STYPE};

// Size of the data buffer
#define BUFFER_SIZE 1024

int main(void)
{

    /****************************/
    /* SERIALIZATION TEST START */
    /****************************/

    // Structure for holding packets.
    dpacket_struct_t packet_one;

    // Packets must be registered using RegisterPacket,
    // before entering any (de)serialization activity.

    // For each packet, pass the packet id, the packet format array, and the format size
    if (!RegisterPacket(PACKET_ONE_ID, packet_one_format, PACKET_ONE_SIZE) ||
        !RegisterPacket(PACKET_TWO_ID, packet_two_format, PACKET_TWO_SIZE))
    {
        // Packet registration error
        // Either you registered an invalid packet format,
        // or you tried to register more packets than allowed by PACKET_TABLE_SIZE,
        // or you tried to register a packet with more fields than MAX_PACKET_FIELDS
        return -1;
    }

    // SERIALIZATION EXAMPLE

    // First thing first, a packet must be constructed
    // NewPacket will allocate a structure for holding the packet
    if (!NewPacket(&packet_one, PACKET_ONE_ID))
    {
        // Error creating packet, nothing to do...
        return -1;
    }

    // Once the packet as been created,
    // it must be freed using FreePacket(dpacket_struct_t *)
    // when it is no longer used!

    // After creating a packet, it should be filled with the serializabled values.
    // This will serialize a boolean value of true.
    if (!AddSerializable(&packet_one, BOOLEAN_STYPE, (data_union_t){.boolean_v = 1}))
    {
        // Error adding serializable
        FreePacket(&packet_one); // Free packet_one reference
        return -1;
    }

    // Construct a zero initialized byte buffer
    unsigned char buffer[BUFFER_SIZE] = {0};
    // Rule of thumb would be to create a buffer large enough to hold all of the maximum values at once.
    // If the buffer isn't large enough to hold all of the serialized data,
    // SerializePacket will return an error value of 0

    size_t buffer_size = 0; // Variable to hold final buffer size

    // Serialize packet into bytebuffer, writing no more than BUFFER_SIZE-1 bytes.
    // The result buffer will be already null terminated,
    // if you created a zero initialized buffer
    if (!SerializePacket(buffer, BUFFER_SIZE - 1, &packet_one, &buffer_size))
    {
        // Serialization error
        FreePacket(&packet_one); // Free packet_one reference
        return -1;
    }
    FreePacket(&packet_one); // Free packet_one reference as it is no longer used.

    // Ensure buffer is null terminated
    buffer[buffer_size] = 0;

    printf("\nPacket One buffer: \nsize: %ld\nbuffer: ", buffer_size);
    for (size_t i = 0; i < buffer_size; i++)
    {
        printf("0x%.02x ", buffer[i]);
    }
    printf("\n");
    /**************************/
    /* SERIALIZATION TEST END */
    /**************************/

    /******************************/
    /* DESERIALIZATION TEST START */
    /******************************/

    // Now let's serialize packet two and deserialize it along with packet one

    dpacket_struct_t packet_two;
    if (!NewPacket(&packet_two, PACKET_TWO_ID))
    {
        // Error creating packet, nothing to do...
        return -1;
    }

    // Fill packet fields

    if (!AddSerializable(&packet_two, DOUBLE_STYPE, (data_union_t){.double_v = 3.1415926}))
    {
        FreePacket(&packet_two);
        return -1;
    }

    // Serializing UTF8 String
    const char *test_string = "I do desire we may be better strangers";
    if (!AddUTF8StringSerializable(&packet_two, test_string, strlen(test_string)))
    {
        FreePacket(&packet_two);
        return -1;
    }

    if (!AddSerializable(&packet_two, INT64_STYPE, (data_union_t){.decimal_v.i64_v = INT64_MIN}))
    {
        FreePacket(&packet_two);
        return -1;
    }

    unsigned char buffer_two[BUFFER_SIZE] = {0};
    size_t buffer_size_two = 0;

    if (!SerializePacket(buffer_two, BUFFER_SIZE - 1, &packet_two, &buffer_size_two))
    {
        // Serialization error
        FreePacket(&packet_two);
        return -1;
    }
    FreePacket(&packet_two); // Free packet reference as it is no longer used.

    // Ensure buffer is null terminated
    buffer_two[buffer_size_two] = 0;

    printf("\nPacket Two buffer: \nsize: %ld\nbuffer: ", buffer_size_two);
    for (size_t i = 0; i < buffer_size_two; i++)
    {
        printf("0x%.02x ", buffer_two[i]);
    }
    printf("\n");

    // Create packet structures for deserialization
    dpacket_struct_t des_packet_one, des_packet_two;

    // Deserialize packet one buffer
    if (!DeserializeBuffer(buffer, buffer_size, &des_packet_one))
    {
        // Deserialization error, do nothing and return.
        return -1;
    }

    // After deserialization, the packet structures must be freed with FreePacket,
    // when no longer used.

    // Deserialize packet two buffer
    if (!DeserializeBuffer(buffer_two, buffer_size_two, &des_packet_two))
    {
        // Deserialization error, free packet one structure
        FreePacket(&des_packet_one);
        return -1;
    }

    // Create a serializable list node pointer, to access packet fields.
    serializable_list_node_t *tmp;

    // Parse packet one fields
    // Check packet id and packet field count
    if (des_packet_one.packet_id == PACKET_ONE_ID &&
        des_packet_one.data_list.size == PACKET_ONE_SIZE)
    {
        // Assign first field
        tmp = des_packet_one.data_list.first_node;
        if (tmp != NULL)
        {
            // Check field type
            if (tmp->stype == BOOLEAN_STYPE)
            {
                printf("\nPACKET_ONE DATA -> %d\n", tmp->data.boolean_v);
            }
        }
    }
    // Free packet reference as it is not longer been used.
    FreePacket(&des_packet_one);

    double d = 0.0;
    char out_str[1024] = {0};
    signed long long int i = 0;

    // Parse packet two fields
    if (des_packet_two.packet_id == PACKET_TWO_ID &&
        des_packet_two.data_list.size == PACKET_TWO_SIZE)
    {
        // Assign first field
        tmp = des_packet_two.data_list.first_node;
        if (tmp == NULL || tmp->stype != DOUBLE_STYPE)
        {
            FreePacket(&des_packet_two);
            return -1;
        }
        // Parse first field
        d = tmp->data.double_v;

        // Parse second field
        tmp = tmp->next_node;
        if (tmp == NULL || tmp->stype != UTF8_STRING_STYPE ||
            tmp->data.utf8_str_v.length > 1024)
        {
            FreePacket(&des_packet_two);
            return -1;
        }
        // Copy UTF8 String
        strncpy(out_str, tmp->data.utf8_str_v.utf8_string, tmp->data.utf8_str_v.length);
        out_str[tmp->data.utf8_str_v.length] = 0;

        // Parse third field
        tmp = tmp->next_node;
        if (tmp == NULL || tmp->stype != INT64_STYPE)
        {
            FreePacket(&des_packet_two);
            return -1;
        }

        i = tmp->data.decimal_v.i64_v;
    }
    FreePacket(&des_packet_two);

    printf("\nPACKET TWO DATA:\ndouble: %lf\nutf8-string: %s\nint64: %d\n", d, out_str, i);

    /****************************/
    /* DESERIALIZATION TEST END */
    /****************************/

    return 0;
}
