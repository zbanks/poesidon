from PIL import Image
import sys

def condense(r,g,b,a=0):
	r=r>>5
	g=g>>5
	b=b>>6
	return r<<5 | g<<2 | b

def condense_sp(x, y):
    # Just use red channel
    return (x[0] & 0xF0) | (y[0] >> 4)

with open("images.h", "w") as fh:
    with open("images.c", "w") as fc:
        fc.write("#include <images.h>\n\n")
        fh.write("#include <stdint.h>\n\n")
        for fname in sys.argv[1:]:

            im=Image.open(fname)
            im=im.rotate(-90)

            sprites = fname.endswith(".spng")

            if not sprites:
                fh.write("// Bitmap File: '{}'\n".format(fname))
                fc.write("// Bitmap File: '{}'\n".format(fname))
                pixels=','.join(['0x{0:X}'.format(condense(*pix)) for pix in im.getdata()])
                varname = fname.split('.')[0].split("/")[-1].upper()

                fc.write("const uint8_t {0}_DATA[{1}] = {{{2}}};\n".format(varname,im.size[0]*im.size[1],pixels))
                fc.write("\n")

                fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_DATA\n".format(varname))
                fh.write("#define {0}_WIDTH ({1})\n".format(varname,im.size[0]))
                fh.write("#define {0}_HEIGHT ({1})\n".format(varname,im.size[1]))
                fh.write("extern const uint8_t {0}_DATA[{1}];\n".format(varname,im.size[0]*im.size[1]))
                fh.write("\n")
            else:
                fh.write("// Sprite File: '{}'\n".format(fname))
                fc.write("// Sprite File: '{}'\n".format(fname))
                # Sprites with an odd # of pixels is weird
                pixels=','.join(['0x{0:X}'.format(condense_sp(*pix)) for pix in zip(*[iter(im.getdata())] * 2)])
                varname = fname.split('.')[0].split("/")[-1].upper()

                size = (im.size[0] * im.size[1])
                size = (size / 2) + (size % 2)

                fc.write("const uint8_t {0}_DATA[{1}] = {{{2}}};\n".format(varname,size,pixels))
                fc.write("\n")

                fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_DATA\n".format(varname))
                fh.write("#define {0}_WIDTH ({1})\n".format(varname,im.size[0]))
                fh.write("#define {0}_HEIGHT ({1})\n".format(varname,im.size[1]))
                fh.write("extern const uint8_t {0}_DATA[{1}];\n".format(varname,size))
                fh.write("\n")
