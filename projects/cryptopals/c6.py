from c3 import xor_cipher
import string


def break_repeating_key_xor(data: bytes):
    normalized_dsts = {}
    start_ks, end_ks = 2, 40

    for ks in range(start_ks, end_ks + 1):
        chunk_bytes = [data[i : i + ks] for i in range(0, len(data), ks)]
        dst = []
        for i in range(len(chunk_bytes) - 1):
            for j in range(i + 1, len(chunk_bytes)):
                dst.append(hemming_distance(chunk_bytes[i], chunk_bytes[j]) / ks)
        normalized_dsts[ks] = sum(dst) / len(dst) if dst else 0

    possible_key_sizes = sorted(normalized_dsts, key=normalized_dsts.get)
    possible_plaintexts = []
    binary_data = data

    for key_size in possible_key_sizes:
        key = b""
        # Break the ciphertext into blocks of key_size length
        for i in range(key_size):
            block = bytes(binary_data[j] for j in range(i, len(binary_data), key_size))
            # Use xor_cipher to find the key for this block
            key_byte = xor_cipher(block)
            if key_byte:
                key += bytes([ord(key_byte[0])])

        # If we couldn't find a complete key, skip this key size
        if len(key) != key_size:
            continue

        # Decrypt the entire message with this key
        decrypted = bytes(
            binary_data[i] ^ key[i % len(key)] for i in range(len(binary_data))
        )

        # Score and store the result
        possible_plaintexts.append((decrypted, key))

    # Return the best decryption based on readability
    if possible_plaintexts:
        return max(
            possible_plaintexts,
            key=lambda x: len([c for c in x[0] if chr(c) in string.printable]),
        )

    return "", b""

    # return ""


def hemming_distance(s1: bytes, s2: bytes) -> int:
    assert len(s1) == len(s2), "Strings must be of equal length"
    dist = 0
    for ch1, ch2 in zip(s1, s2):
        dist += sum(1 for i in format(ch1 ^ ch2, "08b") if i == "1")
    return dist


def c6():
    with open("c6.txt") as f:
        data = f.read().splitlines()
        print(data)
    x = b"this is a test"
    y = b"wokka wokka!!!"
    res = hemming_distance(x, y)
    assert res == 37, f"Unexpected result, output: {res}"


c6()
