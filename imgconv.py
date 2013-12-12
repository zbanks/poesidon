from PIL import Image
import sys

def condense(r,g,b,a=0):
	r=r>>5
	g=g>>5
	b=b>>6
	return r<<5 | g<<2 | b

f=sys.argv[1]
im=Image.open(f)
im=im.rotate(-90)
pixels=','.join(['0x{0:X}'.format(condense(*pix)) for pix in im.getdata()])
varname=sys.argv[1].split('.')[0].upper()
print "const uint8_t {0}_WIDTH = {1};".format(varname,im.size[0])
print "const uint8_t {0}_HEIGHT = {1};".format(varname,im.size[1])
print "const uint8_t {0}_DATA[{1}] = {{{2}}};".format(varname,im.size[0]*im.size[1],pixels)
