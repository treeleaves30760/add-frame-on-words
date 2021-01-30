import os
from PIL import Image

s = 'test.png'
print("Do the image you want to add frame call test.png? (y/n):")
n = input()
if n == 'n':
    print("enter the path of picture you want to add frame:")
    s = input()
img = Image.open(s)
img.save('test.ppm')

os.system('.\\main')

im = Image.open("out.ppm")
im.save("out.jpg")
im.show()
