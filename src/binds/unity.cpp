#include "binds.h"
#include "xdl.h"

class unity_bind {
public:
    unity_bind() {
        void *handle_xdl = xdl_open("libil2cpp.so", RTLD_LAZY);
        UnityResolve::Init(handle_xdl, UnityResolve::Mode::Il2Cpp);
    }

    void info() {
        // todo get unity packagename version etc.
    }

    void assemblies() {
        std::vector<UnityResolve::Assembly *> assemblies = UnityResolve::assembly;
        int index = -1;
        std::for_each(assemblies.begin(), assemblies.end(), [&](auto item) {
            // std::cout << item->name << "\tAssembly address: " << item->address << std::endl;
            // logd("%p -> %s", item->address, item->name.c_str()); });
            printf("[%d] %p -> %s\n", ++index, item->address, item->name.c_str());
        });
    }

    const char *Get(const char *name) {
        return UnityResolve::Get(name)->name.c_str();
    }
};

void reg_unity(lua_State *L) {
    luabridge::getGlobalNamespace(L)
        .beginClass<unity_bind>("unity_bind")
        .addConstructor<void (*)()>()
        .addFunction("info", &unity_bind::info)
        .addFunction("assemblies", &unity_bind::assemblies)
        .endClass();
    static auto unity_ins = new unity_bind();
    luabridge::setGlobal(L, unity_ins, "unity");
    DEBUG_PRINT("[*] luabridge bind unity\n");
}
