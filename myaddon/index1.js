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
function trafficCallback(param){
	console.log('=========交通卡回调成功啦！=========');
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
//var addon = require("bindings")("sdkaddon");
//console.log(addon.test("hello"));
//addon(testcallback);
console.log(addon.add(3, 5));

addon.runCallback(testcallback);
addon.runCallback2(testcallback, "yyb");

nowDate();
console.log("start...");

nowDate();
//sleep(3000);
nowDate();

nowDate();
console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"COM1\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"COM1\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"COM1\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"COM1\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"COM1\",\"baudrate_printer\":\"19200\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"2\",\"serial_iccard\":\"2\", \"serial_Type\":\"0\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"-1\",\"serial_sensor\":\"COM1\"}, \
								   \"TrafficDevType\":{\"traffic_type\":\"-1\",\"serial_traffic\":\"COM1\", \"industrycode_traffic\":\"01\", \"usercode_traffic\":\"01\", \"deviceid_traffic\":\"01\", \"units_traffic\":\"01\"}, \
								   \"LockDevType\":{\"LockDev_Type\":\"-1\",\"serial_lock\":\"COM1\"} }", 
								  "./logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback,
								  trafficCallback, 
								  "D:/golang/myaddon/build/Release") ); 
//console.log( 'hdlocksendlock ret=' + addon.hdlocksendlock()); 
console.log('----------------------');
nowDate();
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); 
console.log( 'hdICcardenable ret=' + addon.hdICcardenable(35));
nowDate();
console.log('----------------------');
/*nowDate();
sleep(30000);
nowDate();
nowDate();
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); */
//console.log( 'hdTrafficEnable ret=' + addon.hdTrafficEnable(100)); 
//sleep(30000);
//console.log( 'hdTrafficReadLog ret=' + addon.hdTrafficReadLog()); 
//console.log( 'hdTrafficDisable ret=' + addon.hdTrafficDisable());
//console.log( 'hdprintercheckstatus ret=' + addon.hdprintercheckstatus());
//console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
//console.log( 'hdsensorenable ret=' + addon.hdsensorenable());
//console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
//sleep(30000);
//console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	



/*console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"COM1\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"COM1\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"COM1\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"COM1\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"COM1\",\"baudrate_printer\":\"19200\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"-1\",\"serial_iccard\":\"COM1\", \"serial_Type\":\"0\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"-1\",\"serial_sensor\":\"COM1\"}, \
								   \"TrafficDevType\":{\"traffic_type\":\"0\",\"serial_traffic\":\"COM1\"}, \
								   \"LockDevType\":{\"LockDev_Type\":\"-1\",\"serial_lock\":\"COM1\"} }", 
								  "./logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback,
								  trafficCallback) ); 
console.log( 'hdTrafficEnable ret=' + addon.hdTrafficEnable(10)); 
sleep(30000);
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	
sleep(10000);*/

/*console.log( 'hdprinterecho ret=' + addon.hdprinterecho(1, "www.baidu.com", 0, false, 0)); //打印文本	
console.log( 'hdprinterecho ret=' + addon.hdprinterecho(1, "百度", 0, false, 0)); //打印文本
console.log( 'hdprinterecho ret=' + addon.hdprinterecho(3, "www.baidu.com", 0, false, 0)); //打印二维码		
console.log( 'hdprinterecho ret=' + addon.hdprinterecho(2, "www.baidu.com", 0, false, 0)); //打印条码 
console.log( 'hdprinterecho ret=' + addon.hdprinterecho(2, "\n\n\n", 0, false, 0)); //打印换行 
console.log( 'hdprintercut ret=' + addon.hdprintercut());*/

/*nowDate();
console.log( 'hdcashinsetnotesaccept ret=' + addon.hdcashinsetnotesaccept(63, 1, 1));
nowDate();
console.log( 'hdcashinenable ret=' + addon.hdcashinenable());
nowDate();
console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 
nowDate();
console.log( 'hdcashinenable ret=' + addon.hdcashinenable());
nowDate();
console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 
nowDate();*/


//console.log( 'hdsensorgetstatus ret=' + addon.hdsensorgetstatus());
//console.log( 'hdsensorenable ret=' + addon.hdsensorenable());
//console.log( 'hdsensordisable ret=' + addon.hdsensordisable());

/*console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 



nowDate();
console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"/dev/ttyUSB0\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"4\",\"serial_printer\":\"/dev/ttyUSB0\",\"baudrate_printer\":\"19200\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"1\",\"serial_iccard\":\"/dev/ttyUSB2\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"0\",\"serial_sensor\":\"/dev/ttyUSB1\"}}", 
								  "/home/pi/nodejs/golang/logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback) ); 
nowDate();
	  
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); 
console.log( 'hdICcardenable ret=' + addon.hdICcardenable(35));
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); 
console.log( 'hdprintercheckstatus ret=' + addon.hdprintercheckstatus());
console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
console.log( 'hdsensorenable ret=' + addon.hdsensorenable());
console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	



nowDate();
console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"/dev/ttyUSB0\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"4\",\"serial_printer\":\"/dev/ttyUSB0\",\"baudrate_printer\":\"19200\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"1\",\"serial_iccard\":\"/dev/ttyUSB2\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"0\",\"serial_sensor\":\"/dev/ttyUSB1\"}}", 
								  "/home/pi/nodejs/golang/logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback) ); 
nowDate();
	  
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); 
console.log( 'hdICcardenable ret=' + addon.hdICcardenable(35));
console.log( 'hdICcarddisable ret=' + addon.hdICcarddisable(35)); 
console.log( 'hdprintercheckstatus ret=' + addon.hdprintercheckstatus());
console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
console.log( 'hdsensorenable ret=' + addon.hdsensorenable());
console.log( 'hdsensordisable ret=' + addon.hdsensordisable());
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	*/

















/*console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"/dev/ttyACM0\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"/dev/ttyUSB1\",\"baudrate_printer\":\"38400\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"1\",\"serial_iccard\":\"/dev/ttyUSB0\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"-1\",\"serial_sensor\":\"/dev/ttyUSB0\"}}", 
								  "/home/pi/nodejs/golang/logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback) ); 
console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	


console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"1\",\"serial_cash_in\":\"/dev/ttyACM0\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"/dev/ttyUSB1\",\"baudrate_printer\":\"38400\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"-1\",\"serial_iccard\":\"/dev/tty\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"-1\",\"serial_sensor\":\"/dev/ttyUSB0\"}}", 
								  "/home/pi/nodejs/golang/logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback) ); 
console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	



console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit(								
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js",
 					 			  "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"}, \
								   \"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"}, \
								   \"CashInDevType\":{\"cash_in_type\":\"1\",\"serial_cash_in\":\"/dev/ttyACM0\",\"cash_accept\":\"63\",\"cash_model\":\"0\"}, \
								   \"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"}, \
								   \"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"}, \
								   \"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"/dev/ttyUSB1\",\"baudrate_printer\":\"38400\"}, \
								   \"ICcardDevType\":{\"iCcard_type\":\"-1\",\"serial_iccard\":\"/dev/tty\"}, \
								   \"SensorDevType\":{\"SensorDev_Type\":\"-1\",\"serial_sensor\":\"/dev/ttyUSB0\"}}", 
								  "/home/pi/nodejs/golang/logs", 
								  "2020-08-31 17:25:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback,
								  sensorCallback) ); 
console.log( 'hdcashindisable ret=' + addon.hdcashindisable()); 
console.log( 'hdhardwareend ret=' + addon.hdhardwareend(testcallback, "yyb") );	*/

nowDate();				  
console.log("end...");






					 
//export LD_LIBRARY_PATH=~/libs:$LD_LIBRARY_PATH
//node-gyp rebuild --dist-url=https://electronjs.org/headers --target=5.0.12 --verbose
//node-gyp rebuild --dist-url=https://electronjs.org/headers --target=5.0.13 --verbose
//node-gyp rebuild --dist-url=https://electronjs.org/headers --target=8.2.5 --verbose
//node-gyp rebuild --dist-url=https://electronjs.org/headers --target=9.4.4 --verbose











