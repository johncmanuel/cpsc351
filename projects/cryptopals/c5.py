def repeating_key_xor(s_bytes: bytes, key_bytes: bytes) -> bytes:
    expanded_key = bytearray()

    for i in range(len(s_bytes)):
        expanded_key.append(key_bytes[i % len(key_bytes)])

    assert len(s_bytes) == len(expanded_key), "Lengths do not match"

    encrypted = bytes([a ^ b for a, b in zip(s_bytes, expanded_key)])
    return encrypted


def bytes_to_hex(data: bytes) -> str:
    return "".join(f"{byte:02x}" for byte in data)


def c5():
    s = b"Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"
    expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"
    key = b"ICE"
    res = bytes_to_hex(repeating_key_xor(s, key))
    assert (
        res == expected
    ), f"Unexpected result for line: {s}. Result: {res}\nExpected: {expected}"
    print("Repeating key XOR successful:", res)


if __name__ == "__main__":
    c5()
