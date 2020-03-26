import glob
import os
import pathlib
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


executable_suffix = ('', '.exe')[is_windows_system()]

engine_path   = abspath(join_path(get_file_path(__file__), '../..'))
tool_path     = join_path(engine_path, 'Tools')
external_path = join_path(engine_path, 'External')
bgfx_src_path = join_path(external_path, 'bgfx/bgfx/src')

shaderc = join_path(tool_path, 'bin/shaderc' + executable_suffix)


def make_sure_folder_exist(f):
    p = os.path.dirname(f)
    pathlib.Path(p).mkdir(parents=True, exist_ok=True)


def execute_command(command):
    # print('$ ' + ' '.join(command))
    subprocess.run(command)


def compile_shader(shader_source, output, platform, shader_type, include_path=None, profile=None, opt_level=None):
    command = [shaderc, '-f', shader_source, '-o', output, '--platform', platform, '--type', shader_type]
    if include_path:
        command += ['-i', include_path]
    if profile:
        command += ['--profile', profile]
    if opt_level:
        command += ['-O', str(opt_level)]
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


def copy(src, dest):
    src_list = get_file_list_from_wildcard(src)
    for source in src_list:
        shutil.copy(source, dest)


def move(src, dest):
    src_list = get_file_list_from_wildcard(src)
    for source in src_list:
        shutil.move(source, dest)


def set_environment_variable(env_var, value):
    command = ['setx', env_var, value]
    execute_command(command)

def write_str_file(str, dest):
    f = open(dest, 'w')
    f.write(str)
    f.close()
