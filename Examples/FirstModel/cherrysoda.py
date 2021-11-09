import os
import sys

current_path = os.path.abspath(os.path.dirname(__file__))

possible_engine_locations = [
        os.path.join(current_path, '../cherrysoda-engine/'),
        os.path.join(current_path, '../../cherrysoda-engine/'),
        os.path.join(current_path, '../'),
        os.path.join(current_path, '../../')
    ]

tool_path = None

for engine_loc in possible_engine_locations:
    tmp_path = os.path.join(engine_loc, 'Tools/')
    if os.path.exists(tmp_path):
        tool_path = tmp_path
        break

if tool_path:
    sys.path.append(os.path.abspath(tool_path))

from lib.cherrysoda import *
