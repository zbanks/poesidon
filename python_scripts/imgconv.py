from PIL import Image
import sys

def condense(r,g,b,a=0):
	r=r>>5
	g=g>>5
	b=b>>6
	return r<<5 | g<<2 | b

with open("images.h", "w") as fh:
    with open("images.c", "w") as fc:
        fc.write("#include <images.h>\n")
        fh.write("#include <stdint.h>\n")
        for fname in sys.argv[1:]:
            fh.write("// File: '{}'\n".format(fname))
            fc.write("// File: '{}'\n".format(fname))

            im=Image.open(fname)
            im=im.rotate(-90)
            pixels=','.join(['0x{0:X}'.format(condense(*pix)) for pix in im.getdata()])
            varname = fname.split('.')[0].split("/")[-1].upper()

            fc.write("const uint8_t {0}_WIDTH = {1};\n".format(varname,im.size[0]))
            fc.write("const uint8_t {0}_HEIGHT = {1};\n".format(varname,im.size[1]))
            fc.write("const uint8_t {0}_DATA[{1}] = {{{2}}};\n".format(varname,im.size[0]*im.size[1],pixels))
            fc.write("\n")

            fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_DATA\n".format(varname))
            fh.write("extern const uint8_t {0}_WIDTH;\n".format(varname))
            fh.write("extern const uint8_t {0}_HEIGHT;\n".format(varname))
            fh.write("extern const uint8_t {0}_DATA[{1}];\n".format(varname,im.size[0]*im.size[1]))
            fh.write("\n")
