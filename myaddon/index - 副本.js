function sleep(numberMillis) {
    var now = new Date();
    var exitTime = now.getTime() + numberMillis;
    while (true) {
        now = new Date();
        if (now.getTime() > exitTime)
            return;
    }
}

function testcallback(p){
	console.log('回调成功啦test-->'+p);
}

function icCardCallback(param){
	console.log('=========读卡器回调成功啦！=========');
	console.log(param);
}
function cashinCallback(param){
	console.log('=========纸币器回调成功啦！=========');
	console.log(param);
}
function sensorCallback(param){
	console.log('=========票取走传感器回调成功啦！=========');
	console.log(param);
}

let success = function(data)
{
	console.log(data);
}
let fail = function(error)
{
	console.log(error);
}


function nowDate()
{
    var dateDigitToString = function (num) {
        return num < 10 ? '0' + num : num;
    };              //这个函数的作用是把个位数的十位置0,凑成两位数，如：08 04

    var currentDate = new Date(),
        year = dateDigitToString(currentDate.getFullYear()),
        month = dateDigitToString(currentDate.getMonth() + 1),//Date.getMonth()的返回值是0-11,所以要+1
        date = dateDigitToString(currentDate.getDate()),
        hour = dateDigitToString(currentDate.getHours()),
        minute = dateDigitToString(currentDate.getMinutes()),
        second = dateDigitToString(currentDate.getSeconds()),
        formattedDateString = year + '年' + month + '月' + date + '日 ' + hour + ':' + minute + ':' + second;
    console.log(formattedDateString);
}


console.log("require sdkaddon.node");
var addon = require("D:/golang/myaddon/build/Release/sdkaddon.node");

nowDate();				  
console.log("end...");






					 
//export LD_LIBRARY_PATH=~/libs:$LD_LIBRARY_PATH
//node-gyp rebuild --dist-url=https://electronjs.org/headers --target=5.0.13 --verbose













