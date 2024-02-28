#include "xdl.h"
#include "bindings.h"

#include "rttr/registration"
#include <fmt/color.h>
#include <sstream>

using namespace rttr;

RTTR_REGISTRATION {
    registration::class_<xdl_info_t>("xdl_info_t")
        .property("dli_fname", &xdl_info_t::dli_fname)
        .property("dli_fbase", &xdl_info_t::dli_fbase)
        .property("dli_sname", &xdl_info_t::dli_sname)
        .property("dli_saddr", &xdl_info_t::dli_saddr)
        .property("dli_ssize", &xdl_info_t::dli_ssize)
        .property("dlpi_phdr", &xdl_info_t::dlpi_phdr)
        .property("dlpi_phnum", &xdl_info_t::dlpi_phnum);
}

class xdl_bind {
public:
    const char *infoTostring() {
        auto handle = xdl_open(EXEC_NAME, RTLD_LAZY);
        if (handle == nullptr) {
            printf("xdl_open failed\n");
        }
        xdl_info_t info;
        xdl_info(handle, XDL_DI_DLINFO, &info);
        xdl_close(handle);
        return XdlInfoToString(&info);
    }

    void info() {
        printf("%s", infoTostring());
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
    static const char *XdlInfoToString(xdl_info_t *info) {
        type t = type::get<xdl_info_t>();
        std::stringstream os;
        for (auto &prop : t.get_properties()) {
            auto name = prop.get_name();
            rttr::variant value = prop.get_value(info);
            if (value.get_type() == rttr::type::get<const char *>()) {
                const char *cstr = value.get_value<const char *>();
                os << '\t' << name << ": " << (cstr ? cstr : "<null>") << std::endl;
            } else if (value.get_type() == rttr::type::get<size_t>()) {
                os << '\t' << name << ": " << value.get_value<size_t>() << std::endl;
            } else if (value.get_type() == rttr::type::get<void *>()) {
                os << '\t' << name << ": " << value.get_value<void *>() << std::endl;
            } else {
                os << '\t' << name << ": " << value.get_value<void *>() << std::endl;
            }
        }
        return os.str().c_str();
    }
};

void reg_xdl(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginClass<xdl_bind>("xdl_bind")
        .addFunction("info", &xdl_bind::info)
        .addFunction("infoTostring", &xdl_bind::infoTostring)
        .addFunction("findSymbyName", &xdl_bind::findSymbyName)
        .addFunction("iterate_phdr", &xdl_bind::iterate_phdr)
        .addFunction("addressInfo", &xdl_bind::addressInfo)
        .endClass();
    static auto xdl = new xdl_bind();
    luabridge::setGlobal(L, xdl, "xdl");
    fmt::print(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "[*] luabridge bind {}\n", "xdl");
}