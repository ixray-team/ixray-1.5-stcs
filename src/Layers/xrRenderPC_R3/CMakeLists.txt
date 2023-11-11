project(xrRender_R3)
set(XRR3_SRC_FILES)

# Files import
file(GLOB_RECURSE XRR3_SOURCE_ALL_FILES
    "*.cpp"
    "*.h"
)
file(GLOB_RECURSE XRR3_SOURCE_COMP_FILES
    "../xrRender/*.cpp"
    "../xrRender/*.h"
)
file(GLOB_RECURSE XRR3_SOURCE_COMP_BLEND_FILES
    "../xrRender/Blender_*.cpp"
    "../xrRender/Blender_*.h"
)
file(GLOB_RECURSE XRR3_SOURCE_BLEND_FILES
    "Blender_*.cpp"
    "Blender_*.h"
)
file(GLOB_RECURSE XRR3_SOURCE_DX9_FILES
    "../xrRenderDX10/*.cpp"
    "../xrRenderDX10/*.h"
)
file(GLOB_RECURSE XRR3_SOURCE_LUA_FILES
    "../../xrEngine/ai_script_lua*.cpp"
    "../../xrEngine/ai_script_lua*.h"
)

# Source groups
source_group("render" FILES ${XRR3_SOURCE_ALL_FILES})
source_group("layers" FILES ${XRR3_SOURCE_COMP_FILES})
source_group("general_dx10" FILES ${XRR3_SOURCE_DX9_FILES})
source_group("lua_ext" FILES ${XRR3_SOURCE_LUA_FILES})
source_group("blend" FILES ${XRR3_SOURCE_COMP_BLEND_FILES})
source_group("blend" FILES ${XRR3_SOURCE_BLEND_FILES})

# Apply list
list(APPEND XRR3_SRC_FILES ${XRR3_SOURCE_ALL_FILES})
list(APPEND XRR3_SRC_FILES ${XRR3_SOURCE_COMP_FILES})
list(APPEND XRR3_SRC_FILES ${XRR3_SOURCE_DX9_FILES})
list(APPEND XRR3_SRC_FILES ${XRR3_SOURCE_LUA_FILES})

# Remove unused files
#list(FILTER XRR3_SRC_FILES EXCLUDE REGEX "../xrRender/HW.cpp")

# xrRender_R3 project
add_library(xrRender_R3 SHARED ${XRR3_SRC_FILES})
target_precompile_headers(xrRender_R3 PRIVATE "stdafx.h")

# Include directories
target_include_directories(xrRender_R3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(xrRender_R3 PUBLIC "${IXRAY_SDK_INC}")

# Project defines
target_compile_definitions(xrRender_R3 PRIVATE _WINDOWS)
target_compile_definitions(xrRender_R3 PRIVATE XRRENDER_R3_EXPORTS)
target_compile_definitions(xrRender_R3 PRIVATE USE_DX10)
target_compile_definitions(xrRender_R3 PRIVATE _USRDLL)

add_compile_options(/fp:fast)
target_compile_definitions(xrRender_R3 PRIVATE "$<$<CONFIG:Debug>:DEBUG>")

# Linker list
target_link_libraries(xrRender_R3 PUBLIC xrEngine)
target_link_libraries(xrRender_R3 PUBLIC xrParticles)
target_link_libraries(xrRender_R3 PUBLIC "d3dcompiler.lib")
target_link_libraries(xrRender_R3 PUBLIC "dxgi.lib")
target_link_libraries(xrRender_R3 PUBLIC "d3d10.lib")

# Nuget
add_custom_command(TARGET ${PROJECT_NAME}
    PRE_BUILD
    COMMAND ${NUGET_COMMAND} restore ${CMAKE_CURRENT_SOURCE_DIR}/Packages.config -SolutionDirectory ${CMAKE_BINARY_DIR}
)

## DirectX SDK
target_include_directories(xrRender_R3 PUBLIC "${CMAKE_BINARY_DIR}/packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/include/")
target_include_directories(xrRender_R3 PUBLIC "${CMAKE_BINARY_DIR}/packages/directxmesh_desktop_win10.2023.4.28.1/include/")

if(NOT "${CMAKE_VS_PLATFORM_NAME}" MATCHES "(x64)")
    target_link_libraries(xrRender_R3 PUBLIC ${CMAKE_BINARY_DIR}/packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/release/lib/x86/d3dx10.lib)
else()
    target_link_libraries(xrRender_R3 PUBLIC ${CMAKE_BINARY_DIR}/packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/release/lib/x64/d3dx10.lib)
endif()