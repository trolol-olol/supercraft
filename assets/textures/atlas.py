from PIL import Image
import os

CELL_SIZE = 16
GRID_SIZE = 16
ATLAS_SIZE = CELL_SIZE * GRID_SIZE

PURPLE = (255, 0, 255)
BLACK = (0, 0, 0)

atlas = Image.new('RGB', (ATLAS_SIZE, ATLAS_SIZE), (0, 0, 0))

for cell_x in range(GRID_SIZE):
    for cell_y in range(GRID_SIZE):
        base_x = cell_x * CELL_SIZE
        base_y = cell_y * CELL_SIZE
        
        for px in range(CELL_SIZE):
            for py in range(CELL_SIZE):
                if px < 8 and py < 8:
                    color = PURPLE
                elif px >= 8 and py >= 8:
                    color = PURPLE
                else:
                    color = BLACK
                
                atlas.putpixel((base_x + px, base_y + py), color)

for x in range(1,256):
    fname = f"{x}.png"
    if os.path.exists(fname):
        try:
            tex=Image.open(fname).resize((CELL_SIZE, CELL_SIZE))
            cell_x=x%GRID_SIZE
            cell_y=x//GRID_SIZE
            atlas.paste(tex,(cell_x * CELL_SIZE, cell_y * CELL_SIZE))
            print(x)
        except:
            pass

atlas.save('atlas.png')
