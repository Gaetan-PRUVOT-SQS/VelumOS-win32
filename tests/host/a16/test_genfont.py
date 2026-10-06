import sys
import unittest
from pathlib import Path

RACINE = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(RACINE / "tools"))

import genfont_emit as emit  # noqa: E402
import genfont_raster as raster  # noqa: E402
from genfont import SPECS  # noqa: E402

FONTS = RACINE / "third_party" / "fonts"


def spec_of(ident):
    return next(spec for spec in SPECS if spec.ident == ident)


class CoverageTests(unittest.TestCase):
    def test_codepoints_present_in_open_sans(self):
        data = (FONTS / "OpenSans-Regular.ttf").read_bytes()
        for cp in (0x20, 0x41, 0xE9, 0x152, 0x17F, 0x2026, 0x20AC):
            self.assertNotEqual(raster.cmap_glyph_id(data, cp), 0, hex(cp))

    def test_codepoints_absent(self):
        data = (FONTS / "OpenSans-Regular.ttf").read_bytes()
        for cp in (0x4E2D, 0x1F600, 0x0378):
            self.assertEqual(raster.cmap_glyph_id(data, cp), 0, hex(cp))

    def test_missing_codepoint_stops_generation(self):
        with self.assertRaises(raster.FontError):
            raster.check_coverage(FONTS / "OpenSans-Regular.ttf", [0x41, 0x4E2D])

    def test_charset_is_sorted_and_complete(self):
        cps = raster.charset()
        self.assertEqual(cps, sorted(set(cps)))
        self.assertEqual(len(cps), 329)
        self.assertNotIn(raster.REPLACEMENT, cps)


class RasterTests(unittest.TestCase):
    def test_fixed_advance_glyph_is_kept_in_cell(self):
        spec = spec_of("mono")
        left = raster.Glyph(0xC5, 7, 13, -1, 0, 8, b"")
        right = raster.Glyph(0x10F, 8, 10, 1, 3, 8, b"")
        wide = raster.Glyph(0x57, 9, 9, 0, 4, 8, b"")
        self.assertEqual(raster.keep_in_cell(spec, left).xoff, 0)
        self.assertEqual(raster.keep_in_cell(spec, right).xoff, 0)
        with self.assertRaises(raster.FontError):
            raster.keep_in_cell(spec, wide)

    def test_proportional_font_is_left_alone(self):
        glyph = raster.Glyph(0x66, 5, 9, -1, 3, 4, b"")
        self.assertIs(raster.keep_in_cell(spec_of("ui"), glyph), glyph)

    def test_soft_hyphen_is_the_hyphen(self):
        glyphs = {glyph.cp: glyph for glyph in raster.rasterize(spec_of("ui"), FONTS)}
        hyphen, soft = glyphs[0x2D], glyphs[0xAD]
        shape = ("bits", "width", "height", "xoff", "yoff", "advance")
        for field in shape:
            self.assertEqual(getattr(soft, field), getattr(hyphen, field), field)

    def test_every_font_fits_its_cell(self):
        for spec in SPECS:
            glyphs = raster.rasterize(spec, FONTS)
            self.assertEqual(len(glyphs), 330, spec.ident)
            self.assertEqual(glyphs[-1].cp, raster.REPLACEMENT)

    def test_mono_ink_stays_inside_its_8_pixel_cell(self):
        for glyph in raster.rasterize(spec_of("mono"), FONTS):
            self.assertGreaterEqual(glyph.xoff, 0, hex(glyph.cp))
            self.assertLessEqual(glyph.xoff + glyph.width, 8, hex(glyph.cp))


class EmitTests(unittest.TestCase):
    def test_identical_bitmaps_share_storage(self):
        glyphs = [
            raster.Glyph(0x41, 1, 1, 0, 0, 2, b"\x80"),
            raster.Glyph(0x42, 1, 1, 0, 0, 2, b"\x80"),
            raster.Glyph(0x43, 1, 1, 0, 0, 2, b"\x40"),
        ]
        blob, offsets = emit.pack_blob(glyphs)
        self.assertEqual(blob, b"\x80\x40")
        self.assertEqual(offsets, {0x41: 0, 0x42: 0, 0x43: 1})

    def test_output_is_sorted_by_codepoint(self):
        spec = spec_of("ui")
        glyphs = list(reversed(raster.rasterize(spec, FONTS)))
        text, _ = emit.emit_c(spec, glyphs)
        cps = [
            int(line.split(",")[0].strip("\t{"))
            for line in text.splitlines()
            if line.startswith("\t{")
        ]
        self.assertEqual(cps, sorted(cps))
        self.assertEqual(cps[-1], raster.REPLACEMENT)

    def test_output_has_offsets_and_no_pointer_per_glyph(self):
        spec = spec_of("ui")
        text, blob_size = emit.emit_c(spec, raster.rasterize(spec, FONTS))
        rows = [line for line in text.splitlines() if line.startswith("\t{")]
        self.assertNotIn("&", text)
        self.assertNotIn("NULL", text)
        for row in rows:
            fields = [int(field) for field in row.strip("\t{},").split(",")]
            self.assertEqual(len(fields), 7)
            stride = (fields[1] + 7) // 8
            self.assertLessEqual(fields[6] + fields[2] * stride, blob_size)
        self.assertIn(f"g_ui_glyphs, g_ui_bits, {blob_size}}};", text)

    def test_blob_larger_than_16_bit_offsets_is_refused(self):
        spec = spec_of("ui")
        glyphs = [
            raster.Glyph(0x100 + k, 8, 255, 0, 0, 8, bytes([k % 256, k // 256]) * 127 + b"\x01")
            for k in range(258)
        ]
        with self.assertRaises(raster.FontError):
            emit.emit_c(spec, glyphs)


if __name__ == "__main__":
    unittest.main()
