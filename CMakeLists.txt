cmake_minimum_required (VERSION 3.5)
project (Smart-Car)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
    include
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    thirdparty/Simple-OpenGL-Image-Library/src
    thirdparty/fann/src/include
)

link_directories(
    thirdparty/Simple-OpenGL-Image-Library/build
    thirdparty/fann/src
)

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    ${GLUT_LIBRARY}
    ${GLEW_LIBRARY}
    SOIL
    fann floatfann
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
)

set(src_files
    src/main.cpp
    src/CollisionDetection/Collision_Detection.cpp
    src/Core/EventHandler.cpp src/Core/Init/Init_GLEW.cpp src/Core/Init/Init_GLUT.cpp
    src/GameLogic/Game_Logic.cpp
    src/Managers/Models_Manager.cpp src/Managers/Scene_Manager.cpp src/Managers/Shader_Manager.cpp
    src/Rendering/Camera.cpp src/Rendering/Models/Car.cpp src/Rendering/Models/Model.cpp src/Rendering/Models/Obstacle.cpp
)

set(hdr_files
    include/CollisionDetection/Collision_Detection.h
    include/Core/EventHandler.h include/Core/Init/Init_GLEW.h include/Core/Init/Init_GLUT.h include/Core/Init/ContextInfo.h include/Core/Init/FrameBufferInfo.h include/Core/Init/IListener.h include/Core/Init/WindowInfo.h
    include/GameLogic/Game_Logic.h include/GameLogic/Agent.h include/GameLogic/Sensor.h
    include/Managers/Models_Manager.h include/Managers/Scene_Manager.h include/Managers/Shader_Manager.h
    include/Rendering/Camera.h include/Rendering/IGameObject.h include/Rendering/Rect.h include/Rendering/VertexFormat.h include/Rendering/Models/Car.h include/Rendering/Models/Model.h include/Rendering/Models/Obstacle.h
)

file(GLOB shdr_files src/Shaders/*.glsl)

# build application
add_executable(smart-car ${src_files} ${hdr_files})
target_link_libraries(smart-car ${ALL_LIBS})
