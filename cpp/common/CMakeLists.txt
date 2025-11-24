add_library(common STATIC 
    src/amqp_channel.cpp 
    src/message.cpp 
    src/task.cpp 
    src/uuid.cpp 
    src/sentences_utils.cpp 
    src/tasks/word_cnt_task.cpp)
target_include_directories(common PUBLIC include)