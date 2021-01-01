#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry
import argparse
import sys


def create_scene(path, name):
    template_path = cherry.join_path(cherry.tool_path, 'res/CherrySoda/SceneTemplate/*.*')
    project_path = path
    if cherry.exists(cherry.join_path(project_path, 'src')):
        project_path = cherry.join_path(project_path, 'src')
    if cherry.exists(cherry.join_path(project_path, 'Scenes')):
        project_path = cherry.join_path(project_path, 'Scenes')
    cherry.copy(template_path, project_path)
    file_list = [cherry.join_path(project_path, cherry.get_file_name(f)) for f in cherry.get_file_list_from_wildcard(template_path)]
    replace_list = [['CherrySodaSceneTemplate', name],
                    ['cherrysodascenetemplate', name.lower()],
                    ['CHERRYSODASCENETEMPLATE', name.upper()]]
    for f in file_list:
        cherry.replace_file_str(f, replace_list)
        cherry.replace_file_name(f, replace_list)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('scene_name')
    args = parser.parse_args(sys.argv[1:])
    create_scene('.', args.scene_name)


if __name__ == '__main__':
    main()
