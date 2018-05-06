cmake_minimum_required( VERSION 2.8.9 )
project(cstrike LANGUAGES CXX )

# Compiler stuff

set( CMAKE_C_COMPILER /usr/bin/gcc )
set( CMAKE_CXX_COMPILER /usr/bin/g++ )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3 -pthread" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -mavx2 -mfma -pthread" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")

# Adding include directory for audio

include_directories( "ext/fmod/4.3" )

# Linking libraries

link_directories( "ext/jpeg/" "ext/fmod/4.3/" "obj/")
link_libraries( "GL" "GLU" "glut" "jpeg" "m" "libfmodex64.so" )

# Adding sources

# Pre-compiled objects
set( OBJS "obj/OpenGL64.o" )
set_source_files_properties( ${OBJS} PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

# Building object files
add_library( Labyrinthe OBJECT "src/Labyrinthe.cpp" )
add_library( Chasseur   OBJECT "src/Chasseur.cpp" )
add_library( Gardien	OBJECT "src/Gardien.cpp")
add_library( Dijkstra	OBJECT "src/Dijkstra.cpp")

# Final executable
add_executable(cstrike
  ${OBJS}
  $<TARGET_OBJECTS:Labyrinthe>
  $<TARGET_OBJECTS:Chasseur>
  $<TARGET_OBJECTS:Gardien>
  $<TARGET_OBJECTS:Dijkstra>
)
