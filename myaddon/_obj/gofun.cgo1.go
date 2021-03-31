// Code generated by cmd/cgo; DO NOT EDIT.

//line gofun.go:1:1
// gofun.go
package main

// int a;
// typedef void (*cb)(char* data);
// extern void callCb(cb callback, char* extra, char* arg);
import _ "unsafe" // C是一个虚包, 上面的注释是c代码, 可以在golang中加 `C.` 前缀访问, 具体参考上面给出的文档
import "time"

//export hello
func hello(arg * /*line :11:17*/_Ctype_char /*line :11:23*/) * /*line :11:26*/_Ctype_char /*line :11:32*/  {
    //name := gjson.Get(arg, "name")
    //return "hello" + name.String()
    return ( /*line :14:12*/_Cfunc_CString /*line :14:20*/)("hello peter:::" + ( /*line :14:41*/_Cfunc_GoString /*line :14:50*/)(arg))
} // 通过export注解,把这个函数暴露到动态链接库里面

//export helloP
func helloP(arg * /*line :18:18*/_Ctype_char /*line :18:24*/, cb  /*line :18:29*/_Ctype_cb /*line :18:33*/, extra * /*line :18:42*/_Ctype_char /*line :18:48*/) * /*line :18:51*/_Ctype_char /*line :18:57*/  {
    ( /*line :19:5*/_Cfunc_callCb /*line :19:12*/)(cb, extra, ( /*line :19:25*/_Cfunc_CString /*line :19:33*/)("one"))
    time.Sleep(time.Second)
    ( /*line :21:5*/_Cfunc_callCb /*line :21:12*/)(cb, extra, ( /*line :21:25*/_Cfunc_CString /*line :21:33*/)("two"))
    return ( /*line :22:12*/_Cfunc_CString /*line :22:20*/)("hello peter:::" + ( /*line :22:41*/_Cfunc_GoString /*line :22:50*/)(arg))
}

func main() {
    println("go main func")
}