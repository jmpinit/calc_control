require 'rmagick'
require 'rubygems'
require 'serialport'
require '../ti-link-protocol.rb'

if ARGV.length != 2
  puts "Usage: draw-image.rb <image file> <serial port>"
  exit(1)
end

serialPortPath = ARGV[1]

@sp = SerialPort.new(serialPortPath, 115200)

def pressKey(key)
	@sp.putc(key.chr);
	sleep 0.08
end

@x = 46
@y = 30

puts "drawing "+ARGV[0]

pressKey(Calc::CLEAR)
pressKey(Calc::CLRDRAW)
pressKey(Calc::ENTER)
pressKey(Calc::PEN)

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

#draw an image
img = Magick::ImageList.new(ARGV[0])

@pixels = []

img.each_pixel do |pixel, c, r|
	#pixels.push(pixel)
	@pixels.push(((pixel.red+pixel.green+pixel.blue)/3)>(65535/2))
end

def getPixel(x, y)
	if(x>0 && y>0 && x<95 && y<63)
		return @pixels[y*95+x]
	else
		return true 
	end
end

#blobify
def blob(x, y)
	#puts "blobbed at ("+x.to_s+", "+y.to_s+")"

	if(getPixel(x, y))
		return
	end

	moveTo(x, y)
	pressKey(Calc::ENTER)
	pressKey(Calc::ENTER)
	@pixels[y*95+x] = true

	blob(x-1, y)
	blob(x+1, y)
	blob(x, y-1)
	blob(x, y+1)
	blob(x-1, y-1)
	blob(x-1, y+1)
	blob(x+1, y-1)
	blob(x+1, y+1)
end

@done = false

while(!@done)
	@plotX = 0
	@plotY = 0
	while(getPixel(@plotX, @plotY))
		@plotX += 1;
		if(@plotX>95)
			@plotX = 0
			@plotY += 1
		end

		if(@plotY>63)
			@done = true
		end
	end

	blob(@plotX, @plotY)
end
puts "done"
exit

@plotX = 0
@plotY = 0
for i in 0..pixels.length
	if(((pixels[i].red+pixels[i].green+pixels[i].blue)/3)>(65535/2))
		print ' '
	else
		print 'x'
		moveTo(@plotX, @plotY)
		pressKey(Calc::ENTER)
		pressKey(Calc::ENTER)
	end

	@plotX += 1
	if(i%95==0 && i!=0)
		puts
		@plotX = 0
		@plotY += 1
	end
end

puts
