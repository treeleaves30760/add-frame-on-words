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
img = Image.open('test.ppm')

os.system('.\\main')

im = Image.open("out.ppm")
im = im.convert('RGBA')

# Transparency
newImage = []
for item in im.getdata():
    if item[:3] == (0, 0, 0):
        newImage.append((255, 255, 255, 0))
    else:
        newImage.append(item)

im.putdata(newImage)

im.save('out.png')
im.show()
