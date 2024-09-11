# This sets up a library for Unity, they don't provide an importable cmake config.
add_library(unity lib/unity/src/unity.c)
target_include_directories(unity PUBLIC lib/unity/src)
