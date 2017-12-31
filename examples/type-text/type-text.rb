require 'rubygems'
require 'serialport'
require '../ti-link-protocol.rb'

@sp = SerialPort.new("/dev/ttyUSB0", 115200)

def pressKey(key)
	@sp.putc(key.chr);
	sleep 0.04
end

#type out the data
calc = Calc.new
for i in 0..ARGV[0].length
	if(ARGV[0][i]>=48 && ARGV[0][i]<=57)
		pressKey(calc.getNumber(ARGV[0][i]-48))
	else
		pressKey(calc.getAlpha(ARGV[0][i].chr.upcase))
	end
end
