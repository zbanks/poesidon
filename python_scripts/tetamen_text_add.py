from PIL import Image,ImageDraw,ImageFont
import sys, textwrap

fontsize=int(sys.argv[1])

font = ImageFont.truetype("/usr/share/fonts/truetype/msttcorefonts/Comic_Sans_MS_Bold.ttf", fontsize)

imgName = sys.argv[2]

imageFile = Image.open(imgName)

blankImage = Image.new("RGB", (130, 130))

def resize(im):
	width = 77
	height = 100
	im.resize((width, height), Image.ANTIALIAS)
#	im.save(imgName.split('.')[0] + '_small.png'
	im = blankImage.paste(im, (0, 130 - height))
	return im
	
def addText(im, text):
	x_offset = 81
	y_offset = 45
	max_characters = 9

	for s in textwrap.wrap(text, max_characters):
		ts = draw.textsize(text, font)
		draw = ImageDraw.Draw(im)
		draw.text((x_offset, y_offset), ts + "\n", (255,255,255), font=font)

	im.save(imgName.split('.')[0] + '_withtext.png'

"""
def vertSplit(text):
	containerWidth = 45
	textWidth = font.getsize(text)[0]
	newText = ""

	if (textWidth > containerWidth):
		text = text.split(" ")
		for i in range(len(text))[1:]:
			if (font.getsize(text[i-1] + ' ' + text[i]) > containerWidth):
				newText += text[i-1] + ' ' + text[i]
			else:
				newText += text[i-1]
"""	

imageFile = resize(imageFile)
addText(imageFile, eval(sys.argv[3:]))
