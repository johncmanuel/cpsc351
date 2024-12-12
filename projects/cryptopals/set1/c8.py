from c3 import hex_to_bytes

aes_block_size = 16


def count_repeated_blocks(ciphertext: bytes) -> int:
    blocks = [
        ciphertext[i : i + aes_block_size]
        for i in range(0, len(ciphertext), aes_block_size)
    ]
    return len(blocks) - len(set(blocks))


def detect_aes_ecb(ciphertext: list[bytes]):
    highest_repeats = 0
    for i in range(len(ciphertext)):
        repeats = count_repeated_blocks(ciphertext[i])
        highest_repeats = max(highest_repeats, repeats)
    return highest_repeats


def c8():
    with open("./set1/input/c8.txt") as f:
        data = f.read().splitlines()
        encrypted = [hex_to_bytes(line) for line in data]
        res = detect_aes_ecb(encrypted)
        print(res)


if __name__ == "__main__":
    c8()
