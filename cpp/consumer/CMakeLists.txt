add_executable(consumer src/main.cpp)
target_link_libraries(consumer PUBLIC common)
target_link_libraries(consumer PRIVATE amqpcpp pthread dl ev gflags)
target_include_directories(consumer PRIVATE include)
