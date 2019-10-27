shaderc -f %1/fs_%2.sc -o %1/fs_%2.es.bin --platform nacl --type fragment -i ../External/bgfx/bgfx/src
shaderc -f %1/vs_%2.sc -o %1/vs_%2.es.bin --platform nacl --type vetex -i ../External/bgfx/bgfx/src
