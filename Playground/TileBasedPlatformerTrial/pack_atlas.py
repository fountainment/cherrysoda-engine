#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cherrysoda
import os


current_path = os.path.abspath(os.path.dirname(__file__))

cherrysoda.pack_atlas(current_path, verbose=True, args='-j -p -u -s2048')
cherrysoda.update_assets()
