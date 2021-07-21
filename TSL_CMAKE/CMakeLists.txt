# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(TSL_CMAKE)

set(LINKLIBS)
set(COMPILE_OPTION)
set(INCL)
set(SRC)
set(OpenCV_DIR "C:/opencv/build")

if(WIN32)
  find_package( OpenCV REQUIRED )
  list(APPEND LINKLIBS
    ${OpenCV_LIBS}
    )
endif()

list(APPEND SRC
  src/main.cpp
  src/camera_usage.cpp
  src/image_processing.cpp
)

list(APPEND LINKLIBS)
list(APPEND COMPILE_OPTION)

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${INCL}
  )

target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 14 )
