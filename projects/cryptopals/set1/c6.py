def break_repeating_key_xor(ciphertext, keysizes) -> str:

    return ""


def hemming_distance(s1: str, s2: str) -> int:
    assert len(s1) == len(s2), "Strings must be of equal length"
    dist = 0
    for ch1, ch2 in zip(s1, s2):
        b1, b2 = f"{ord(ch1):08b}", f"{ord(ch2):08b}"
        dist += sum(b1 != b2 for b1, b2 in zip(b1, b2))
    return dist


def c6():
    x = "this is a test"
    y = "wokka wokka!!!"
    res = hemming_distance(x, y)
    assert res == 37, f"Unexpected result, output: {res}"


c6()
