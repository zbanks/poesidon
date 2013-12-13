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

offset = 0
blocksize = 512 

with open("images.h", "w") as fh:
    with open("images.c", "w") as fc:
        with open("images.obj", "wb") as fo:
            fc.write("#include <images.h>\n\n")
            fh.write("#include <stdint.h>\n\n")
            for fname in sys.argv[1:]:

                im=Image.open(fname)
                im=im.rotate(-90)

                sprites = fname.endswith(".spng")
                obj = fname.endswith(".opng")

                if not obj or not sprites:
                    fh.write("// Bitmap File: '{}'\n".format(fname))
                    fc.write("// Bitmap File: '{}'\n".format(fname))
                    pixels=','.join(['0x{0:X}'.format(condense(*pix)) for pix in im.getdata()])
                    varname = fname.split('.')[0].split("/")[-1].upper()
                    size = (im.size[0] * im.size[1])
                    
                    fc.write("const uint8_t {0}_DATA[{1}] = {{{2}}};\n".format(varname,im.size[0]*im.size[1],pixels))
                    fc.write("\n")

                    fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_DATA\n".format(varname))
                    fh.write("#define {0}_WIDTH ({1})\n".format(varname,im.size[0]))
                    fh.write("#define {0}_HEIGHT ({1})\n".format(varname,im.size[1]))
                    fh.write("extern const uint8_t {0}_DATA[{1}];\n".format(varname,im.size[0]*im.size[1]))
                    fh.write("\n")
                elif obj:
                    fh.write("// SD Object Bitmap File: '{}'\n".format(fname))
                    fc.write("// SD Object Bitmap File: '{}'\n".format(fname))
                    pixels=','.join(['0x{0:X}'.format(condense(*pix)) for pix in im.getdata()])
                    varname = fname.split('.')[0].split("/")[-1].upper()
                    size = (im.size[0] * im.size[1])
                    
                    raw_data = "".join([chr(condense(*pix)) for pix in im.getdata()])
                    if size % blocksize != 0:
                        pad_length = blocksize  - (size % blocksize)
                    else:
                        pad_length = 0
                    pad = "\0" * pad_length
                    raw_data_pad = raw_data + pad
                    total_length = size + pad_length
                    fo.write(raw_data_pad)

#fc.write("const uint8_t {0}_DATA[{1}] = {{{2}}};\n".format(varname,im.size[0]*im.size[1],pixels))
                    fc.write("\n")

#fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_DATA\n".format(varname))
                    fh.write("#define {0}_IMAGE {0}_WIDTH,{0}_HEIGHT,(uint8_t*){0}_OFFSET\n".format(varname))
                    fh.write("#define {0}_WIDTH ({1})\n".format(varname,im.size[0]))
                    fh.write("#define {0}_HEIGHT ({1})\n".format(varname,im.size[1]))
                    fh.write("#define {0}_OFFSET ({1})\n".format(varname,hex(offset)))
#fh.write("extern const uint8_t {0}_DATA[{1}];\n".format(varname,im.size[0]*im.size[1]))
                    fh.write("\n")

                    offset += total_length
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
