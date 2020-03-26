import lib.cherrysoda as cherry
import re
import sys


# TODO: Support shader in folder


def compile_shader_program(shader_dir, shader_name):
    print('Compiling shader ' + shader_name + '...')
    vert_shader = cherry.join_path(shader_dir, 'shaders/vs_' + shader_name + '.sc')
    frag_shader = cherry.join_path(shader_dir, 'shaders/fs_' + shader_name + '.sc')
    vert_out    = cherry.join_path(shader_dir, '%s/vs_' + shader_name + '.bin')
    frag_out    = cherry.join_path(shader_dir, '%s/fs_' + shader_name + '.bin')
    include_dir = cherry.bgfx_src_path

    shader_out_dir = 'assets/shaders'

    compile_info = []
    if cherry.is_windows_system():
        compile_info += [
            {
                'platform': 'windows',
                'profile': ['vs_4_0', 'ps_4_0'],
                'opt_level': 3
            }
        ]
    compile_info += [
        {
            'platform': 'linux',
            'profile': ['120', '120']
        },
        {
            'platform': 'nacl' 
        },
        {
            'platform': 'android'
        }
    ]

    for info_dict in compile_info:
        platform  = info_dict.get('platform')
        profile   = info_dict.get('profile', [None, None])
        opt_level = info_dict.get('opt_level')
        folder    = cherry.join_path(shader_out_dir, platform)
        cherry.compile_shader(vert_shader, vert_out % folder, platform, 'vertex',   include_dir, profile[0], opt_level)
        cherry.compile_shader(frag_shader, frag_out % folder, platform, 'fragment', include_dir, profile[1], opt_level)


def main():
    if len(sys.argv) < 3:
        print('Usage: python compile_shader.py <shader_dir> <shader_name>')
        print('       python compile_shader.py <shader_dir> --file-list <shader_file_list>')
        return
    shader_dir = sys.argv[1]
    if len(sys.argv) == 3:
        shader_name = sys.argv[2]
        compile_shader_program(shader_dir, shader_name)
    elif len(sys.argv) == 4 and sys.argv[2] == '--file-list':
        shader_src = sys.argv[3]
        p = re.compile('vs_([A-Za-z0-9_]*)\.sc')
        shader_names = p.findall(shader_src) 
        for shader_name in shader_names:
            compile_shader_program(shader_dir, shader_name)


if __name__ == '__main__':
    main()
