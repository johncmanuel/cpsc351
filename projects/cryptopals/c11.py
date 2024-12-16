from c10 import encrypt_aes_cbc, decrypt_aes_cbc
import os
import random
from c9 import pkcs7_padding
from Crypto.Cipher import AES
from c8 import AES_BLOCK_SIZE_BYTES, get_blocks


def encryption_oracle(input: str) -> tuple[bytes, str]:
    key = os.urandom(AES_BLOCK_SIZE_BYTES)
    prefix, suffix = os.urandom(random.randint(5, 10)), os.urandom(
        random.randint(5, 10)
    )
    padded_input = pkcs7_padding(prefix + input.encode() + suffix, AES_BLOCK_SIZE_BYTES)
    # ECB Mode
    if random.randint(0, 1) == 0:
        cipher = AES.new(key, AES.MODE_ECB)
        encrypted = cipher.encrypt(padded_input)
        mode = "ECB"
    # CBC Mode
    else:
        encrypted = encrypt_aes_cbc(padded_input, key, os.urandom(AES_BLOCK_SIZE_BYTES))
        mode = "CBC"
    return encrypted, mode


def detect_ecb_or_cbc(encrypted: bytes) -> str:
    blocks = get_blocks(encrypted, AES_BLOCK_SIZE_BYTES)
    if len(blocks) > len(set(blocks)):
        return "ECB"
    return "CBC"


def c11():
    repeated_input = "A" * 48
    for _ in range(10):
        encrypted, mode = encryption_oracle(repeated_input)
        detected_mode = detect_ecb_or_cbc(encrypted)
        print("Detected:", detected_mode, "Actual:", mode)
        assert detected_mode == mode, f"Detection failed for {mode}"
    print("All tests passed")


if __name__ == "__main__":
    c11()
