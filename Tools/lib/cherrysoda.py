# -*- coding: utf-8 -*-

import glob
import os
import platform
import shutil
import stat
import subprocess
import sys
import zipfile


if sys.version_info.major == 3:
    import urllib.request as urllib2
else:
    import urllib2


def is_windows_system():
    return platform.system() == 'Windows'


def is_macos_system():
    return platform.system() == 'Darwin'


def is_linux_system():
    return platform.system() == 'Linux'


def join_path(*argv):
    result = ''
    for arg in argv:
        result = os.path.join(result, arg)
    return result


def abspath(a):
    return os.path.abspath(a)


def get_file_path(f):
    return os.path.dirname(f)


def get_file_name(f):
    return os.path.basename(f)


def get_file_extention(f):
    return os.path.splitext(f)[1]


def get_file_name_without_extention(f):
    return os.path.splitext(os.path.basename(f))[0]


executable_suffix = ('', '.exe')[is_windows_system()]

project_path  = abspath(join_path(get_file_path(__file__), '..', '..'))
shader_path   = join_path(project_path, 'Engine', 'CherrySoda', 'Graphics', 'shaders')
tool_path     = join_path(project_path, 'Tools')
external_path = join_path(project_path, 'External')
tmp_path      = join_path(project_path, 'Tmp')
bgfx_src_path = join_path(external_path, 'bgfx', 'bgfx', 'src')

crunch = join_path(tool_path, 'bin', 'crunch' + executable_suffix)
shaderc = join_path(tool_path, 'bin', 'shaderc' + executable_suffix)
shaderc_local = join_path(tool_path, 'bin', 'shaderc.local' + executable_suffix)
shaderc_show_command = False

sdl2_version = '2.0.14'
sdl2_path = join_path(external_path, 'SDL2-' + sdl2_version)


def exists(path):
    return os.path.exists(path)


def make_sure_folder_exist(f):
    p = os.path.dirname(f)
    if not exists(p):
        os.makedirs(p)


def execute_command(command, working_dir=None, show_command=False):
    if show_command:
        print('$ ' + ' '.join(command))
    subprocess.call(command, cwd=working_dir)


def compile_shader(shader_source, output, platform, shader_type, include_paths=None, profile=None, opt_level=None, bin2c_array=None):
    shader_compiler = (shaderc, shaderc_local)[exists(shaderc_local)]
    command = [shader_compiler, '-f', shader_source, '-o', output, '--platform', platform, '--type', shader_type]
    if include_paths:
        for include_path in include_paths:
            command += ['-i', include_path]
    if profile:
        command += ['--profile', profile]
    if opt_level:
        command += ['-O', str(opt_level)]
    if bin2c_array:
        command += ['--bin2c', bin2c_array]
    make_sure_folder_exist(output)
    if shaderc_show_command:
        print(' '.join(command))
    execute_command(command)


def download_url_to(url, dest):
    filename = url.split('/')[-1]
    print("Downloading '%s' from '%s'..." % (filename, url))
    u = urllib2.urlopen(url)
    filedest = os.path.join(dest, filename)
    make_sure_folder_exist(filedest)
    f = open(filedest, 'wb')
    f.write(u.read())
    f.close()
    print("Finished!")
    return filedest


def extract_zip_to(filename, dest):
    zfile = zipfile.ZipFile(filename, 'r')
    for ef in zfile.namelist():
        if ef.endswith('/'):
            continue
        p = os.path.join(dest, ef)   
        make_sure_folder_exist(p)
        f = open(p, 'wb')
        f.write(zfile.read(ef))
        f.close()
    zfile.close()


def get_file_list_from_wildcard(wildcard):
    return glob.glob(wildcard)


def get_file_list_of_path(path):
    walkList = os.walk(path)
    file_list = []
    for i in walkList:
        root, dirs, files = i
        for f in files:
            file_list.append(os.path.join(root, f))
    return file_list


def copy(src, dest):
    src_list = get_file_list_from_wildcard(src)
    for source in src_list:
        shutil.copy(source, dest)


def copytree(src, dest):
    shutil.copytree(src, dest)


def readonly_handler(func, path, execinfo):
    os.chmod(path, stat.S_IWRITE)
    func(path)


def rmtree(t):
    if exists(t):
        shutil.rmtree(t, onerror=readonly_handler)


def move(src, dest):
    src_list = get_file_list_from_wildcard(src)
    for source in src_list:
        shutil.move(source, dest)


def write_str_file(s, dest):
    f = open(dest, 'w')
    f.write(s)
    f.close()


def read_file(file):
    f = open(file, 'r')
    ret = f.read()
    f.close()
    return ret


def replace_file_str(file, replace_list):
    s = read_file(file)
    for i in replace_list:
        find, rep = i
        s = s.replace(find, rep)
    write_str_file(s, file)


def replace_file_name(file, replace_list):
    s = file
    for i in replace_list:
        find, rep = i
        s = s.replace(find, rep)
    if file != s:
        if exists(s):
            print('"%s" already exists!' % (s))
            return
        move(file, s)


def get_aseprite_location():
    location = ''
    if is_windows_system():
        location = 'C:\\Program Files (x86)\\Steam\\steamapps\\common\\Aseprite\\aseprite.exe'
    elif is_linux_system():
        location = os.environ['HOME'] + '/.local/share/Steam/steamapps/common/Aseprite/aseprite'
    elif is_macos_system():
        location = os.environ['HOME'] + '/Library/Application Support/Steam/steamapps/common/Aseprite/Aseprite.app/Contents/MacOS/aseprite'

    if exists(location):
        return location
    return 'aseprite'


def pack_atlas(path=None, verbose=False):
    aseprite = get_aseprite_location()
    aseprite_folder = 'aseprites'
    if exists(aseprite_folder):
        aseprite_file_list = get_file_list_from_wildcard(join_path(aseprite_folder, '*.aseprite'))
        for aseprite_file in aseprite_file_list:
            aseprite_command = [
                    aseprite,
                    '-b', aseprite_file,
                    '--save-as', join_path('textures', get_file_name_without_extention(aseprite_file) + '_{tag}{tagframe00}.png')
                ]
            execute_command(aseprite_command , working_dir=path, show_command=True)
    if not exists(crunch):
        print('crunch binary not found, please build the project for once first')
        return
    atlas_folder = 'assets/atlases/'
    if path:
        atlas_folder = join_path(path, atlas_folder)
    make_sure_folder_exist(atlas_folder)
    crunch_command = [
            crunch,
            'assets/atlases/atlas',
            'textures/',
            '-j', '-p', '-u', '-t', '-s2048', '-p8'
        ]
    if verbose:
        crunch_command.append('-v')
    execute_command(crunch_command, working_dir=path)


def update_assets(path=join_path(project_path, 'build')):
    make_sure_folder_exist(path)
    execute_command(['cmake', project_path], working_dir=path)


def build(path=join_path(project_path, 'build'), build_type='Release'):
    make_sure_folder_exist(path)
    execute_command(['cmake', '--build', '.', '--config', build_type], working_dir=path)
