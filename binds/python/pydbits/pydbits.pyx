cimport pydbits
import numpy as np
from dataclasses import dataclass

__PACKET_FORMATS = [None for _ in range(PACKET_TABLE_SIZE)]

# Data Types definitions

UINT8_TYPE = pydbits.UINT8_STYPE
UINT16_TYPE = pydbits.UINT16_STYPE
UINT32_TYPE = pydbits.UINT32_STYPE
UINT64_TYPE = pydbits.UINT64_STYPE
INT8_TYPE = pydbits.INT8_STYPE
INT16_TYPE = pydbits.INT16_STYPE
INT32_TYPE = pydbits.INT32_STYPE
INT64_TYPE = pydbits.INT64_STYPE
DOUBLE_TYPE = pydbits.DOUBLE_STYPE
BOOLEAN_TYPE = pydbits.BOOLEAN_STYPE
UTF8_STRING_TYPE = pydbits.UTF8_STRING_STYPE

# Packet format dataclass definition

@dataclass
class PacketFormat:
    def __post_init__(self):
        for k,v in self.__dict__.items():
            if not isinstance(v, int) or v not in {
                pydbits.UINT8_STYPE,
                pydbits.UINT16_STYPE,
                pydbits.UINT32_STYPE,
                pydbits.UINT64_STYPE,
                pydbits.INT8_STYPE,
                pydbits.INT16_STYPE,
                pydbits.INT32_STYPE,
                pydbits.INT64_STYPE,
                pydbits.DOUBLE_STYPE,
                pydbits.BOOLEAN_STYPE,
                pydbits.UTF8_STRING_STYPE
            }:
                raise ValueError(f"PacketFormat field `{k}` is not a type-integer")

# RegisterPacket wrapper

def registerPacket(packet_id:int, packet_format:PacketFormat) -> bool:

    cdef size_t field_count = len(packet_format.__dict__)

    if packet_id >= PACKET_TABLE_SIZE or field_count == 0 or field_count > MAX_PACKET_FIELDS:
        return False

    cdef int[:] field_format = np.zeros(field_count, dtype=np.int32)

    cdef int i = 0

    for field_id in packet_format.__dict__.values():
        field_format[i] = field_id
        i += 1

    if pydbits.RegisterPacket(packet_id,<int*>&field_format[0], field_count):
        __PACKET_FORMATS[packet_id] = packet_format
        return True

    return False

# Packet definition

cdef class Packet:
    cdef pydbits.dpacket_struct_t _packet

    cdef dict fields

    def __cinit__(self, packet_id:int, **kwargs):

        cdef packet_id_t cpacket_id

        self.fields = {}

        if packet_id >= 0:
            cpacket_id = packet_id
            if packet_id > pydbits.PACKET_TABLE_SIZE:
                raise MemoryError(f"{packet_id} is not a valid packet id")

            if not pydbits.NewPacket(&self._packet, <pydbits.packet_id_t>cpacket_id):
                raise MemoryError(f"Cannot create new packet!")

            if kwargs and not self.__setFields(packet_id, **kwargs):
                raise MemoryError(f"Error setting packet fields!")

    def __dealloc__(self):
        pydbits.FreePacket(&self._packet)

    def __checkFieldValue(self, fieldType:int, obj) -> bool:

        if fieldType == UINT8_TYPE:
            if not isinstance(obj, int) or obj < 0 or obj > 0xff:
                return False
        elif fieldType == UINT16_TYPE:
            if not isinstance(obj, int) or obj < 0 or obj > 0xffff:
                return False
        elif fieldType == UINT32_TYPE:
            if not isinstance(obj, int) or obj < 0 or obj > 0xffffffff:
                return False
        elif fieldType == UINT64_TYPE:
            if not isinstance(obj, int) or obj < 0 or obj > 0xffffffffffffffff:
                return False
        elif fieldType == INT8_TYPE:
            if not isinstance(obj, int) or obj < -0x80 or obj > 0x7f:
                return False
        elif fieldType == INT16_TYPE:
            if not isinstance(obj, int) or obj < -0x8000 or obj > 0x7fff:
                return False
        elif fieldType == INT32_TYPE:
            if not isinstance(obj, int) or obj < -0x80000000 or obj > 0x7fffffff:
                return False
        elif fieldType == INT64_TYPE:
            if not isinstance(obj, int) or obj < -0x8000000000000000 or obj > 0x7fffffffffffffff:
                return False
        elif fieldType == BOOLEAN_TYPE:
            if not isinstance(obj, int) and not isinstance(obj, bool):
                return False
        elif fieldType == DOUBLE_TYPE:
            if (not isinstance(obj, float) and not isinstance(obj, int)) or obj > pydbits.DBL_MAX:
                return False

        return True

    def __add_string_serializable(self, s:str) -> bool:
        byteString = (<unicode>s).encode('UTF-8')

        cdef size_t str_len = len(byteString)

        cdef unsigned char* cstring = byteString

        return True if pydbits.AddUTF8StringSerializable(&self._packet,
            cstring, str_len) else False

    def __add_serializable(self, fieldType:int, o) -> bool:

        cdef serializable_type_t typeID = fieldType
        cdef data_union_t dataUnion

        if not isinstance(o, int) and not isinstance(o, bool) and not isinstance(o, float):
            return False

        if fieldType == UINT8_TYPE:
            dataUnion.decimal_v.u8_v = o
        elif fieldType == UINT16_TYPE:
            dataUnion.decimal_v.u16_v = o
        elif fieldType == UINT32_TYPE:
            dataUnion.decimal_v.u32_v = o
        elif fieldType == UINT64_TYPE:
            dataUnion.decimal_v.u64_v = o
        elif fieldType == INT8_TYPE:
            dataUnion.decimal_v.i8_v = o
        elif fieldType == INT16_TYPE:
            dataUnion.decimal_v.i16_v = o
        elif fieldType == INT32_TYPE:
            dataUnion.decimal_v.i32_v = o
        elif fieldType == INT64_TYPE:
            dataUnion.decimal_v.i64_v = o
        elif fieldType == BOOLEAN_TYPE:
            dataUnion.boolean_v = 0 if o else 1
        elif fieldType == DOUBLE_TYPE:
            dataUnion.double_v = o

        return True if pydbits.AddSerializable(&self._packet, typeID, dataUnion) else False

    def __getattr__(self, key):
        if key == "fields":
            return self.fields
        return self.fields[key]

    def __setattr__(self, __name: str, __value) -> None:
        if __name == "fields":
            super().__setattr__(__name, __value)
        else:
            self.fields[__name] = __value

    def __setFields(self, packet_id:pydbits.packet_id_t, **kwargs) -> bool:

        if not issubclass(type(__PACKET_FORMATS[packet_id]), PacketFormat):
            raise ValueError(f"Packet ID {packet_id} is not registered!")

        for fieldKey, fieldType in __PACKET_FORMATS[packet_id].__dict__.items():
            if fieldKey not in kwargs.keys():
                raise ValueError(f"Missing field {fieldKey} in Packet {self.packet_id} constructor")

            fieldValue = kwargs.get(fieldKey)

            if fieldKey not in __PACKET_FORMATS[self.packet_id].__dict__.keys():
                raise ValueError(f"Packet {self.packet_id} has no field named {fieldKey}")

            if not self.__checkFieldValue(__PACKET_FORMATS[self.packet_id].__dict__.get(fieldKey), fieldValue):
                raise ValueError(f"Invalid assignment for field {fieldKey} in Packet {self.packet_id}")

            self.fields[fieldKey] = fieldValue

            if fieldType == UTF8_STRING_TYPE:
                if not self.__add_string_serializable(fieldValue):
                    return False
            elif not self.__add_serializable(fieldType, fieldValue):
                return False

        return True

    @property
    def packet_id(self) -> int:
        return self._packet.packet_id

    @property
    def fields_count(self) -> int:
        return self._packet.data_list.size

    def serialize(self, buffer_size:int=1024) -> bytes | None:

        if len(self.fields.items()) != len(__PACKET_FORMATS[self.packet_id].__dict__.items()):
            return None

        cdef unsigned char [:] buffer = np.zeros(buffer_size, np.uint8)
        cdef size_t bufSize = buffer_size
        cdef size_t outSize = 0

        if not pydbits.SerializePacket(<unsigned char*>&buffer[0], bufSize-1, &self._packet, &outSize):
            return None

        return bytes(buffer[:outSize])

    @staticmethod
    def deserialize(buffer:bytes) -> Packet | None:

        cdef size_t buflen = len(buffer)

        cdef unsigned char [:] buf_p = np.zeros(buflen+1, np.uint8)
        for i, b in enumerate(buffer):
            buf_p[i] = b
        buf_p[buflen] = 0


        cdef dpacket_struct_t _packet
        if not pydbits.DeserializeBuffer(<unsigned char*>&buf_p[0], buflen, &_packet):
            return None

        packet = Packet(-1)
        packet._packet = _packet

        cdef bytes py_string
        cdef serializable_list_node_t *_pkt_node = _packet.data_list.first_node
        cdef size_t lsize = _packet.data_list.size

        form = list(__PACKET_FORMATS[packet.packet_id].__dict__.keys())
        for i in range(lsize):

            if not _pkt_node:
                return None

            if _pkt_node.stype == UINT8_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.u8_v
            elif _pkt_node.stype == UINT16_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.u16_v
            elif _pkt_node.stype == UINT32_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.u32_v
            elif _pkt_node.stype == UINT64_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.u64_v
            elif _pkt_node.stype == INT8_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.i8_v
            elif _pkt_node.stype == INT16_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.i16_v
            elif _pkt_node.stype == INT32_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.i32_v
            elif _pkt_node.stype == INT64_TYPE:
                packet.fields[form[i]] = _pkt_node.data.decimal_v.i64_v
            elif _pkt_node.stype == BOOLEAN_TYPE:
                packet.fields[form[i]] = True if _pkt_node.data.boolean_v else False
            elif _pkt_node.stype == DOUBLE_TYPE:
                packet.fields[form[i]] = _pkt_node.data.double_v
            elif _pkt_node.stype == UTF8_STRING_TYPE:
                py_string = _pkt_node.data.utf8_str_v.utf8_string
                packet.fields[form[i]] = str(py_string.decode("UTF-8"))

            _pkt_node = _pkt_node.next_node

        return packet
