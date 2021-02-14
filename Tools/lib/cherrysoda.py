# -*- coding: utf-8 -*-

import glob
import os
import platform
import shutil
import subprocess
import sys
import zipfile


if sys.version_info.major == 3:
    import urllib.request as urllib2
else:
    import urllib2


def is_windows_system():
    return platform.system() == 'Windows'


def join_path(a, b):
    return os.path.join(a, b)


def abspath(a):
    return os.path.abspath(a)


def get_file_path(f):
    return os.path.dirname(f)


def get_file_name(f):
    return os.path.basename(f)


executable_suffix = ('', '.exe')[is_windows_system()]

project_path  = abspath(join_path(get_file_path(__file__), '../..'))
engine_path   = join_path(project_path, 'Engine')
tool_path     = join_path(project_path, 'Tools')
external_path = join_path(project_path, 'External')
tmp_path      = join_path(project_path, 'Tmp')
bgfx_src_path = join_path(external_path, 'bgfx/bgfx/src')

shaderc = join_path(tool_path, 'bin/shaderc' + executable_suffix)
shaderc_local = join_path(tool_path, 'bin/shaderc.local' + executable_suffix)

sdl2_version = '2.0.12'
sdl2_path = join_path(external_path, 'SDL2-' + sdl2_version)


def exists(path):
    return os.path.exists(path)


def make_sure_folder_exist(f):
    p = os.path.dirname(f)
    if not exists(p):
        os.makedirs(p)


def execute_command(command):
    # print('$ ' + ' '.join(command))
    subprocess.call(command)


def compile_shader(shader_source, output, platform, shader_type, include_path=None, profile=None, opt_level=None, bin2c_array=None):
    shader_compiler = (shaderc, shaderc_local)[exists(shaderc_local)]
    command = [shader_compiler, '-f', shader_source, '-o', output, '--platform', platform, '--type', shader_type]
    if include_path:
        command += ['-i', include_path]
    if profile:
        command += ['--profile', profile]
    if opt_level:
        command += ['-O', str(opt_level)]
    if bin2c_array:
        command += ['--bin2c', bin2c_array]
    make_sure_folder_exist(output)
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
