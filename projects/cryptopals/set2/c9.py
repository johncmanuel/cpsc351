def pkcs7_padding(plain_text: bytes, block_size: int) -> bytes:
    padding = block_size - len(plain_text) % block_size
    return plain_text + bytes([padding] * padding)


def c9():
    msg = b"YELLOW SUBMARINE"
    res = pkcs7_padding(msg, 20)
    expected = "YELLOW SUBMARINE\x04\x04\x04\x04"
    print(res, expected)


if __name__ == "__main__":
    c9()
