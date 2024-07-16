# CMakeList.txt : projet CMake pour GameEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.

file(GLOB_RECURSE SOURCES "src/*.cpp")

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(GameEngine ${SOURCES})

set_property(TARGET GameEngine PROPERTY CXX_STANDARD 20)

target_include_directories(GameEngine PRIVATE include ${glad_INCLUDE_DIRS})

target_link_libraries(GameEngine PRIVATE glfw glad::glad)
target_link_libraries(GameEngine PUBLIC glm::glm )

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
