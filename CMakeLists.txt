cmake_minimum_required(VERSION 3.21)
project(interlock C)

set(CMAKE_C_STANDARD 99)

add_executable(
        interlock main.c
        include/cube.h
        include/custom_types.h
        include/custom_types.c
        include/part.h
        include/block.h
        include/block.c
        include/cube.c
        include/obj_write.h
        include/obj_write.c
        include/3d.h
        include/world.h
        include/world.c
        include/pos.h
        include/pos.c include/perms.c include/perms.h include/debug.c include/debug.h)
target_link_libraries(interlock m)