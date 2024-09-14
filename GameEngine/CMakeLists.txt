# CMakeList.txt : projet CMake pour GameEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
file(GLOB_RECURSE LIBS "lib/*.lib")

add_library(GameEngine ${SOURCES} ${HEADERS})

set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)

target_include_directories(GameEngine PRIVATE include)

add_compile_definitions(GLEW_STATIC)

target_link_libraries(GameEngine PRIVATE ${LIBS})

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
