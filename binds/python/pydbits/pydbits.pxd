cdef extern from "dtypes.h":

    ctypedef enum signed_int_size_t:
        INT_NOSIZE = 0x0,
        INT8_SIZE = 0x7,
        INT16_SIZE = 0xf,
        INT32_SIZE = 0x1f,
        INT64_SIZE = 0x3f

    ctypedef enum unsigned_int_size_t:
        UINT_NOSIZE = 0x0,
        UINT8_SIZE = 0x8,
        UINT16_SIZE = 0x10,
        UINT32_SIZE = 0x20,
        UINT64_SIZE = 0x40

    ctypedef enum data_header_size_t:
        NO_HEADER = 0x0,
        HEADER8_SIZE = 0x3,
        HEADER16_SIZE = 0x4,
        HEADER32_SIZE = 0x5,
        HEADER64_SIZE = 0x6

    ctypedef unsigned char Boolean

    ctypedef signed char Int8
    ctypedef signed short Int16
    ctypedef signed int Int32
    ctypedef signed long long Int64

    ctypedef unsigned char UInt8
    ctypedef unsigned short UInt16
    ctypedef unsigned int UInt32
    ctypedef unsigned long long UInt64

    ctypedef double Double

    ctypedef UInt8 packet_id_t

    cdef unsigned int MAX_STRING_LENGTH = 0xff

    ctypedef struct utf8_string_t:
        size_t length
        # Hardcoding string size as macro value is not allowed
        UInt8 utf8_string[0xff]

    ctypedef union decimal_union_t:
        UInt8 u8_v
        UInt16 u16_v
        UInt32 u32_v
        UInt64 u64_v
        Int8 i8_v
        Int16 i16_v
        Int32 i32_v
        Int64 i64_v

    ctypedef union data_union_t:
        Double double_v
        Boolean boolean_v
        utf8_string_t utf8_str_v
        decimal_union_t decimal_v

    # Enums holding serializable types **/
    ctypedef enum serializable_type_t:
        NO_TYPE = 0x0,
        UINT8_STYPE = 0x1,
        UINT16_STYPE = 0x2,
        UINT32_STYPE = 0x3,
        UINT64_STYPE = 0x4,
        INT8_STYPE = 0x5,
        INT16_STYPE = 0x6,
        INT32_STYPE = 0x7,
        INT64_STYPE = 0x8,
        DOUBLE_STYPE = 0x9,
        BOOLEAN_STYPE = 0xa,
        UTF8_STRING_STYPE = 0xb


cdef extern from "dpacket.h":

    cdef unsigned int PACKET_TABLE_SIZE

    cdef unsigned int MAX_PACKET_FIELDS

    ctypedef struct serializable_list_node_t:
        serializable_list_node_t *next_node
        serializable_type_t stype
        data_union_t data

    ctypedef struct serializable_list_t:
        unsigned long size
        serializable_list_node_t *first_node

    ctypedef struct dpacket_struct_t:
        packet_id_t packet_id
        serializable_list_t data_list

    ctypedef dpacket_struct_t *dpacket_t


    char NewPacket(dpacket_t packet_p, packet_id_t packet_id)

    char RegisterPacket(packet_id_t packet_id, int *packet_format, size_t format_size)

    void FreePacket(dpacket_t list_p)

    char AddSerializable(dpacket_t dpacket_p, serializable_type_t stype, data_union_t datav)

    char AddUTF8StringSerializable(dpacket_t dpacket_p, const unsigned char *string_v, size_t string_len)


cdef extern from "dbits.h":

    char SerializePacket(unsigned char *buffer, size_t buffer_size, dpacket_t packet, size_t *out_size)

    char DeserializeBuffer(unsigned char *buffer, const size_t buffer_size, dpacket_t packet_out)

cdef extern from "float.h":

    cdef double DBL_MAX
