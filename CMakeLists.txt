cmake_minimum_required(VERSION 3.25)
project(getmic)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SLikeNet)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SLikeNet/Source/include)
add_compile_definitions(_RETAIL _RAKNET_SUPPORT_FileOperations=0 _RAKNET_SUPPORT_DirectoryDeltaTransfer=0 _RAKNET_SUPPORT_ConnectionGraph2=0 _RAKNET_SUPPORT_FileListTransfer=0 _RAKNET_SUPPORT_FullyConnectedMesh2=0 _RAKNET_SUPPORT_FullyConnectedMesh=0)
set(RAKNET_ENABLE_EXTENSIONS FALSE)
set(SLIKENET_ENABLE_DLL FALSE)
execute_process (
        COMMAND bash -c "rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/SLikeNet/Source/src/crypto"
        OUTPUT_VARIABLE outVar
) #plz ignore crypto warning, it can't compile at all
add_executable(getmic main.cpp src/recorder.cpp src/include/recorder.h src/xinput.cpp src/include/xinput.h src/rakClient.cpp src/include/rakClient.h src/player.cpp src/include/player.h)
target_link_libraries(getmic PUBLIC SLikeNetLibStatic pulse-simple pulse spdlog X11 Xtst)