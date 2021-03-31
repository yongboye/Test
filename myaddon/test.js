// test.js
let addon = require('./build/Release/sdkaddon');
let success = function (data) {
    console.log("leo")
    console.log(data);
}
let fail = function (error) {
    console.log('peter')
    console.log(error)
}
addon.loadCore('gofun.so')
addon.callCoreAsync('hello', JSON.stringify({name: '我爱你'}), success, fail)
setTimeout(function () {
    addon.callCoreAsync('helloP', JSON.stringify({name: '我爱你1'}), success, fail, function (data) {
        console.log('js log:' + data)
    })
})
