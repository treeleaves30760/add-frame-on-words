import os
from PIL import Image

os.system('.\\main.exe')

im = Image.open("out.ppm")
im.save("out.jpg")
im.show()
