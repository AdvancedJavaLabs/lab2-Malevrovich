#include "producer_app.hpp"
#include "task.hpp"

#include <fstream>
#include <gflags/gflags.h>
#include <gflags/gflags_declare.h>

DEFINE_string(addr, "amqp://localhost/", "AMQP Address");
DEFINE_string(queue, "request", "RPC AMQP Queue");

DEFINE_string(input, "input.txt", "Input text filename");
DEFINE_string(output, "output.txt", "Output text filename");

DEFINE_string(task, "", "task");
DEFINE_string(task_arg, "", "task argument");

DEFINE_uint32(chunk_size, 4096, "chunk size");

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::ifstream input{FLAGS_input};
    if (!input) {
        std::cerr << "Failed to open file " << FLAGS_input;
        return 1;
    }

    std::ofstream output{FLAGS_output};
    if (!output) {
        std::cerr << "Failed to open file " << FLAGS_output;
        return 1;
    }

    auto task_type = lab2::ParseTaskType(FLAGS_task);
    if (!task_type) {
        std::cerr << "Unknown task type: " << FLAGS_task;
        return 1;
    }

    lab2::ProducerApplication app{AMQP::Address(FLAGS_addr), FLAGS_queue, std::move(input),
                                  std::move(output),         *task_type,  FLAGS_task_arg,
                                  FLAGS_chunk_size};

    app.run();

    return 0;
}