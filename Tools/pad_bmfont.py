#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Adds transparent gaps around every glyph of bmfont-converted fonts that are
# packed into a crunch atlas, so linear texture filtering cannot bleed
# neighbouring glyphs into each other when content is scaled by a non-integer
# factor (high-DPI displays). Rewrites the atlas PNG, atlases/atlas.json and
# fonts/*.json in place. Requires Pillow.
#
# Usage: python3 pad_bmfont.py <assets_dir> [--gap 1]
#        <assets_dir> is an app assets folder containing atlases/ and fonts/.

import argparse
import json
import os
import re
import sys

from PIL import Image


def shelf_pack(chars, page_width, gap):
    """Pack (id -> (x, y, w, h)) glyph rects onto a page, gap px apart."""
    placed = {}
    x = gap
    y = gap
    shelf_height = 0
    for id, (cx, cy, cw, ch) in sorted(chars, key=lambda c: (-c[1][3], c[1][0])):
        if cw <= 0 or ch <= 0:
            placed[id] = (0, 0)
            continue
        if x + cw + gap > page_width - gap:
            x = gap
            y += shelf_height + gap
            shelf_height = 0
        placed[id] = (x, y)
        x += cw + gap
        shelf_height = max(shelf_height, ch)
    return placed, y + shelf_height + gap


def pad_font(font_path, atlas_dir, gap):
    with open(font_path, 'r') as f:
        font = json.load(f)

    atlas_json_path = os.path.join(atlas_dir, 'atlas.json')
    with open(atlas_json_path, 'r') as f:
        atlas = json.load(f)
    if len(atlas['textures']) != 1:
        print('ERROR: expected exactly one texture in %s' % (atlas_json_path,))
        return False
    tex = atlas['textures'][0]
    atlas_png_path = os.path.join(atlas_dir, tex['name'] + '.png')
    atlas_image = Image.open(atlas_png_path).convert('RGBA')

    # page file "font_0.png" -> atlas image name "font_0"
    page_images = {}
    for page in font['pages']:
        name = os.path.splitext(os.path.basename(page['file']))[0]
        entry = next((i for i in tex['images'] if i['n'] == name), None)
        if entry is None:
            print('ERROR: page %s not found in atlas' % (name,))
            return False
        page_images[page['id']] = (name, entry)

    if len(tex['images']) != len(page_images):
        print('ERROR: atlas contains images other than the font pages; '
              're-generating the atlas with crunch after padding is not supported yet')
        return False

    new_atlas_image = None
    for page_id, (name, entry) in page_images.items():
        chars = [(c['id'], (c['x'], c['y'], c['width'], c['height']))
                 for c in font['chars'] if c['page'] == page_id]
        page_width = entry['w']
        placed, new_height = shelf_pack(chars, page_width, gap)
        page_image = atlas_image.crop((entry['x'], entry['y'],
                                       entry['x'] + entry['w'], entry['y'] + entry['h']))
        padded = Image.new('RGBA', (page_width, new_height), (0, 0, 0, 0))
        for id, (cx, cy, cw, ch) in chars:
            if cw <= 0 or ch <= 0:
                continue
            px, py = placed[id]
            padded.paste(page_image.crop((cx, cy, cx + cw, cy + ch)), (px, py))
        if new_atlas_image is None:
            new_atlas_image = padded
        else:
            print('ERROR: multiple pages are not supported yet')
            return False
        entry['w'] = page_width
        entry['h'] = new_height
        entry['fw'] = page_width
        entry['fh'] = new_height
        for c in font['chars']:
            if c['page'] == page_id and c['width'] > 0 and c['height'] > 0:
                c['x'], c['y'] = placed[c['id']]
        font['common']['scaleW'] = page_width
        font['common']['scaleH'] = new_height
        print('page %s: %dx%d -> %dx%d (gap %d)' % (name, page_width, entry['h'], page_width, new_height, gap))

    new_atlas_image.save(atlas_png_path, optimize=True)
    with open(atlas_json_path, 'w') as f:
        json.dump(atlas, f, indent='\t')
        f.write('\n')

    # surgical update: keep bmfont_to_json.py's one-char-per-line style
    with open(font_path, 'r') as f:
        lines = f.readlines()
    char_coords = {c['id']: (c['x'], c['y']) for c in font['chars']}
    for i, line in enumerate(lines):
        if re.match(r'\s*"common":\{', line):
            line = re.sub(r'"scaleW":-?\d+', '"scaleW":%d' % font['common']['scaleW'], line, count=1)
            line = re.sub(r'"scaleH":-?\d+', '"scaleH":%d' % font['common']['scaleH'], line, count=1)
            lines[i] = line
            continue
        m = re.match(r'\s*\{"id":(\d+),', line)
        if not m:
            continue
        id = int(m.group(1))
        if id in char_coords:
            x, y = char_coords[id]
            line = re.sub(r'"x":-?\d+', '"x":%d' % x, line, count=1)
            line = re.sub(r'"y":-?\d+', '"y":%d' % y, line, count=1)
            lines[i] = line
    with open(font_path, 'w') as f:
        f.writelines(lines)
    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('assets_dir', help='app assets dir containing atlases/ and fonts/')
    parser.add_argument('--gap', type=int, default=1, help='transparent pixels between glyphs (default 1)')
    args = parser.parse_args()

    fonts_dir = os.path.join(args.assets_dir, 'fonts')
    atlas_dir = os.path.join(args.assets_dir, 'atlases')
    if not os.path.isdir(fonts_dir) or not os.path.isdir(atlas_dir):
        print('Usage: python3 pad_bmfont.py <assets_dir> [--gap 1]')
        sys.exit(1)

    ok = True
    for name in sorted(os.listdir(fonts_dir)):
        if name.endswith('.json'):
            print('padding %s ...' % (name,))
            ok = pad_font(os.path.join(fonts_dir, name), atlas_dir, args.gap) and ok
    sys.exit(0 if ok else 1)


if __name__ == '__main__':
    main()
