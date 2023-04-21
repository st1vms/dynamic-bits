import pydbits
from pydbits import Packet
from dataclasses import dataclass

# Define Packet one
TEST1_PACKET_ID = 0


@dataclass
class Test1(pydbits.PacketFormat):
    a: int = pydbits.UINT8_TYPE
    b: int = pydbits.UINT16_TYPE
    c: int = pydbits.UINT32_TYPE
    d: int = pydbits.UINT64_TYPE
    e: int = pydbits.INT8_TYPE


# Define packet two
TEST2_PACKET_ID = 1


@dataclass
class Test2(pydbits.PacketFormat):
    f: int = pydbits.INT16_TYPE
    g: int = pydbits.INT32_TYPE
    h: int = pydbits.INT64_TYPE
    i: int = pydbits.DOUBLE_TYPE
    j: int = pydbits.BOOLEAN_TYPE
    k: int = pydbits.UTF8_STRING_TYPE


# Register packets

assert pydbits.registerPacket(TEST1_PACKET_ID, Test1())
assert pydbits.registerPacket(TEST2_PACKET_ID, Test2())

# Create Packet one
p1 = pydbits.Packet(
    TEST1_PACKET_ID,
    a=(1 << 8) - 1,
    b=(1 << 16) - 1,
    c=(1 << 32) - 1,
    d=(1 << 64) - 1,
    e=-(1 << 7),
)

# Create Packet two
p2 = pydbits.Packet(
    TEST2_PACKET_ID,
    f=-(1 << 15),
    g=-(1 << 31),
    h=-(1 << 63),
    i=3.14156,
    j=False,
    k="Hello World!",
)

# Serialzie Packet one
b1 = p1.serialize()
print(len(b1), b1)

# Serialize packet two
b2 = p2.serialize()
print(len(b2), b2)

# Deserialize buffer one
d1 = Packet.deserialize(b1)
if d1 and d1.packet_id == TEST1_PACKET_ID:
    for f, v in d1.fields.items():
        print(f, v)

# Deserialize buffer two
d2 = Packet.deserialize(b2)
if d2 and d2.packet_id == TEST2_PACKET_ID:
    for f, v in d2.fields.items():
        print(f, v)
