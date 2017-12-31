class Calc
	RIGHT	= 0x01
	LEFT	= 0x02
	UP		= 0x03
	DOWN	= 0x04

	ENTER	= 0x05
	CLEAR	= 0x09
	DEL		= 0x0A
	
	PRGM	= 0x2D
	CATALOG	= 0x3E

	DRAW	= 0x2F
	PEN		= 0x6D
	CIRCLE	= 0x6A
	LINE	= 0x6C
	CLRDRAW	= 0x6B

	VERT	= 0x71
	HORIZ	= 0x72
	TEXT	= 0x73

	COMMA	= 0x8B
	QUOTE	= 0xCB

	def getNumber(num)
		if(num>=0 && num<=9)
			return num+0x8E
		else
			return 0x8E
		end
	end

	def getAlpha(letter)
		if(letter[0]>=65 && letter[0]<=90)
			index = letter[0]-65
			return 0x9A+index
		else
			return 0x99
		end
	end
end
