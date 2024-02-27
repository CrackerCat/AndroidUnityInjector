#include "main.h"
#include "test.h"
#include <string>
#include <unistd.h>

using namespace std;

static string get_self_path() {
    char buf[1024];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return string(buf);
    } else {
        return "";
    }
}

#include "Injector/KittyInjector.hpp"
#include "fmt/color.h"

static KittyInjector kitInjector;
static std::chrono::duration<double, std::milli> inj_ms{};

int main(int argc, char *argv[]) {

    // raise(SIGSTOP);

    if (argc != 2) {
        fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "\nUsage: {} <pid|package_name>\n\n", argv[0]);
        return 1;
    }

    pid_t pid = -1;

    try {
        if (isdigit(argv[1][0])) {
            pid = std::stoi(argv[1]);
        }
    } catch (const std::exception &e) {
        std::string appPkg = argv[1];
        pid = KittyMemoryEx::getProcessID(appPkg);
    }

    // set_selinux_state ...
    // ...

    string lib = get_self_path();
    bool use_memfd = false, hide_maps = false, hide_solist = false, stopped = false;

    injected_info_t ret{};
    if (kitInjector.init(pid, EK_MEM_OP_IO)) {
        fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] KittyInjector init\n");
        if (kitInjector.attach()) {
            fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] KittyInjector attach\n");
        } else {
            fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] KittyInjector attach failed\n");
            return 0;
        }
        auto tm_start = std::chrono::high_resolution_clock::now();

        ret = kitInjector.injectLibrary(lib, RTLD_NOW | RTLD_LOCAL, use_memfd, hide_maps, hide_solist,
                                        [&pid, &stopped](injected_info_t &injected) {
                                            if (injected.is_valid() && stopped) {
                                                fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] Continuing target process...\n");
                                                kill(pid, SIGCONT);
                                                stopped = false;
                                            }
                                        });

        inj_ms = std::chrono::high_resolution_clock::now() - tm_start;
        if (inj_ms.count() > 0)
            fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] Injection took {} MS.\n", inj_ms.count());

        kitInjector.detach();
    }

    return 0;
}