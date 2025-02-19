import hashlib
from md5 import MD5

# L'attaquant ne connait que :
    #   - H(K) sans padding
    #   - name
    #   - expiry

# 6a616371756573000000000000000000313938362d30332d30330000000000003a2e64d7a374bf564c1876dd3bd8fafe
h_k = "fcf8d0e08c5d621fc0dc920b97ad5079"
name = b"jacques"
expiry = b"2035-01-01"

def int32RevEndianness(hexa):
    tab = [int.from_bytes(bytes.fromhex(hexa[i:i+8]), byteorder="big").to_bytes(4, "little").hex() for i in range(0, len(hexa), 8)]
    return ''.join(tab)

# length of data bytes
def pad(bytes_length):
    p = b''
    # Append '1' as per MD5 padding requirements
    p += b'\x80'
    # Add '0's needed to pad the message
    while (((bytes_length + len(p)) * 8) % 512 != 448):
        p += b'\x00'
    # Message's size in bits
    size_64bits = (bytes_length*8).to_bytes(8, byteorder="little", signed=False)
    return p + size_64bits

def build_tag(name, expiry, hash):
    name = name.hex() + "00"*(16-len(name))
    expiry = expiry.hex() + "00"*(16-len(expiry))
    return name + expiry + hash

def attack():
    iv = int32RevEndianness(h_k)
    s = name + expiry
    p = pad(64 + len(s))
    print(f"pad : {p.hex()}")

    my_md5 = MD5().digest(s, iv, p)

    print(f"Crafted H(K||name||expiry) = {my_md5}")
    print(f"tag : {build_tag(name, expiry, my_md5)}")


attack()



