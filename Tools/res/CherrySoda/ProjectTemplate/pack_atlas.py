#!/usr/bin/env python
# -*- coding: utf-8 -*-


import os
import sys

tool_path = None

if os.path.exists('../cherrysoda-engine/Tools/'):
	tool_path = '../cherrysoda-engine/Tools/'
elif os.path.exists('../../cherrysoda-engine/Tools/'):
	tool_path = '../../cherrysoda-engine/Tools/'
elif os.path.exists('../Tools/'):
	tool_path = '../Tools/'
elif os.path.exists('../../Tools/'):
	tool_path = '../../Tools/'

if tool_path:
	sys.path.append(os.path.abspath(tool_path))	

import lib.cherrysoda as cherry

cherry.pack_atlas()
