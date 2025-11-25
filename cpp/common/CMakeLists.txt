add_library(common STATIC 
    src/amqp_channel.cpp 
    src/message.cpp 
    src/task.cpp 
    src/uuid.cpp 
    src/sentences_utils.cpp 
    src/serialization.cpp
    src/tasks/word_cnt_task.cpp
    src/tasks/top_n_task.cpp
    src/tasks/mood_task.cpp
    src/tasks/name_replace_task.cpp
    src/tasks/sentence_sort_task.cpp)

target_include_directories(common PUBLIC include)