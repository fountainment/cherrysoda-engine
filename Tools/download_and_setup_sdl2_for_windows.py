import lib.cherrysoda as cherry

def main():
    # Download
    sdl2_download_link = 'https://www.libsdl.org/release/SDL2-devel-2.0.10-VC.zip'
    sdl2_zip_filename = cherry.download_url_to(sdl2_download_link, cherry.join_path(cherry.tool_path, 'tmp'))
    cherry.extract_zip_to(sdl2_zip_filename, cherry.external_path)
    # Setup
    sdl2_dir = cherry.join_path(cherry.external_path, 'SDL2-2.0.10')
    sdl2_header_dir = cherry.join_path(sdl2_dir, 'include\\SDL2\\')
    cherry.copy(cherry.join_path(cherry.tool_path, 'cfg\\sdl2-config.cmake'), sdl2_dir)
    cherry.make_sure_folder_exist(sdl2_header_dir)
    cherry.move(cherry.join_path(sdl2_dir, 'include\\*.*'), sdl2_header_dir)
    cherry.set_environment_variable('SDL2_DIR', sdl2_dir)

if __name__ == '__main__':
    main()