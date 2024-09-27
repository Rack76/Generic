# CMakeList.txt : projet CMake pour GameEngine, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.

file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB_RECURSE SOURCES_LINUX "src/platform/Linux/*.cpp")
file(GLOB_RECURSE SOURCES_WINDOWS "src/platform/Windows/*.cpp")

file(GLOB_RECURSE HEADERS "include/*.h")

if(WIN32)
	list(REMOVE_ITEM SOURCES ${SOURCES_LINUX})
	file(GLOB_RECURSE LIBS "lib/platform/Windows/*.lib")
else()
	list(REMOVE_ITEM SOURCES ${SOURCES_WINDOWS})
	find_package(GLEW REQUIRED)
	find_package(glfw3 REQUIRED)
	find_package(OpenGL REQUIRED)
endif()

add_library(GameEngine ${SOURCES} ${HEADERS})

if(WIN32)
	target_link_libraries(GameEngine PRIVATE ${LIBS})
else()
	link_libraries(GameEngine PRIVATE ${LIBS} glfw GLEW::glew OpenGL::GL)
endif()

set_property(TARGET GameEngine PROPERTY CXX_STANDARD 17)

target_include_directories(GameEngine PRIVATE include)

add_compile_definitions(GLEW_STATIC)

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
