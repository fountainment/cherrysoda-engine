import lib.cherrysoda as cherry
import sys


def generate_simple_index_file(executable_name, index_file_path):
    template = '''<!DOCTYPE html>
<html>
    <head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    </head>
    <body style="margin:0;padding:0;">
        <canvas class="emscripten" id="canvas" oncontextmenu="event.preventDefault()"></canvas>
        <script src="${EXECUTABLE_NAME}.js"></script>
        <script type='text/javascript'>
            window.onload = focuswindow;
            window.onmousedown = focuswindow;
            function focuswindow() {
                window.focus();
            }
        </script>
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
