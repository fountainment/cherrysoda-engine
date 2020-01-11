import os
import pathlib
import subprocess
import sys

def is_windows_system():
    return True

def join_path(a, b):
    return os.path.join(a, b)

def get_file_path(f):
    return os.path.dirname(f)

executable_suffix = ('', '.exe')[is_windows_system()]

engine_path   = join_path(get_file_path(__file__), '../..')
tools_path    = join_path(engine_path, 'tools')
bgfx_src_path = join_path(engine_path, 'External/bgfx/bgfx/src')

shaderc = join_path(tools_path, 'bin/shaderc' + executable_suffix)

def make_sure_folder_exist(f):
    p = os.path.dirname(f)
    pathlib.Path(p).mkdir(parents=True, exist_ok=True)

def execute_command(command):
    subprocess.run(command)

def compile_shader(shader_source, output, platform, shader_type, include_path=None, profile=None, opt_level=None):
    command = ' '.join([shaderc, '-f', shader_source, '-o', output, '--platform', platform, '--type', shader_type])
    if include_path:
        command += ' -i ' + include_path
    if profile:
        command += ' --profile ' + profile
    if opt_level:
        command += ' -O ' + str(opt_level)
    make_sure_folder_exist(output)
    execute_command(command)