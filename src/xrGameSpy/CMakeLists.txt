project(xrGameSpy)
set(XGS_SRC_FILES)

# Files import
file(GLOB XGS_SOURCE_ALL_FILES
    "DistributionFunc.cpp"
    "GameSpy/CDKey/gcdkeyc.c"
    "GameSpy/CDKey/gcdkeys.c"
    "GameSpy/common/gsAssert.c"
    "GameSpy/common/gsAvailable.c"
    "GameSpy/common/gsCrypt.c"
    "GameSpy/common/gsDebug.c"
    "GameSpy/common/gsLargeInt.c"
    "GameSpy/common/gsMemory.c"
    "GameSpy/common/gsPlatform.c"
    "GameSpy/common/gsPlatformSocket.c"
    "GameSpy/common/gsPlatformThread.c"
    "GameSpy/common/gsPlatformUtil.c"
    "GameSpy/common/gsRC4.c"
    "GameSpy/common/gsSHA1.c"
    "GameSpy/common/gsSSL.c"
    "GameSpy/common/gsStringUtil.c"
    "GameSpy/common/gsXML.c"
    "GameSpy/common/win32/gsSocketWin32.c"
    "GameSpy/darray.c"
    "GameSpy/ghttp/ghttpBuffer.c"
    "GameSpy/ghttp/ghttpCallbacks.c"
    "GameSpy/ghttp/ghttpCommon.c"
    "GameSpy/ghttp/ghttpConnection.c"
    "GameSpy/ghttp/ghttpEncryption.c"
    "GameSpy/ghttp/ghttpMain.c"
    "GameSpy/ghttp/ghttpPost.c"
    "GameSpy/ghttp/ghttpProcess.c"
    "GameSpy/hashtable.c"
    "GameSpy/md5c.c"
    "GameSpy/Patching/ptMain.c"
    "GameSpy/Qr2/qr2.c"
    "GameSpy/Qr2/qr2regkeys.c"
    "GameSpy/ServerBrowsing/sb_crypt.c"
    "GameSpy/ServerBrowsing/sb_queryengine.c"
    "GameSpy/ServerBrowsing/sb_server.c"
    "GameSpy/ServerBrowsing/sb_serverbrowsing.c"
    "GameSpy/ServerBrowsing/sb_serverlist.c"
    "xrGameSpy.cpp"
    "xrGameSpy_Available.cpp"
    "xrGameSpy_CDKey.cpp"
    "xrGameSpy_Core.cpp"
    "xrGameSpy_GP.cpp"
    "xrGameSpy_HTTP.cpp"
    "xrGameSpy_Patching.cpp"
    "xrGameSpy_QR2.cpp"
    "xrGameSpy_SAKE.cpp"
    "xrGameSpy_ServerBrowser.cpp"
    "GameSpy/CDKey/gcdkeyc.h" 
    "GameSpy/CDKey/gcdkeys.h" 
    "GameSpy/common/gsAssert.h" 
    "GameSpy/common/gsAvailable.h" 
    "GameSpy/common/gsCommon.h" 
    "GameSpy/common/gsCrypt.h" 
    "GameSpy/common/gsDebug.h" 
    "GameSpy/common/gsLargeInt.h" 
    "GameSpy/common/gsMemory.h" 
    "GameSpy/common/gsPlatform.h" 
    "GameSpy/common/gsPlatformSocket.h" 
    "GameSpy/common/gsPlatformThread.h" 
    "GameSpy/common/gsPlatformUtil.h" 
    "GameSpy/common/gsRC4.h" 
    "GameSpy/common/gsSHA1.h" 
    "GameSpy/common/gsSSL.h" 
    "GameSpy/common/gsStringUtil.h" 
    "GameSpy/common/gsXML.h" 
    "GameSpy/darray.h"
    "GameSpy/ghttp/ghttp.h" 
    "GameSpy/ghttp/ghttpASCII.h" 
    "GameSpy/ghttp/ghttpBuffer.h" 
    "GameSpy/ghttp/ghttpCallbacks.h" 
    "GameSpy/ghttp/ghttpCommon.h" 
    "GameSpy/ghttp/ghttpConnection.h" 
    "GameSpy/ghttp/ghttpEncryption.h" 
    "GameSpy/ghttp/ghttpMain.h" 
    "GameSpy/ghttp/ghttpPost.h" 
    "GameSpy/ghttp/ghttpProcess.h" 
    "GameSpy/hashtable.h"
    "GameSpy/md5.h"
    "GameSpy/nonport.h"
    "GameSpy/Patching/pt.h" 
    "GameSpy/Qr2/qr2.h" 
    "GameSpy/Qr2/qr2regkeys.h" 
    "GameSpy/ServerBrowsing/sb_ascii.h" 
    "GameSpy/ServerBrowsing/sb_crypt.h" 
    "GameSpy/ServerBrowsing/sb_internal.h" 
    "GameSpy/ServerBrowsing/sb_serverbrowsing.h"
)

# Source groups
source_group("kernel" FILES ${XGS_SOURCE_ALL_FILES})

# Apply list
list(APPEND XGS_SRC_FILES ${XGS_SOURCE_ALL_FILES})

# Remove unused files
#list(FILTER XGS_SRC_FILES EXCLUDE REGEX "GameSpy_Patching.cpp$")

# xrGameSpy project
add_library(xrGameSpy SHARED ${XGS_SRC_FILES})

# Include directories
target_include_directories(xrGameSpy PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(xrGameSpy PUBLIC "${IXRAY_SDK_INC}")

# Project defines
target_compile_definitions(xrGameSpy PRIVATE _WINDOWS)
target_compile_definitions(xrGameSpy PRIVATE XRGAMESPY_EXPORTS)
target_compile_definitions(xrGameSpy PRIVATE _USRDLL)

add_compile_options(/fp:fast)
target_compile_definitions(xrGameSpy PRIVATE "$<$<CONFIG:Debug>:DEBUG>")

# Linker list
target_link_libraries(xrGameSpy PUBLIC xrCore)

# Move precompiled header
set_target_properties(xrGameSpy PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
