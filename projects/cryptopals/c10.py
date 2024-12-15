from c7 import base64_to_bytes
from Crypto.Cipher import AES
from c8 import AES_BLOCK_SIZE_BYTES, get_blocks
from c9 import pkcs7_padding


def pkcs7_unpadding(data: bytes) -> bytes:
    len_padding = data[-1]
    return data[:-len_padding]


def encrypt_aes_cbc(plaintext, key, iv):
    cipher = AES.new(key, AES.MODE_ECB)

    plaintext = pkcs7_padding(plaintext, AES_BLOCK_SIZE_BYTES)
    blocks = get_blocks(plaintext, AES_BLOCK_SIZE_BYTES)

    ciphertext = b""
    for block in blocks:
        xor_block = bytes(a ^ b for a, b in zip(block, iv))
        encrypted_block = cipher.encrypt(xor_block)
        ciphertext += encrypted_block
        iv = encrypted_block
    return ciphertext


def decrypt_aes_cbc(ciphertext, key, iv):
    cipher = AES.new(key, AES.MODE_ECB)
    blocks = get_blocks(ciphertext, AES_BLOCK_SIZE_BYTES)

    plaintext = b""

    for block in blocks:
        decrypted_block = cipher.decrypt(block)
        plaintext += bytes(a ^ b for a, b in zip(decrypted_block, iv))
        iv = block
    return pkcs7_unpadding(plaintext)


def c10():
    key = b"YELLOW SUBMARINE"
    iv = b"\x00" * AES_BLOCK_SIZE_BYTES
    with open("c10.txt") as f:
        data = f.read().splitlines()
        ciphertext = base64_to_bytes("".join(data))
        if ciphertext:
            decrypted = decrypt_aes_cbc(ciphertext, key, iv)
            encrypted = encrypt_aes_cbc(decrypted, key, iv)
            print("Decrypted:", decrypted.decode())
            print("Does encrypted match the original input:", encrypted == ciphertext)


if __name__ == "__main__":
    c10()
