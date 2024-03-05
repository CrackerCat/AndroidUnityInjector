#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> console;

void init_logger() {
    if (!console) {
        console = spdlog::stdout_color_mt("console");
        spdlog::set_pattern("[%T.%e] [%^%l%$] | %v");
        console->set_level(spdlog::level::debug);
    }
}