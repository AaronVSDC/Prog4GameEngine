#---------------------------------
# UNDYNE APP EXE
#---------------------------------


add_executable(UndyneApp src/UndyneApp.cpp)

target_link_libraries(UndyneApp PRIVATE UndyneEngine)
