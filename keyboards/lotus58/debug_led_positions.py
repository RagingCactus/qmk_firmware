from PIL import Image, ImageDraw

keys = [
    ( 55,  2), ( 44,  1), ( 33,  0), ( 22,  1), ( 11,  3), (  0,  4),
    (  0, 14), ( 11, 13), ( 22, 11), ( 33, 10), ( 44, 11), ( 55, 12),
    ( 55, 22), ( 44, 21), ( 33, 20), ( 22, 21), ( 11, 23), (  0, 24),
    (  0, 34), ( 11, 33), ( 22, 31), ( 33, 30), ( 44, 31), ( 55, 32),
    ( 70, 27), ( 66, 40), ( 50, 42), ( 39, 41) ,( 28, 40),

    (165, 2), (176, 1), (187, 0), (198, 1), (209, 3), (220, 4),
    (220, 14), (209, 13), (198, 11), (187, 10), (176, 11), (165, 12),
    (165, 22), (176, 21), (187, 20), (198, 21), (209, 23), (220, 24),
    (220, 34), (209, 33), (198, 31), (187, 30), (176, 31), (165, 32),
    (150, 27), (154, 40), (170, 42), (181, 41), (192, 40),
]

red = (255, 0, 0)
green = (0, 255, 0)
black = (0, 0, 0)
white = (255, 255, 255)
grey = (222, 222, 222)

img_size = (256, 128)
usual_dimensions = (225, 65)

im = Image.new("RGBA", img_size, white)
draw = ImageDraw.Draw(im)

draw.rectangle([(0, 0), usual_dimensions], (grey))

center = (110, 21)
im.putpixel(center, red)

last = None

for key in keys:
    if last:
        draw.line([last, key], green)
        im.putpixel(last, black)
    im.putpixel(key, black)
    last = key

im.show()
