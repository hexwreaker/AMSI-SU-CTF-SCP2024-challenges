#!/bin/python3
import sys
from Cryptodome.Cipher import AES
import hashlib

def extract_blob(datas, max_bytes):
    blob = []
    offset = 0
    for i in range(max_bytes):
        # print(f"{hex(datas[offset])} at offset {hex(offset)}")
        blob.append(datas[offset])
        if datas[offset] == 0:
            # print("coucou")
            offset += 2
        else:
            offset += datas[offset]*2

    # The size of the blob must be a 16 multiple, if not we cut it.
    if len(blob)%16:
        blob = blob[:len(blob)-(len(blob)%16)]
        # print(len(blob))
    return bytes(blob)

def decryptAES(key, blob, iv):
    cipher = AES.new(key, AES.MODE_CBC, iv)
    # Decrypt the ciphertext
    return cipher.decrypt(blob)

def sha256_hash(data):
    # Create a new SHA-256 hash object
    sha256 = hashlib.sha256()
    sha256.update(data)
    # Return the hexadecimal representation of the digest
    return sha256.digest()

def calc_hashes():
    table = []
    for c in range(ord('!'), ord('}')+1):
        table.append({"char": chr(c), "hash": sha256_hash(bytes([c]))[:16].hex()})
    return table

def uncipher_blob(init_key, blob):
    iv = b'\x00'*16
    decs = []
    for i in range(int(len(blob)/16)):
        # print(f"enc : {blob[i*16:i*16+16].hex()}")
        decs.append(decryptAES(init_key, blob[i*16:i*16+16], iv))
        # print(f"key : {init_key.hex()}")
        # print(f"dec : {decs[i].hex()}")
        init_key = bytes(a ^ b for a, b in zip(init_key, blob[i*16:i*16+16]))

    return decs

def translate_decs_to_str(decs):
    hashes = calc_hashes()
    str = ""
    # print(hashes)
    for dec in decs:
        # print(dec.hex())
        r = [c_h for c_h in hashes if c_h['hash'] == dec.hex() ]
        # print(r)
        if len(r) == 1:
            str += r[0]["char"]

    return str


def main():

    if len(sys.argv) != 2:
        print("If you have a FLAC file u can convert it into PCM format using \"sox\".") 
        print("Usage : solveSpoiler.py <audio.pcm>") 
    
    filename = sys.argv[1]
    datas = b''

    with open(filename, 'rb') as fd:
        datas = fd.read()

    # Extract the blob from the audio file
    #   we suppose that we are searching for a command less than 20 char,
    #   we know that the result is 16 bytes for each char.
    print("extract blob...")
    blob = extract_blob(datas, 50*16)
    # print(blob)

    print("decrypt blocs... for rnd in 0xff :")
    for rnd in range(256):
        init_key = b'\x08\x09\x02\x02\x04\x06\x04\x08\x07\x09\x05\x01\x02\x03\x04' + rnd.to_bytes(1)
        # print(blob)
        hs = uncipher_blob(init_key, blob)
        # print(hs)
        res = translate_decs_to_str(hs)
        if (len(res) >= 1):
            print(f"for key {init_key} find ! {res}")


















if __name__=='__main__':
    main()