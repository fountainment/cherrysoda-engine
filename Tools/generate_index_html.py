#!/usr/bin/env python
# -*- coding: utf-8 -*-

import lib.cherrysoda as cherry
import sys


def generate_simple_index_file(executable_name, index_file_path):
    template = '''<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <style>
            * { margin:0; padding:0; }
            body { background-color: black; }
            html, body { width: 100%; height: 100%; }
            #loading_ui { position: absolute; z-index: -1; }
            #loading_ui, #canvas { height: 100vh; width: 100vw; display: block; border: 0px; }
        </style>
        <script type='text/javascript'>
            window.onload = focuswindow;
            window.onmousedown = focuswindow;
            function focuswindow() { window.focus(); }
        </script>
    </head>
    <body>
        <div class="emscripten_border">
            <canvas id="loading_ui"></canvas>
            <canvas class="emscripten" id="canvas" oncontextmenu="event.preventDefault()" tabindex="-1"></canvas>
        </div>
        <script>
            var loading_ui_canvas = document.getElementById("loading_ui");
            var ctx = loading_ui_canvas.getContext("2d");

            ctx.fillStyle = '#000000';
            ctx.fillRect(0, 0, 300, 150);

            ctx.fillStyle = "#ffffff";
            ctx.fillRect(18, 71, 1, 8);
            ctx.fillRect(281, 71, 1, 8);
            ctx.fillRect(18, 71, 264, 1);
            ctx.fillRect(18, 78, 264, 1);

            var Module = {
                preRun: [],
                postRun: [],
                setStatus: function(text) {
                    if (!Module.setStatus.last) Module.setStatus.last = { time: Date.now(), text: '' };
                    if (text === Module.setStatus.last.text) return;
                    var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
                    var now = Date.now();
                    if (m && now - Module.setStatus.last.time < 30) return;
                    Module.setStatus.last.time = now;
                    Module.setStatus.last.text = text;
                    if (m) {
                        var progress = parseInt(m[2]) / parseInt(m[4]);
                        ctx.fillRect(20, 73, 260 * progress, 4);
                    } else {
                        loading_ui_canvas.style.display = 'none';
                    }
                }
            };
        </script>
        <script async="" type="text/javascript" src="${EXECUTABLE_NAME}.js"></script>
    </body>
</html>
'''
    template = template.replace('${EXECUTABLE_NAME}', executable_name)
    cherry.write_str_file(template, index_file_path)
    

def main():
    if len(sys.argv) < 1:
        print('Usage: python generate_simple_index_file.py <executable_name> <index_file_path>')
        return 
    executable_name = sys.argv[1]
    index_file_path = sys.argv[2]
    generate_simple_index_file(executable_name, index_file_path)


if __name__ == '__main__':
    main()
