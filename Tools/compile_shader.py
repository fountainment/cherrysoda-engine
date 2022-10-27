#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry
import re
import sys
import argparse


def compile_shader_program(shader_dir, shader_name):
    print('Compiling shader "' + shader_name + '" ...')
    vert_shader = cherry.join_path(shader_dir, 'shaders', 'vs_' + shader_name + '.sc')
    frag_shader = cherry.join_path(shader_dir, 'shaders', 'fs_' + shader_name + '.sc')
    vert_out    = cherry.join_path(shader_dir, '%s',      'vs_' + shader_name + '.bin')
    frag_out    = cherry.join_path(shader_dir, '%s',      'fs_' + shader_name + '.bin')
    include_dirs= [cherry.bgfx_src_path, cherry.shader_path]

    shader_out_dir = cherry.join_path('assets', 'shaders')

    compile_info = []
    if cherry.is_windows_system():
        compile_info += [
            ['windows', ['vs_3_0', 'ps_3_0'], 3, 'dx9'],
            ['windows', ['vs_5_0', 'ps_5_0'], 3, 'dx11']
        ]
    compile_info += [
        ['nacl',    [None,    None   ], None, 'essl'],
        ['android', [None,    None   ], None, 'essl_a'],
        ['linux',   ['120',   '120'  ], None, 'glsl'],
        ['osx',     ['metal', 'metal'], None, 'metal'],
#       ['orbis',   ['pssl',  'pssl' ], None, 'pssl'],
        ['linux',   ['spirv', 'spirv'], None, 'spirv']
    ]

    for info_arr in compile_info:
        platform, profile, opt_level, shader_id = info_arr
        folder    = cherry.join_path(shader_out_dir, shader_id)
        cherry.compile_shader(vert_shader, vert_out % folder, platform, 'vertex',   include_dirs, profile[0], opt_level)
        cherry.compile_shader(frag_shader, frag_out % folder, platform, 'fragment', include_dirs, profile[1], opt_level)


def compile_embedded_shader_program(shader_dir, shader_name):
    print('Compiling embedded shader "' + shader_name + '" ...')
    vert_shader = cherry.join_path(shader_dir, 'shaders',          'vs_' + shader_name + '.sc')
    frag_shader = cherry.join_path(shader_dir, 'shaders',          'fs_' + shader_name + '.sc')
    vert_out    = cherry.join_path(shader_dir, 'embedded_shaders', 'vs_' + shader_name + '.bin.h')
    frag_out    = cherry.join_path(shader_dir, 'embedded_shaders', 'fs_' + shader_name + '.bin.h')
    dx_vert_out = cherry.join_path(shader_dir, 'embedded_shaders', 'vs_' + shader_name + '.dx.bin.h')
    dx_frag_out = cherry.join_path(shader_dir, 'embedded_shaders', 'fs_' + shader_name + '.dx.bin.h')
    include_dirs= [cherry.bgfx_src_path, cherry.shader_path]
    shader_tmp  = cherry.join_path(cherry.tmp_path, 'shader.tmp')

    compile_info = [
        {
            'platform': 'linux',
            'profile': ['120', '120'],
            'suffix': 'glsl'
        },
        {
            'platform': 'android',
            'suffix': 'essl'
        },
        {
            'platform': 'linux',
            'profile': ['spirv', 'spirv'],
            'suffix': 'spv'
        },
        {
            'platform': 'ios',
            'profile': ['metal', 'metal'],
            'opt_level': 3,
            'suffix': 'mtl'
        }
    ]
    cherry.make_sure_folder_exist(vert_out)
    vs_file = open(vert_out, 'w')
    fs_file = open(frag_out, 'w')
    for info_dict in compile_info:
        platform  = info_dict.get('platform')
        profile   = info_dict.get('profile', [None, None])
        opt_level = info_dict.get('opt_level')
        suffix    = info_dict.get('suffix', '')
        cherry.compile_shader(vert_shader, shader_tmp, platform, 'vertex',   include_dirs, profile[0], opt_level, 'vs_' + shader_name + '_' + suffix)
        vs_file.write(cherry.read_file(shader_tmp))
        cherry.compile_shader(frag_shader, shader_tmp, platform, 'fragment', include_dirs, profile[1], opt_level, 'fs_' + shader_name + '_' + suffix)
        fs_file.write(cherry.read_file(shader_tmp))
    vs_file.write('extern const uint8_t* vs_' + shader_name + '_pssl;\n')
    vs_file.write('extern const uint32_t vs_' + shader_name + '_pssl_size;\n')
    fs_file.write('extern const uint8_t* fs_' + shader_name + '_pssl;\n')
    fs_file.write('extern const uint32_t fs_' + shader_name + '_pssl_size;\n')
    vs_file.close()
    fs_file.close()
    if cherry.is_windows_system():
        compile_info = [
            {
                'platform': 'windows',
                'profile': ['vs_3_0', 'ps_3_0'],
                'opt_level': 3,
                'suffix': 'dx9'
            },
            {
                'platform': 'windows',
                'profile': ['vs_4_0', 'ps_4_0'],
                'opt_level': 3,
                'suffix': 'dx11'
            }
        ]
        vs_file = open(dx_vert_out, 'w')
        fs_file = open(dx_frag_out, 'w')
        for info_dict in compile_info:
            platform  = info_dict.get('platform')
            profile   = info_dict.get('profile', [None, None])
            opt_level = info_dict.get('opt_level')
            suffix    = info_dict.get('suffix', '')
            cherry.compile_shader(vert_shader, shader_tmp, platform, 'vertex',   include_dirs, profile[0], opt_level, 'vs_' + shader_name + '_' + suffix)
            vs_file.write(cherry.read_file(shader_tmp))
            cherry.compile_shader(frag_shader, shader_tmp, platform, 'fragment', include_dirs, profile[1], opt_level, 'fs_' + shader_name + '_' + suffix)
            fs_file.write(cherry.read_file(shader_tmp))
        vs_file.close()
        fs_file.close()


def generate_embedded_shader_header(shader_dir):
    embedded_shader_dir = cherry.join_path(shader_dir, 'embedded_shaders')
    file_list = cherry.get_file_list_from_wildcard(cherry.join_path(embedded_shader_dir, '*.bin.h'))
    shader_src = ' '.join(list(map(cherry.get_file_name, file_list)))
    p = re.compile('fs_([A-Za-z0-9_]*)\.bin.h')
    shader_names = p.findall(shader_src)
    shader_names.sort()
    header_file = open(cherry.join_path(shader_dir, 'embedded_shaders.h'), 'w')
    header_file.write('#ifndef _CHERRYSODA_EMBEDDED_SHADERS_H_\n') 
    header_file.write('#define _CHERRYSODA_EMBEDDED_SHADERS_H_\n') 
    header_file.write('\n')
    for shader in shader_names:
        header_file.write('#include "embedded_shaders/vs_%s.bin.h"\n' % (shader,))
        header_file.write('#include "embedded_shaders/fs_%s.bin.h"\n' % (shader,))
    header_file.write('\n') 
    for shader in shader_names:
        header_file.write('#include "embedded_shaders/vs_%s.dx.bin.h"\n' % (shader,))
        header_file.write('#include "embedded_shaders/fs_%s.dx.bin.h"\n' % (shader,))
    header_file.write('\n')
    header_file.write('static const bgfx::EmbeddedShader s_embeddedShaders[] =\n{\n')
    for shader in shader_names:
        header_file.write('\tBGFX_EMBEDDED_SHADER(vs_%s),\n' % (shader,))
        header_file.write('\tBGFX_EMBEDDED_SHADER(fs_%s),\n' % (shader,))
    header_file.write('\n\tBGFX_EMBEDDED_SHADER_END()\n};\n')
    header_file.write('\n')
    header_file.write('static const cherrysoda::STL::Vector<cherrysoda::String> s_embeddedShaderNameList =\n{\n')
    for shader in shader_names[:-1]:
        header_file.write('\t"%s",\n' % (shader,))
    if shader_names:
        header_file.write('\t"%s"\n' % (shader_names[-1],))
    header_file.write('};\n')
    header_file.write('\n')
    header_file.write('#endif // _CHERRYSODA_EMBEDDED_SHADERS_H_\n') 
    header_file.close()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('shader_dir')
    parser.add_argument('--shader-name')
    parser.add_argument('--file-list')
    parser.add_argument('-E', action='store_true')
    args = parser.parse_args(sys.argv[1:])
    if args.shader_name == None and args.file_list == None:
        parser.print_usage()
        return
    compile_func = compile_shader_program
    if args.E:
        compile_func = compile_embedded_shader_program
    shader_dir = args.shader_dir
    if args.shader_name != None:
        shader_name = args.shader_name
        compile_func(shader_dir, shader_name)
    elif args.file_list != None: 
        shader_src = args.file_list
        p = re.compile('fs_([A-Za-z0-9_]*)\.sc')
        shader_names = p.findall(shader_src) 
        for shader_name in shader_names:
            compile_func(shader_dir, shader_name)
    if args.E:
        generate_embedded_shader_header(shader_dir)


if __name__ == '__main__':
    main()
