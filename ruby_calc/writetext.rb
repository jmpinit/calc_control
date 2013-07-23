require 'rubygems'
require 'nokogiri'
require 'open-uri'
require 'rubygems'
require 'serialport'
require './calc.rb'

#@sp = SerialPort.new("/dev/ttyUSB0", 115200)
@calc = Calc.new

def pressKey(key)
	@sp.putc(key.chr);
	sleep 0.1
end

@x = 46
@y = 30

#clear stuff to make ready
#pressKey(Calc::CLEAR)
#pressKey(Calc::CLRDRAW)
#pressKey(Calc::ENTER)

def text(x, y, msg)
	xText = x.to_s
	yText = y.to_s

	pressKey(Calc::CLEAR)
	pressKey(Calc::TEXT)

	for i in 0...yText.length
		pressKey(@calc.getNumber(yText[i]-48))
	end

	pressKey(Calc::COMMA)

	for i in 0...xText.length
		pressKey(@calc.getNumber(xText[i]-48))
	end

	pressKey(Calc::COMMA)

	pressKey(Calc::QUOTE)
	for i in 0...msg.length
		pressKey(@calc.getAlpha(msg[i].chr))
	end
	pressKey(Calc::QUOTE)

	pressKey(Calc::ENTER)
end


def moveTo(x, y)
	distx = x-@x
	disty = y-@y

	if(distx>0)
		for i in 0...distx
			pressKey(Calc::RIGHT)
		end
	else
		for i in 0...-distx
			pressKey(Calc::LEFT)
		end
	end
	
	if(disty>0)
		for i in 0...disty
			pressKey(Calc::DOWN)
		end
	else
		for i in 0...-disty
			pressKey(Calc::UP)
		end
	end

	@x = x
	@y = y
end

#grab reddit homepage
#wmctrl -r google -e 0,0,0,384,256
#import -window 0x0200002c -resize 96x64 "test.png"

file = open('http://reddit.com')
doc = Nokogiri::HTML(file)
file.close

results = doc.search('body a')
for i in 0..results.length
	puts results[i].content
end
