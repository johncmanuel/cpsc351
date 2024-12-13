from c3 import hex_to_bytes

AES_BLOCK_SIZE_BYTES = 16


def count_repeated_blocks(ciphertext: bytes) -> int:
    blocks = [
        ciphertext[i : i + AES_BLOCK_SIZE_BYTES]
        for i in range(0, len(ciphertext), AES_BLOCK_SIZE_BYTES)
    ]
    return len(blocks) - len(set(blocks))


def detect_aes_ecb(ciphertext: list[bytes]) -> tuple[int, int]:
    highest_repeats = (-1, 0)
    for i in range(len(ciphertext)):
        repeats = count_repeated_blocks(ciphertext[i])
        highest_repeats = max(highest_repeats, (i, repeats), key=lambda x: x[1])
    return highest_repeats


def c8():
    with open("c8.txt") as f:
        data = f.read().splitlines()
        encrypted = [hex_to_bytes(line) for line in data]
        res = detect_aes_ecb(encrypted)
        expected = 132
        assert (
            res[0] == expected
        ), f"Expected output does not match: {res[0]}\nExpected: {expected}"
    print("Set 1 Challenge 8 was successful.\nOutput:", res[0], "\nExpected:", expected)


if __name__ == "__main__":
    c8()
