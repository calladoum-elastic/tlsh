import pathlib
import hashlib
import os

import tlsh

CURFILE = pathlib.Path(__file__)
CURDIR = CURFILE.parent
TEST_DATA_PATH = (CURDIR / "../../tests/datasets").absolute()


def test_module_version():
    assert isinstance(tlsh.version, str) and tlsh.version
    assert isinstance(tlsh.version_info, tlsh.ModuleVersionInfo)
    assert isinstance(tlsh.version_info.major, int)
    assert isinstance(tlsh.version_info.minor, int)
    assert isinstance(tlsh.version_info.patch, int)
    assert isinstance(tlsh.version_info.checksum_granularity, int)
    assert isinstance(tlsh.version_info.window_slide_size, int)

    assert tlsh.version_info.checksum_granularity in (1, 3)
    assert tlsh.version_info.window_slide_size == 5


def test_basic_test():
    buf = os.urandom(1024)

    hexval = tlsh.hexdigest(buf)
    assert isinstance(hexval, str)
    assert not hexval.startswith("T")

    val = tlsh.digest(buf)
    assert isinstance(val, bytes)
    assert not val.startswith(b"T")

    hexval2 = tlsh.hexdigest(buf, 1)
    assert isinstance(hexval2, str)
    assert hexval2.startswith("T1")

    val2 = tlsh.digest(buf, 1)
    assert isinstance(val2, bytes)
    assert val2.startswith(b"T1")

    for _ in range(2000):
        a = tlsh.Tlsh()
        a.update(buf)
        a.final()
        assert a.valid

        b = tlsh.Tlsh()
        b.update(buf)
        b.final()
        assert b.valid

        assert bool(a)
        assert isinstance(a.hexdigest(), str)
        assert isinstance(a.digest(), bytes)
        assert val == a.digest()
        assert hexval == a.hexdigest()
        assert a.diff(b) == 0


def test_validate_diff():
    a_s = "4141414141414141414141414141414141414141414141414141414141414141414141"
    a_b = b"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    b_s = "4141414141414141414141414141414141414141414141414141414141414141414142"
    b_b = b"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB"
    assert tlsh.diff(a_s, b_s) == tlsh.diff(a_b, b_b)
    assert tlsh.diff(a_s, b_s) == tlsh.diff(a_b, b_s)
    assert tlsh.diff(a_b, b_s) == tlsh.diff(a_s, b_b)


def test_load_string():
    input_a = "B0524126A7A1CF3EDD3893B804A74631A2B67898A37522372755B7342F933544AA34C9"
    input_b = "T1B0524126A7A1CF3EDD3893B804A74631A2B67898A37522372755B7342F933544AA34C9"
    a = tlsh.Tlsh()
    a.load(input_a)
    assert a.valid
    assert a.hexdigest() == input_a
    b = tlsh.Tlsh()
    b.load(input_b)
    assert b.valid
    assert b.hexdigest(ver=1) == input_b


def test_batch_test_base():
    expected_values = [
        x.strip().split(maxsplit=1)
        for x in (TEST_DATA_PATH / "base.txt").open().readlines()
    ]

    # full read
    for fname, expected_value in expected_values:
        buf = (TEST_DATA_PATH / fname).open("rb").read()
        tlsh_hex = tlsh.hexdigest(buf).upper()
        tlsh_raw = tlsh.digest(buf)
        assert (
            tlsh_hex == expected_value.upper()
        ), f"Mismatch on {fname}: expected={expected_value.upper()}  got={tlsh_hex}"
        expected_value_raw = bytes.fromhex(expected_value)
        assert (
            tlsh_raw == expected_value_raw
        ), f"Mismatch on {fname}: got={tlsh_raw} expected={expected_value_raw}"

    # block read
    for fname, expected_value in expected_values:
        bksz = 512
        t = tlsh.Tlsh()
        assert not t.valid
        with (TEST_DATA_PATH / fname).open("rb") as fd:
            while True:
                buf = fd.read(bksz)
                if not buf:
                    break
                t.update(buf)
            t.final()
            assert t.valid
        tlsh_hex = t.hexdigest(0).upper()
        tlsh_raw = t.digest(0)
        assert (
            tlsh_hex == expected_value.upper()
        ), f"Mismatch on {fname}: expected={expected_value.upper()}  got={tlsh_hex}"
        expected_value_raw = bytes.fromhex(expected_value)
        assert (
            tlsh_raw == expected_value_raw
        ), f"Mismatch on {fname}: got={tlsh_raw} expected={expected_value_raw}"


def test_batch_test_extended_file_level():
    test_dir = TEST_DATA_PATH / "extended/samples"
    expected_value_matrix = [
        (
            test_dir / "c/bin/code_share.x64",
            "f606b57f08198a84aaf6df04757aaee1552d969be66e0be1a61f2c2a6e4c090b",
            "B0524126A7A1CF3EDD3893B804A74631A2B67898A37522372755B7342F933544AA34C9",
        ),
        (
            test_dir / "c/bin/code_share2.x64",
            "11d5959efb6829761d561e194a3139137a9a9bebe5e265b616d8aa24808ec5c1",
            "04525016A7A1CE7EDD38A3B804A74230A2B67898A3762337265577342F533548EA35C9",
        ),
        (
            test_dir / "c/bin/main_arm",
            "7a34f72c372a07a9bdc97954f130606a40b56762c7a8912eb1d642a6c97b506a",
            "4E63EAA5FB85DD6BE462973488D783B0B372E954A76343533A18A3786E037D40E6218A",
        ),
        (
            test_dir / "c/bin/main_x86",
            "f675790b051af32e8929c138286c7a35fbdd764154c03b327a8ca9b8ab0a8ab0",
            "06525203B3AA8A3BD9941B7804934330F1FBE45093B3872F1B25B3741E413A45D3BA6A",
        ),
        (
            test_dir / "c/bin/main_x86-32",
            "d77b9c297a7e5a2c2d04338717e757a79c4e9464dcc4d6b441f8384f042b019e",
            "07627207B36AC937D894177C059B8724B1B7E43097B3472B2729B3A41D423A86D3FB5A",
        ),
        (
            test_dir / "c/bin/main_x86_64",
            "dbff8b77aa93e92cdf7b97fddb0f518d8507ccfffeac018b45d66836c6969875",
            "8652344BBB60CF7ACC68137844A74730B2BAF85462A153232B55B3340E937949F575EA",
        ),
        (
            test_dir / "c/bin/version1.arm",
            "e95c3b940be8bc45a58068e86581b9108bb18a23de66357c3f5ab9157b2d2484",
            "2863C78AF981EE9BE4C1B334889753E0737AD81943226303750DE3796F93BC55EAB185",
        ),
        (
            test_dir / "c/bin/version1.x64",
            "3307eaab7c5f6e831ba233936f2220ba31ba82280c1106c3dd36cde8a2fbf64f",
            "F852D71AFBA1C9BEDCBCD3F484A70230B2B67D625376613B265076391F533805A138E8",
        ),
        (
            test_dir / "c/bin/version2.arm",
            "7b0bc23eb57fe7fd0e09b8115c3a5fa8768df41b9e2a671e36851bda1c71e83c",
            "3563D78AF981DE8BE4C1E330889753E0737BE859432263037609D3796F93BC55EAA184",
        ),
        (
            test_dir / "c/bin/version2.x64",
            "d2955fd2a886f4ab7e9249b9b933c9830cc8779c880c022c371420077005aa10",
            "5452D81AFBA1C9BEDCFCD3F484A70230A2B67962137A713B265466351F533805B538E5",
        ),
        (
            test_dir / "c/bin/version3.arm",
            "58e660b3f850f43b94c9d37577f5725f36f98ab4aca8d605872472f0e214f290",
            "CD63F78BF981DD9BE481E33488D753F07376E819472223037649E3796F93BC59EAA084",
        ),
        (
            test_dir / "c/bin/version3.x64",
            "30c5d196188ff76784e7272bad8ef6f249a61d63318e10d274f06dcbca1e48fe",
            "5F52FA2AFBA1C9BDDCBCE3F484570170B2B6786153B6623B269467341F933445A538E8",
        ),
        (
            test_dir / "c/bin/version4.x64",
            "92f8041d84316754342983b9685a862ffa97612c9cdc0e6ee03b77b256112e3e",
            "DA52C81AFBA2C9BEDDBCD3F484A74230A2B63D6213766137269476381F533405B538E5",
        ),
    ]

    for fpath, sha2val, tlshval in expected_value_matrix:
        assert fpath.exists()
        buf = fpath.open("rb").read()
        assert hashlib.sha256(buf).digest() == bytes.fromhex(sha2val)
        assert tlsh.hexdigest(buf) == tlshval


def test_perf_tlsh_load(benchmark):
    result: int = benchmark(
        tlsh.diff,
        "DA52C81AFBA2C9BEDDBCD3F484A74230A2B63D6213766137269476381F533405B538E5",
        "5F52FA2AFBA1C9BDDCBCE3F484570170B2B6786153B6623B269467341F933445A538E8",
    )
    assert isinstance(result, int)


DIFF_TESTCASES = [
    # (hash1, hash2, expected_score)
    [
        "T17E16D300ACF0A8D1D44DB37A95DC19249EE71EC38D30695EBBDCEC9D1F216884DE269B",
        "T17E16D300ACF0A8D1D44DB37A95DC19249EE71EC38D30695EBBDCEC9D1F216884DE26AA",
        2,
    ]
]


def test_diff():
    for tc in DIFF_TESTCASES:
        t1 = tlsh.Tlsh()
        assert t1.load(tc[0])
        assert t1.valid
        t2 = tlsh.Tlsh()
        assert t2.load(tc[1])
        assert t2.valid
        assert t1.diff(t2, False) == tc[2]
