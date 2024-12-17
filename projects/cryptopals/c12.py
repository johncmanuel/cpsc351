import os
from c8 import AES_BLOCK_SIZE_BYTES, count_repeated_blocks
from c7 import base64_to_bytes
from c9 import pkcs7_padding
from Crypto.Cipher import AES

key = os.urandom(AES_BLOCK_SIZE_BYTES)
secret = base64_to_bytes(
    "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkg"
    "aGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBq"
    "dXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUg"
    "YnkK"
)


# modified version of oracle method from c11.py.
# ensure the input is encrypted only in ECB mode
def encryption_oracle(input: bytes) -> bytes:
    plaintext = input + secret
    padded_input = pkcs7_padding(plaintext, AES_BLOCK_SIZE_BYTES)
    cipher = AES.new(key, AES.MODE_ECB)
    return cipher.encrypt(padded_input)


# gets the block size of encryption
def detect_block_size() -> int:
    txt = b""
    init_len = len(encryption_oracle(txt))
    curr_len = init_len
    while curr_len == init_len:
        txt += b"A"
        curr_len = len(encryption_oracle(txt))
    return curr_len - init_len


# double check to see if the encryption is in ECB mode before proceeding
def detect_ecb_mode(block_size: int) -> bool:
    test_input = b"A" * (block_size * 3)
    encrypted = encryption_oracle(test_input)
    return count_repeated_blocks(encrypted) > 0


def byte_at_a_time_ecb_decrypt(block_size: int) -> bytes:
    assert detect_ecb_mode(block_size), "ECB mode not detected"

    decrypted = b""
    total_len = len(encryption_oracle(b""))
    NUM_POSSIBLE_CHARS = 256

    for _ in range(total_len):
        length = (block_size - (1 + len(decrypted))) % block_size
        prefix = b"A" * length
        new_len = length + len(decrypted)
        real = encryption_oracle(prefix)
        for j in range(NUM_POSSIBLE_CHARS):
            fake = encryption_oracle(prefix + decrypted + bytes([j]))
            if fake[:new_len] == real[:new_len]:
                decrypted += bytes([j])
                break
        decrypted += b""

    return decrypted


def c12():
    block_size = detect_block_size()
    # print("Block size", block_size)
    assert detect_ecb_mode(block_size), "ECB mode not detected"
    # print("ECB mode detected")
    decrypted = byte_at_a_time_ecb_decrypt(block_size)
    print(decrypted.decode())


if __name__ == "__main__":
    c12()
