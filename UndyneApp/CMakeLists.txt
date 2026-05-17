#---------------------------------
# UNDYNE APP EXE
#---------------------------------


add_executable(UndyneApp src/UndyneApp.cpp "src/UndyneApp.h")

target_link_libraries(UndyneApp PRIVATE UndyneEngine)
