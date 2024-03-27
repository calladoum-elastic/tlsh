"""
Wrapper module for the native `_tlsh` module
"""

from collections import namedtuple
from typing import Optional, Union
from ._tlsh import Tlsh as _Tlsh  # pylint: disable=import-error


class Tlsh:
    """Wrapper for the native class _tlsh.Tlsh"""

    def __init__(self, buffer: Optional[bytes] = None) -> None:
        self._tlsh_obj = _Tlsh()
        if buffer:
            self.update(buffer)

    def __bool__(self) -> bool:
        return self.valid

    @property
    def valid(self) -> bool:
        return self._tlsh_obj.isValid()

    def update(self, buffer: Union[bytes, bytearray]) -> None:
        """Update the buffer to calculate the TLSH on"""
        assert isinstance(buffer, bytes) or isinstance(buffer, bytearray)
        _buffer = buffer if isinstance(buffer, bytearray) else bytearray(buffer)
        self._tlsh_obj.update(_buffer)

    def final(self, buffer: Union[bytes, bytearray] = b"") -> None:
        """Finalize the TLSH computation"""
        assert isinstance(buffer, bytes) or isinstance(buffer, bytearray)
        _buffer = buffer if isinstance(buffer, bytearray) else bytearray(buffer)
        self._tlsh_obj.final(_buffer, 0)

    def hexdigest(self, ver: int = 0) -> str:
        """Once finalized, output the TLSH hash as a hex-string"""
        assert bool(self), "Invalid state"
        assert 0 <= ver < 10, "Invalid version"
        return str(self._tlsh_obj.getHash(ver))

    def digest(self, ver: int = 0) -> bytes:
        """Once finalized, output the TLSH hash as bytes"""
        res = self.hexdigest()
        if ver:
            return b"T%d" % ver + bytes.fromhex(res[2:])
        return bytes.fromhex(res)

    def diff(self, other: "Tlsh", different_length: bool = True) -> int:
        """Calculate the score between with another TLSH object"""
        return self._tlsh_obj.totalDiff(other._tlsh_obj, different_length)

    def reset(self) -> None:
        """Reset the internal buffer used for TLSH computation."""
        self._tlsh_obj.reset()

    def load(self, tlsh_input: Union[str, bytes]) -> bool:
        """Load a TLSH hash from a string or bytes"""
        if isinstance(tlsh_input, str):
            return self._tlsh_obj.fromTlshStr(tlsh_input) == 0
        if isinstance(tlsh_input, bytes):
            return self._tlsh_obj.fromTlshBytes(bytearray(tlsh_input)) == 0
        raise TypeError


version: str = _Tlsh().version
ModuleVersionInfo = namedtuple(
    "version_info",
    ["major", "minor", "patch", "checksum_granularity", "window_slide_size"],
)
version_info = ModuleVersionInfo(*_Tlsh().version_info)


def hexdigest(buffer: bytes, ver: int = 0) -> str:
    """Helper function to quickly calculate the TLSH hash of a block of bytes
    and output the result to an hex-string."""
    t = Tlsh(buffer)
    t.final()
    return t.hexdigest(ver)


def digest(buffer: bytes, ver: int = 0) -> bytes:
    """Helper function to quickly calculate the TLSH hash of a block of bytes
    and output the result to an array of bytes."""
    t = Tlsh(buffer)
    t.final()
    return t.digest(ver)


def diff(str1: Union[str, bytes], str2: Union[str, bytes]) -> int:
    """Helper function to quickly calculate the TLSH score between two TLSH
    hashes."""
    t1 = Tlsh()
    assert t1.load(str1)
    t2 = Tlsh()
    assert t2.load(str2)
    return t1.diff(t2)
