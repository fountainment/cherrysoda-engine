shaderc -f %1/fs_%2.sc -o %1/fs_%2.bin --platform windows --type fragment -i ../External/bgfx/bgfx/src --profile ps_5_0 -O 3
shaderc -f %1/vs_%2.sc -o %1/vs_%2.bin --platform windows --type vetex -i ../External/bgfx/bgfx/src --profile vs_5_0 -O 3
