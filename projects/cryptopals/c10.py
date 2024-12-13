from c7 import base64_to_bytes
from Crypto.Cipher import AES
from c8 import AES_BLOCK_SIZE_BYTES


def encrypt_aes_ecb(plaintext, key, iv):
    pass


def decrypt_aes_ecb(ciphertext, key, iv):
    pass


def c10():
    key = b"YELLOW SUBMARINE"
    iv = b"\x00" * AES_BLOCK_SIZE_BYTES
    with open("c10.txt") as f:
        data = f.read().splitlines()
        ciphertext = base64_to_bytes("".join(data))
        if ciphertext:
            plaintext = decrypt_aes_ecb(ciphertext, key, iv)
            encrypted = encrypt_aes_ecb(plaintext, key, iv)


if __name__ == "__main__":
    c10()
