import hashlib
from md5 import MD5

def int32RevEndianness(hexa):
    tab = [int.from_bytes(bytes.fromhex(hexa[i:i+8]), byteorder="big").to_bytes(4, "little").hex() for i in range(0, len(hexa), 8)]
    return ''.join(tab)

def pad(message):
    p = b''
    message_bytes = bytes.fromhex(message)
    # Append '1' as per MD5 padding requirements
    message_bytes += b"\x80"
    p += b'\x80'
    # Add '0's needed to pad the message
    while (len(message_bytes) * 8) % 512 != 448:
        # print("pad !")  
        message_bytes += b"\x00"
        p += b'\x00'
    padded_message = message_bytes
    # Message's size in bits
    original_size = len(message * 4)
    # Convert the size to a 64-bit representation in little-endian format
    size_64bits = (original_size).to_bytes(8, byteorder="little", signed=False)
    # Concatenate the 64 bits representing the size to the padded bit sequence
    preprocessed_message = padded_message + size_64bits
    return p + size_64bits

iv = "67452301EFCDAB8998BADCFE10325476"
s = "4c6543544632414d53495f6573745f567261696d656e745f54726f70386269656e5f4f6e53616d757365436f6d6d65446573466f757354726f704269656e5f5f"
h_k = MD5().digest(bytes.fromhex(s), iv, b'')
print(f"H(K) = {h_k}")

s = "4c6543544632414d53495f6573745f567261696d656e745f54726f70386269656e5f4f6e53616d757365436f6d6d65446573466f757354726f704269656e5f5f6a616371756573313938362d30332d3033"
p = pad("4c6543544632414d53495f6573745f567261696d656e745f54726f70386269656e5f4f6e53616d757365436f6d6d65446573466f757354726f704269656e5f5f6a616371756573313938362d30332d3033")
print(f"p : {p}")
my_md5 = MD5().digest(bytes.fromhex(s), iv)
print(f"H(K||name||expiry) = {my_md5}")

iv = int32RevEndianness(h_k)
print(f"iv = H(K) = {iv}")
s = "6a616371756573313938362d30332d3033"
my_md5 = MD5().digest(bytes.fromhex(s), iv, p)
print(f"H(name||expiry)^iv = {my_md5}")


def attack(h_k_np, name, expiry):
    # L'attaquant ne doit connaitre que :
    #   - H(K) sans padding
    #   - name et expiry
    
    iv = int32RevEndianness(h_k_np)
    p = pad("a"*128 + name + expiry)
    print(f"pad : {p.hex()}")
    s = name + expiry

    my_md5 = MD5().digest(bytes.fromhex(s), iv, p)
    print(f"Crafted H(K||name||expiry) = {my_md5}")

# attack(h_k, "6a616371756573", "313938362d30332d3033")
attack(h_k, "6a616371756573", "323032352d30312d3031")


# 6a616371756573000000000000000000323032352d30312d30310000000000000cdcc96fe8495d8bbe6d1b583e504753