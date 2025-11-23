add_library(common STATIC src/amqp_channel.cpp)
target_include_directories(common PUBLIC include)