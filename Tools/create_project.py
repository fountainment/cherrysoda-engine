#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry
import argparse
import sys


def create_project(path, name):
    template_path = cherry.join_path(cherry.tool_path, 'res/CherrySoda/ProjectTemplate')
    project_path = cherry.join_path('.', name)
    if cherry.exists(project_path):
        print('"%s" already exists in current directory!' % (name))
        return 
    cherry.copytree(template_path, project_path)
    file_list = cherry.get_file_list_of_path(project_path)
    processed_name = name
    processed_name = processed_name.replace('-', '_')
    if processed_name[0].islower():
        processed_name = processed_name[0].upper() + processed_name[1:]
    elif processed_name[0] in '0123456789':
        processed_name = 'C_' + processed_name
    replace_list = [['CherrySodaOriginal', name],
                    ['CherrySodaTemplate', processed_name],
                    ['cherrysodatemplate', processed_name.lower()],
                    ['CHERRYSODATEMPLATE', processed_name.upper()]]
    for f in file_list:
        if cherry.get_file_extention(f) in ('.cpp', '.h', '.txt'):
            cherry.replace_file_str(f, replace_list)
        cherry.replace_file_name(f, replace_list)
    cherry.append_str_to_file('add_subdirectory(%s)\n' % (name,), 'CMakeLists.txt')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('project_name')
    args = parser.parse_args(sys.argv[1:])
    create_project('.', args.project_name)


if __name__ == '__main__':
    main()
