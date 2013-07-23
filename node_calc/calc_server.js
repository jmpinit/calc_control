var exec = require('child_process').exec;
var sp = require("serialport")
var Sleep = require("sleep");

const KEY_GRAPH	= 0x44;
const KEY_DRAW	= 0x2F;
const KEY_ENTER	= 0x05;

const KEY_RIGHT	= 0x01;
const KEY_LEFT	= 0x02;
const KEY_UP	= 0x03;
const KEY_DOWN	= 0x04;

var serialPort = new sp.SerialPort("/dev/ttyUSB0", {
	baudrate: 9600,
	flowcontrol: true
});

function pressKey(key) {
	console.log(String.fromCharCode(key))
	serialPort.write(String.fromCharCode(key));
}

var serialPort;
sp.list(function(err, ports) {
	serialPort = new sp.SerialPort(ports[0].comName, {
		baudrate: 9600
	});

	for(var i=0; i<1000; i++) {
		serialPort.write("Hi there from linux");
	}

	console.log("connected to "+ports[0].comName);
});

