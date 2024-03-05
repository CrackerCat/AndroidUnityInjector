#include "main.h"

using namespace std;

string get_self_path() {
    char buf[1024];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return string(buf);
    } else {
        return "";
    }
}