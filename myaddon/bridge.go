// bridge.go
package main

// typedef void (*cb)(char* extra, char* data);
// void callCb(cb callback, char* extra , char* arg) { // c的回调, go将通过这个函数回调c代码
//    callback(extra,arg);
// }
import "C"