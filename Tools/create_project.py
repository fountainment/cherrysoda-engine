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
    replace_list = [['CherrySodaTemplate', name],
                    ['cherrysodatemplate', name.lower()],
                    ['CHERRYSODATEMPLATE', name.upper()]]
    for f in file_list:
        cherry.replace_file_str(f, replace_list)
        cherry.replace_file_name(f, replace_list)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('project_name')
    args = parser.parse_args(sys.argv[1:])
    create_project('.', args.project_name)


if __name__ == '__main__':
    main()
