from c3 import xor_cipher
from c5 import repeating_key_xor, bytes_to_hex
import string
from c7 import base64_to_bytes


def break_repeating_key_xor(data: bytes) -> tuple[bytes, bytes]:
    normalized_dsts = {}
    start_ks, end_ks = 2, 40
    printable_chars = set(string.printable)

    for ks in range(start_ks, end_ks + 1):
        chunk_bytes = [data[i : i + ks] for i in range(0, len(data), ks)][:4]
        dst = []
        for i in range(len(chunk_bytes) - 1):
            for j in range(i + 1, len(chunk_bytes)):
                h = hemming_distance(chunk_bytes[i], chunk_bytes[j])
                dst.append(h)
        avg_dst = sum(dst) / len(dst)
        normalized_dsts[ks] = avg_dst / ks

    possible_key_sizes = sorted(normalized_dsts, key=normalized_dsts.get)[:3]
    possible_plaintexts = []
    final_res = []
    print(f"Possible key sizes: {possible_key_sizes}")

    for s in possible_key_sizes:
        key = b""
        for i in range(s):
            # transpose the blocks
            block = bytes([data[j] for j in range(i, len(data), s)])
            key += bytes([xor_cipher(block)[1]])
        if not key:
            continue
        print(f"Key: {key}")
        r = repeating_key_xor(data, key)
        possible_plaintexts.append((key, r))
    # print(possible_plaintexts)
    # print(len(possible_plaintexts))
    for key, plaintext in possible_plaintexts:
        if all(char in printable_chars for char in plaintext):
            score = sum(1 for char in plaintext if char.isalpha() or char.isspace())
            final_res.append((score, key, plaintext))
    # print(final_res)
    if final_res:
        return max(final_res, key=lambda x: x[0])
    return b"", b""


def hemming_distance(s1: bytes, s2: bytes) -> int:
    assert len(s1) == len(s2), "Strings must be of equal length"
    dist = 0
    for ch1, ch2 in zip(s1, s2):
        dist += sum(1 for i in format(ch1 ^ ch2, "08b") if i == "1")
    return dist


def c6():
    x, y = b"this is a test", b"wokka wokka!!!"
    res = hemming_distance(x, y)
    assert res == 37, f"Unexpected result, output: {res}"
    with open("c6.txt") as f:
        data = f.read().replace("\n", "")
        data = base64_to_bytes(data)
        res = break_repeating_key_xor(data)
        print(res)


if __name__ == "__main__":
    c6()
