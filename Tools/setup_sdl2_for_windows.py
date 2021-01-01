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
    sdl2_zip_filename = cherry.join_path(cherry.tool_path, 'res\\SDL2\\SDL2-devel-' + cherry.sdl2_version + '-VC.zip')
    cherry.extract_zip_to(sdl2_zip_filename, cherry.external_path)
    sdl2_dir = cherry.sdl2_path
    sdl2_header_dir = cherry.join_path(sdl2_dir, 'include\\SDL2\\')
    cherry.copy(cherry.join_path(cherry.tool_path, 'res\\SDL2\\sdl2-config.cmake'), sdl2_dir)
    cherry.make_sure_folder_exist(sdl2_header_dir)
    cherry.move(cherry.join_path(sdl2_dir, 'include\\*.*'), sdl2_header_dir)
    cherry.set_environment_variable('SDL2_DIR', sdl2_dir)


if __name__ == '__main__':
    main()
