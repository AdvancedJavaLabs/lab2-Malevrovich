add_executable(producer src/main.cpp src/aggregator.cpp)
target_link_libraries(producer PUBLIC common)
target_link_libraries(producer PRIVATE amqpcpp pthread dl ev gflags)
target_include_directories(producer PRIVATE include)
