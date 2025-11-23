#include "producer_app.hpp"

#include <gflags/gflags.h>
#include <gflags/gflags_declare.h>

DEFINE_string(addr, "amqp://localhost/", "AMQP Address");
DEFINE_string(queue, "hello", "AMQP Queue");

int main() {
    ProducerApplication app{AMQP::Address(FLAGS_addr), FLAGS_queue};

    app.run();

    return 0;
}