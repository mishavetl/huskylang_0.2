file(GLOB srcfiles *.c)
file(GLOB main_c main.c)

list(REMOVE_ITEM srcfiles ${main_c})

add_library(src ${srcfiles})
