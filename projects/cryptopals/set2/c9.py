def pkcs7_padding(plain_text: bytes, block_size: int) -> bytes:
    padding = block_size - len(plain_text) % block_size
    return plain_text + bytes([padding] * padding)


def verify_pkcs7_padding(padded_text, block_size):
    if len(padded_text) % block_size != 0:
        return False

    padding_length = padded_text[-1]

    if padding_length == 0 or padding_length > block_size:
        return False

    padding_bytes = padded_text[-padding_length:]
    return all(byte == padding_length for byte in padding_bytes)


def c9():
    msg = b"YELLOW SUBMARINE"
    block_size = 20
    res = pkcs7_padding(msg, block_size)
    expected = "YELLOW SUBMARINE\x04\x04\x04\x04".encode()
    assert (
        res == expected
    ), f"Expected output does not match: {res}\nExpected: {expected}"
    print("Set 2 Challenge 9 was successful.\nOutput:", res, "\nExpected:", expected)


if __name__ == "__main__":
    c9()
