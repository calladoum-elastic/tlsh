"""
Wrapper module for the native `_tlsh` module
"""
from typing import Optional, Union
from ._tlsh import Tlsh as _Tlsh  # type: ignore


class Tlsh:
    """Wrapper for the native class _tlsh.Tlsh"""

    def __init__(self, buffer: Optional[bytes] = None) -> None:
        self._tlsh_obj = _Tlsh()
        if buffer:
            self.update(buffer)

    def __bool__(self) -> bool:
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

    def hexdigest(self) -> str:
        """Once finalized, output the TLSH hash as a hex-string"""
        assert bool(self)
        return str(self._tlsh_obj.getHash(0))

    def digest(self) -> bytes:
        """Once finalized, output the TLSH hash as bytes"""
        return bytes.fromhex(self.hexdigest())

    def diff(self, other: "Tlsh", different_length: bool = True) -> int:
        """Calculate the score between with another TLSH object"""
        return self._tlsh_obj.totalDiff(other._tlsh_obj, different_length)

    def reset(self) -> None:
        """Reset the internal buffer used for TLSH computation."""
        self._tlsh_obj.reset()

    def fromTlshStr(self, tlsh_str: str) -> None:
        """Load a TLSH hash from a string"""
        self._tlsh_obj.fromTlshStr(tlsh_str)


version: str = _Tlsh().version


def hexdigest(buffer: bytes) -> str:
    """Helper function to quickly calculate the TLSH hash of a block of bytes
    and output the result to an hex-string."""
    t = Tlsh(buffer)
    t.final()
    return t.hexdigest()


def digest(buffer: bytes) -> bytes:
    """Helper function to quickly calculate the TLSH hash of a block of bytes
    and output the result to an array of bytes."""
    t = Tlsh(buffer)
    t.final()
    return t.digest()


def diff(h1: str, h2: str) -> int:
    """Helper function to quickly calculate the TLSH score between two TLSH
    hashes."""
    t1 = Tlsh()
    t1.fromTlshStr(h1)
    t2 = Tlsh()
    t2.fromTlshStr(h2)
    return t1.diff(t2)
