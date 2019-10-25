shaderc -f fs_simple.sc -o fs_simple.bin --platform windows --type fragment -i ../../External/bgfx/bgfx/src --profile ps_5_0 -O 3
shaderc -f vs_simple.sc -o vs_simple.bin --platform windows --type vetex -i ../../External/bgfx/bgfx/src --profile vs_5_0 -O 3
