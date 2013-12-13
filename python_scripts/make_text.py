from PIL import Image,ImageDraw,ImageFont
import sys

#txt=sys.argv[3]

fontsize=int(sys.argv[1])

for txt in sys.argv[2:]:
    font = ImageFont.truetype("/usr/share/fonts/truetype/msttcorefonts/Comic_Sans_MS_Bold.ttf", fontsize)
#font = ImageFont.truetype("/usr/share/fonts/truetype/msttcorefonts/Comic_Sans_MS.ttf", fontsize)
    draw=ImageDraw.Draw(Image.new("RGB",(1,1)))
    ts=draw.textsize(txt,font)

    image = Image.new("RGB", ts, (255,255,255))
    draw = ImageDraw.Draw(image)

    draw.text((0, 0), txt, (0,0,0), font=font)

    image.save("txt_{}.png".format(txt.replace(" ", "_")))
