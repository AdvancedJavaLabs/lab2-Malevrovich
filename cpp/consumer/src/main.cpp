#include "consumer_app.hpp"

#include <gflags/gflags.h>
#include <gflags/gflags_declare.h>

DEFINE_string(addr, "amqp://localhost/", "AMQP Address");
DEFINE_string(queue, "request", "RPC AMQP Queue");

int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    lab2::ConsumerApplication app{AMQP::Address(FLAGS_addr), FLAGS_queue};

    app.run();

    return 0;
}