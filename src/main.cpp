#include "main.h"
#include "test.h"
#include <string>
#include <unistd.h>

using namespace std;

KittyInjector kitInjector;
std::chrono::duration<double, std::milli> inj_ms{};

constexpr bool ONLY_START_VM = DEBUG_LOCAL;
int main(int argc, char *argv[]) {

    std::thread t([]() {
        pthread_setname_np(pthread_self(), "dohook");
        while (true) {
            if (checkIl2cppLoaded())
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    if (ONLY_START_VM) {
        JNI_OnLoad(nullptr, nullptr);
        return 0;
    }

    // raise(SIGSTOP);

    if (argc != 2) {
        console->error("Usage: {} <pid|package_name>", argv[0]);
        return 1;
    }

    pid_t pid = -1;

    try {
        if (isdigit(argv[1][0])) {
            pid = std::stoi(argv[1]);
        }
    } catch (const std::exception &e) {
        std::string appPkg = argv[1];
        // not working
        pid = KittyMemoryEx::getProcessID(appPkg);
    }

    // set_selinux_state ...
    // ...

    string lib = get_self_path();
    bool use_memfd = false, hide_maps = false, hide_solist = false, stopped = false;

    injected_info_t ret{};
    if (kitInjector.init(pid, EK_MEM_OP_IO)) {
        console->info("KittyInjector init");
        if (kitInjector.attach()) {
            console->info("KittyInjector attach");
        } else {
            console->error("KittyInjector attach failed");
            return 0;
        }
        auto tm_start = std::chrono::high_resolution_clock::now();

        ret = kitInjector.injectLibrary(lib, RTLD_NOW | RTLD_LOCAL, use_memfd, hide_maps, hide_solist,
                                        [&pid, &stopped](injected_info_t &injected) {
                                            if (injected.is_valid() && stopped) {
                                                console->info("[*] Continuing target process...");
                                                kill(pid, SIGCONT);
                                                stopped = false;
                                            }
                                        });

        inj_ms = std::chrono::high_resolution_clock::now() - tm_start;
        if (inj_ms.count() > 0)
            console->info("[*] Injection took {} MS.", inj_ms.count());

        kitInjector.detach();
    }

    return 0;
}