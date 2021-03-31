/*function testcallback(p){
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

let success = function(data)
{
	console.log(data);
}
let fail = function(error)
{
	console.log(error);
}*/





console.log("------------------");
console.log("start...");

var addon = require("./build/Release/sdkaddon.node");
//var addon = require("bindings")("sdkaddon");

//console.log(addon.test("hello"));
//addon(testcallback);
//console.log(addon.add(3, 5));
//addon.runCallback(testcallback);
//addon.runCallback2(testcallback, "yyb");
		  
/*console.log( 'hdhardwareinit ret=' + addon.hdhardwareinit("D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/nodejs-dynamiclink-example-master/myaddon/callback.js", 					 "{\"System\":{\"authorization_code\":\"754a0b2598ffa1839ea165829918396d1a06967e7de87304\"},\"CoinDevType\":{\"coin_type\":\"-1\",\"national_cny\":\"1\",\"serial_coin_in\":\"/dev/ttyUSB0\",\"include_coin_out_half\":\"1\",\"include_coin_out_one\":\"1\",\"box_coin_out_one\":\"1\",\"serial_coin_out_half\":\"2\",\"serial_coin_out_one\":\"3\"},\"CashInDevType\":{\"cash_in_type\":\"-1\",\"serial_cash_in\":\"COM5\",\"cash_accept\":\"15\",\"cash_model\":\"0\"},\"CashOutDevType\":{\"cash_out_type\":\"-1\",\"serial_cash_out\":\"/dev/ttyUSB0\",\"cashbox_num\":\"2\",\"cashbox0_money_type\":\"20\",\"cashbox1_money_type\":\"5\",\"cashbox2_money_type\":\"-1\",\"cashbox3_money_type\":\"-1\",\"cashbox4_money_type\":\"-1\",\"cashbox5_money_type\":\"-1\",\"cashbox0_ID\":\"137\",\"cashbox1_ID\":\"134\",\"cashbox2_ID\":\"-1\",\"cashbox3_ID\":\"-1\",\"cashbox4_ID\":\"-1\",\"cashbox5_ID\":\"-1\",\"country_currentcy\":\"0\"},\"UnionDevType\":{\"union_type\":\"-1\",\"serial_union\":\"/dev/ttyUSB0\",\"union_zhongyu\":\"1\",\"union_BsweepC\":\"0\"},\"PrinterDevType\":{\"printer_type\":\"-1\",\"serial_printer\":\"COM9\",\"baudrate_printer\":\"19200\"},\"ICcardDevType\":{\"iCcard_type\":\"1\",\"serial_iccard\":\"/dev/ttyUSB0\"}}", 
								  "D:/commit/basedOnTheComponent/HWLIB/KT_NJGYPCC_1.0.0/golang/myaddon/logs", 
								  "2020-03-14 15:46:01", 
								  success,
								  fail,
								  icCardCallback,
								  cashinCallback) );*/							  
//console.log(addon.hdprinterecho(1, "www.baidu.com", 0, false, 0));	
//console.log(addon.hdprinterecho(3, "www.baidu.com", 0, false, 0));		
//console.log(addon.hdprintercut());
//console.log(addon.hdprinterecho(2, "www.baidu.com", 0, false, 0)); //带切纸
//console.log(addon.hdICcardenable(35));
//console.log(addon.hdcashinenable());
//console.log(addon.HardWareEnd());
					  
console.log("end...");					 
//console.log("------------------");




