// gofun.go
package main

// int a;
// typedef void (*cb)(char* data);
// extern void callCb(cb callback, char* extra, char* arg);
import "C" // C是一个虚包, 上面的注释是c代码, 可以在golang中加 `C.` 前缀访问, 具体参考上面给出的文档
import "time"

//export hello
func hello(arg *C.char) *C.char  {
    //name := gjson.Get(arg, "name")
    //return "hello" + name.String()
    return C.CString("hello peter:::" + C.GoString(arg))
} // 通过export注解,把这个函数暴露到动态链接库里面

//export helloP
func helloP(arg *C.char, cb C.cb, extra *C.char) *C.char  {
    C.callCb(cb, extra, C.CString("one"))
    time.Sleep(time.Second)
    C.callCb(cb, extra, C.CString("two"))
    return C.CString("hello peter:::" + C.GoString(arg))
}

func main() {
    println("go main func")
}