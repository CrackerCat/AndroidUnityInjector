#include "xdl.h"
#include "binds.h"

class xdl_bind {
public:
    void info() {
        auto handle = xdl_open(EXEC_NAME, RTLD_LAZY);
        if (handle == nullptr) {
            printf("xdl_open failed\n");
            return;
        }
        xdl_info_t info;
        xdl_info(handle, XDL_DI_DLINFO, &info);
        printXDLInfo(&info);
        xdl_close(handle);
    }

    void addressInfo(size_t p) {
        xdl_info_t info;
        auto handle = xdl_open(EXEC_NAME, RTLD_LAZY);
        if (handle == nullptr) {
            printf("xdl_open failed\n");
            return;
        }
        xdl_info(handle, XDL_DI_DLINFO, &info);
        xdl_addr((void *)p, &info, nullptr);
        printf("xdl_info: %p\n", info);
        xdl_close(handle);
    }

    void findSymbyName(const char *lib, const char *name) {
        if (lib == "")
            lib = EXEC_NAME;
        auto handle = xdl_open(lib, RTLD_LAZY);
        void *func = xdl_sym(handle, name, nullptr);
        printf("%p @ %s\n", func, name);
        xdl_close(handle);
    }

    void iterate_phdr() {
        auto handle = xdl_open(EXEC_NAME, RTLD_LAZY);
        xdl_iterate_phdr(
            [](struct dl_phdr_info *info, size_t size, void *data) -> int {
                printf("dlpi_name: %s\n", info->dlpi_name);
                printf("\tdlpi_addr: %p\n", info->dlpi_addr);
                printf("\tdlpi_phdr: %p\n", info->dlpi_phdr);
                printf("\tdlpi_phnum: %p\n", info->dlpi_phnum);
                return 0;
            },
            nullptr, XDL_DEFAULT);
        xdl_close(handle);
    }

private:
    static void printXDLInfo(xdl_info_t *info) {
        printf("xdl_info: %p\n", info);
        printf("dli_fname: %s\n", info->dli_fname);
        printf("dli_fbase: %p\n", info->dli_fbase);
        printf("dli_sname: %s\n", info->dli_sname);
        printf("dli_saddr: %p\n", info->dli_saddr);
        printf("dli_ssize: %p\n", info->dli_ssize);
        printf("dlpi_phdr: %p\n", info->dlpi_phdr);
        printf("dlpi_phnum: %p\n", info->dlpi_phnum);
    }
};

void reg_xdl(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginClass<xdl_bind>("xdl_bind")
        .addFunction("info", &xdl_bind::info)
        .addFunction("findSymbyName", &xdl_bind::findSymbyName)
        .addFunction("iterate_phdr", &xdl_bind::iterate_phdr)
        .addFunction("addressInfo", &xdl_bind::addressInfo)
        .endClass();
    static auto xdl = new xdl_bind();
    luabridge::setGlobal(L, xdl, "xdl");
    DEBUG_PRINT("[*] luabridge bind xdl\n");
}