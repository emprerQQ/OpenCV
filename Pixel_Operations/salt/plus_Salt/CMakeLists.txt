cmake_minimum_required(VERSION 2.8)
project( plus_Salt_Image )
find_package( OpenCV REQUIRED )
add_executable( plus_Salt_Image plus_Salt_Image.cpp )
target_link_libraries( plus_Salt_Image ${OpenCV_LIBS} )
