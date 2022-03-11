#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry


def main():
    if not cherry.is_windows_system():
        print('This script is for X86/Windows.')
        print('On Linux, please use package manager to install libSDL2.')
        return
    if cherry.exists(cherry.sdl2_path):
        print('"%s" already exists!' % (cherry.sdl2_path,))
        return
    sdl2_zip_filename = cherry.join_path(cherry.tool_path, 'res\\SDL2\\SDL2-devel-' + cherry.sdl2_version + '.zip')
    cherry.extract_zip_to(sdl2_zip_filename, cherry.external_path)
    sdl2_dir = cherry.sdl2_path
    cherry.copy(cherry.join_path(cherry.tool_path, 'res\\SDL2\\sdl2-config.cmake'), sdl2_dir)


if __name__ == '__main__':
    main()
