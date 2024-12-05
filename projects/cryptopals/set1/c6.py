def break_repeating_key_xor(data: bytes) -> str:
    normalized_dsts = {}
    start_ks, end_ks = 2, 40

    for ks in range(start_ks, end_ks + 1):
        chunk_bytes = [data[i : i + ks] for i in range(0, len(data), ks)]
        dst = 0
        for i in range(len(chunk_bytes) - 1):
            dst += hemming_distance(chunk_bytes[i], chunk_bytes[i + 1])
        dst /= 6
        normalized_dsts[ks] = dst / ks

    likely_ks = sorted(normalized_dsts, key=normalized_dsts.get)
    likely_plaintxt = []

    # get top 3
    for ks in likely_ks[:3]:
        blocks = [[] for _ in range(ks)]
        for i, byte in enumerate(data):
            blocks[i % ks].append(byte)
        key = []
        for block in blocks:
            # English letter frequency analysis
            best_char = max(
                range(256),
                key=lambda x: sum(
                    1 for b in block if chr(b ^ x).lower() in "etaoin shrdlu"
                ),
            )
            key.append(best_char)

        full_key = bytes(key * (len(data) // len(key) + 1))[: len(data)]
        plaintext = bytes(a ^ b for a, b in zip(data, full_key))

        # TODO: implement scoring using statistical analysis
        # if not likely_plaintxt or

    return ""


def hemming_distance(s1: bytes, s2: bytes) -> int:
    assert len(s1) == len(s2), "Strings must be of equal length"
    dist = 0
    for ch1, ch2 in zip(s1, s2):
        dist += sum(1 for i in format(ch1 ^ ch2, "08b") if i == "1")
    return dist


def c6():
    with open("./set1/input/c6.txt") as f:
        data = f.read().splitlines()
        print(data)
    x = b"this is a test"
    y = b"wokka wokka!!!"
    res = hemming_distance(x, y)
    assert res == 37, f"Unexpected result, output: {res}"


c6()
