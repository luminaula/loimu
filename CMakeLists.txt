cmake_minimum_required(VERSION 3.20)

#setup project
set(project_name            supkt)
project(${project_name})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


set(build_executable        1)

#libraries needed
set(libraries_to_link   "")

find_package(Threads)
find_package(OpenMP)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

list(APPEND libraries_to_link Threads::Threads Qt5::Widgets Qt5::Core Qt5::Gui)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#sources
set(source_folders      "")
set(include_folders     "")
set(source_file_types   "")
set(sources             "")

list(APPEND source_folders ${CMAKE_CURRENT_SOURCE_DIR}/src)
list(APPEND include_folders ${CMAKE_CURRENT_SOURCE_DIR}/inc)

list(APPEND source_file_types
    ".c"
    ".cpp"
)

#language setup
set(project_languages
                            C
                            CXX
)
enable_language(${project_languages})
set(CMAKE_C_STANDARD        11)
set(CMAKE_CXX_STANDARD      20)

#debug build
set(CMAKE_C_FLAGS_DEBUG   "-O0 -ggdb3 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 -Wall -Wextra")

#release build
set(CMAKE_C_FLAGS_RELEASE   "-Ofast -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native")

#release debug build
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -ggdb3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -ggdb3")




foreach(folder IN LISTS source_folders)
    foreach(filetype IN LISTS source_file_types)
        set(tmp_sources "")
        file(GLOB tmp_sources ${folder}/*${filetype})
        list(APPEND sources ${tmp_sources})
    endforeach()    
endforeach()



#project is executable
if(build_executable)
    add_executable(${project_name}
        ${sources}   
    )
endif()



#headers
target_include_directories(${project_name}
    PRIVATE
    ${include_folders}
)

target_link_libraries(${project_name} ${libraries_to_link})