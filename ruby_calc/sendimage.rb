require 'RMagick'
require 'rubygems'
require 'serialport'
require './calc.rb'

@sp = SerialPort.new("/dev/ttyUSB0", 115200)

puts "drawing "+ARGV[0]

#draw an image
img = Magick::ImageList.new(ARGV[0])

@pixels = []

img.each_pixel do |pixel, c, r|
	#pixels.push(pixel)
	@pixels.push(((pixel.red+pixel.green+pixel.blue)/3)>(65535/2))
end

def getPixel(x, y)
	if(x>0 && y>0 && x<96 && y<64)
		return @pixels[y*96+x]
	else
		return true 
	end
end

for y in 0...64
	for x in 0...12
		byte = 0
		for bit in 0...8
			if not getPixel(x*8+bit, y)
				byte = byte|(1<<(7-bit))
			end
		end
		
		@sp.putc(byte.chr);
		sleep 0.01
		
		puts "sent "+(y*12+x+1).to_s+" bytes."
	end
end
