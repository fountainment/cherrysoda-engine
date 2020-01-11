import libs.cherrysoda as cherry
import sys

def main():
    shader_dir  = sys.argv[1]
    shader_name = sys.argv[2]
    vert_shader = cherry.join_path(shader_dir, 'vs_' + shader_name + '.sc')
    frag_shader = cherry.join_path(shader_dir, 'fs_' + shader_name + '.sc')
    vert_out    = cherry.join_path(shader_dir, '%s/vs_' + shader_name + '.bin')
    frag_out    = cherry.join_path(shader_dir, '%s/fs_' + shader_name + '.bin')
    include_dir = cherry.bgfx_src_path

    shader_out_dir = 'assets/shaders'

    compile_info = [
        {
            'platform': 'windows',
            'profile': ['vs_4_0', 'ps_4_0'],
            'opt_level': 3
        },
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

if __name__ == '__main__':
    main()