#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry


def main():
	local_projects_path = cherry.abspath(cherry.join_path(cherry.project_path, '..'))
	local_bgfx_path = cherry.join_path(local_projects_path, 'bgfx')
	local_bimg_path = cherry.join_path(local_projects_path, 'bimg')
	local_bx_path   = cherry.join_path(local_projects_path, 'bx')

	engine_bgfx_path = cherry.join_path(cherry.external_path, 'bgfx/bgfx')
	engine_bimg_path = cherry.join_path(cherry.external_path, 'bgfx/bimg')
	engine_bx_path   = cherry.join_path(cherry.external_path, 'bgfx/bx')

	cherry.rmtree(engine_bgfx_path)
	cherry.rmtree(engine_bimg_path)
	cherry.rmtree(engine_bx_path)

	cherry.copytree(local_bgfx_path, engine_bgfx_path)
	cherry.copytree(local_bimg_path, engine_bimg_path)
	cherry.copytree(local_bx_path,   engine_bx_path)

	cherry.rmtree(cherry.join_path(engine_bgfx_path, '.github'))
	cherry.rmtree(cherry.join_path(engine_bgfx_path, 'bindings'))
	cherry.rmtree(cherry.join_path(engine_bgfx_path, 'examples'))
	cherry.rmtree(cherry.join_path(engine_bimg_path, 'tools'))
	cherry.rmtree(cherry.join_path(engine_bx_path,   'tests'))


if __name__ == '__main__':
	main()
