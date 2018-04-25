cmake_minimum_required(VERSION 3.9)
project(labc_proj C)

set(CMAKE_C_STANDARD 99)

add_executable(labc_proj Client/User-main.c Client/User.c Client/User.h Common.h Client/Post.c Client/Post.h)