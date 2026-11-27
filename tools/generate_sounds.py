#!/usr/bin/env python3
"""Generates Piung's sound effects: short square-wave tones in the style of
the original 1972 Pong. Uses only the standard library.

Run from the repository root:  python3 tools/generate_sounds.py
"""
import math
import struct
import wave
from pathlib import Path

RATE = 22050
VOLUME = 0.3
FADE = 0.004  # seconds of fade in/out, avoids clicks

SOUNDS = {
    "hit.wav": (459.0, 0.06),     # paddle hit: short, high blip
    "point.wav": (490.0, 0.25),   # point scored: longer tone
}


def square_tone(freq: float, duration: float) -> bytes:
    total = int(RATE * duration)
    fade = int(RATE * FADE)
    samples = bytearray()
    for i in range(total):
        level = VOLUME if math.sin(2 * math.pi * freq * i / RATE) >= 0 else -VOLUME
        envelope = min(1.0, i / fade, (total - i) / fade)
        samples += struct.pack("<h", int(level * envelope * 32767))
    return bytes(samples)


def main() -> None:
    out = Path(__file__).resolve().parent.parent / "Resources" / "Audio"
    out.mkdir(parents=True, exist_ok=True)
    for name, (freq, duration) in SOUNDS.items():
        with wave.open(str(out / name), "wb") as w:
            w.setnchannels(1)
            w.setsampwidth(2)
            w.setframerate(RATE)
            w.writeframes(square_tone(freq, duration))
        print(f"wrote {out / name}")


if __name__ == "__main__":
    main()
