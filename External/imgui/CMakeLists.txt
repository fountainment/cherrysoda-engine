add_library(imgui STATIC imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp imconfig.h imgui.h imgui_internal.h)
target_include_directories(imgui PUBLIC .)
