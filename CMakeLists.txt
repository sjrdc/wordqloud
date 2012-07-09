PROJECT(wordcloud)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

# find required libraries
find_package(Boost COMPONENTS program_options REQUIRED)
set(QT_USE_QTOPENGL true)
set(QT_USE_QTSVG true)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
find_package(OpenCV REQUIRED)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# add subdirs
ADD_SUBDIRECTORY(src)
