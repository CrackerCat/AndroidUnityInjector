### UnityHook的缝合怪实现法（刚写还没法用，大致思路如下）

#### 目的：脱离frida，使用ptrace的方式注入一个lua虚拟机，并绑定 Unity Hook Cpp Impl
（后面考虑还是使用JavaScript虚拟机）

##### Lua 绑定使用到 [LuaBridge](https://github.com/vinniefalco/LuaBridge) 

##### JavaScript虚拟机有很多备选
1. [V8](https://chromium.googlesource.com/v8/v8.git) 还得是google优选
2. [QuickJS](https://github.com/bellard/quickjs) 经典的极简虚拟机，但是不支持lambda
3. [Duktape](https://github.com/svaarala/duktape) 体验也不错,多了一个inline repl，但是依旧不支持lambda
4. [jerryscript](https://github.com/jerryscript-project/jerryscript) 编译了一个windows版本的好像还挺中规中矩的
5. [ChakraCore](https://github.com/chakra-core/ChakraCore) 来自微软的ERROR: Unsupported target processor: aarch64 (-.-!)
   
##### 总体还是觉得v8比较舒服（除了编译麻烦点），配合dobby hook函数可以使用到lambda，命令行使用起来更简洁

##### UnityHook 使用到
1. [UnityResolve](https://github.com/issuimo/UnityResolve.hpp)  只是简单测试过安卓平台,单文件实现很简洁很香
2. [IL2CPP_Resolver](https://github.com/sneakyevil/IL2CPP_Resolver)  还没测试

##### Inject的参考代码
1. [AndKittyInjector](https://github.com/MJx0/AndKittyInjector) 最完备的实现
2. [AndroidPtraceInject](https://github.com/SsageParuders/AndroidPtraceInject) 注释非常清晰
3. [TinyInjector](https://github.com/shunix/TinyInjector)
实现也都是ptrace注入动态库，保存寄存器，然后远程调用，恢复寄存器 （这里后续的实现的化就考虑注入自己）

##### 与Lua虚拟机交互
- 安卓本地端创建一个socket服务器，远端windows/linux使用python或者再编译一个命令行程序用来与安卓通信

---

使用的话还是很常规的操作

`

push uinjector to /data/local/tmp

adbe start com.xxx.xxx -> start app

adb shell pidof com.xxx.xxx -> get pid

setenforce 0

./data/local/tmp/uinjector ${pid}

`

然后就是远端操作了，
这里由于还有没客户端代码，所以展示socket的链接测试就用nc, 端口用的是8024

`
    nc 127.0.0.1 8024
`

然后界面没有提示，但是是一个阻塞的socket，可以当作lua shell使用


---

然后想要实现的api大致同 [Il2CppHookScripts](https://github.com/axhlzy/Il2CppHookScripts) 这里面的常用api
