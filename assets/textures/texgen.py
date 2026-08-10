from PIL import Image
import random
import os

TEX_SIZE = 16

def generate_texture(base_color, ampl, filename, sync=False):
    img = Image.new('RGB', (TEX_SIZE, TEX_SIZE))
    pixels = img.load()
    
    for x in range(TEX_SIZE):
        for y in range(TEX_SIZE):
            if sync:
                noise_r=noise_g=noise_b=random.randint(-ampl,ampl)
            else:
                noise_r = random.randint(-ampl, ampl)
                noise_g = random.randint(-ampl, ampl)
                noise_b = random.randint(-ampl, ampl)
            
            r = max(0, min(255, base_color[0] + noise_r))
            g = max(0, min(255, base_color[1] + noise_g))
            b = max(0, min(255, base_color[2] + noise_b))
            
            pixels[x, y] = (r, g, b)
    
    img.save(filename)
    print(f"Generated {filename}")

generate_texture((80, 180, 40), 20, "1.png")
generate_texture((150, 100, 40), 20, "2.png")
generate_texture((160, 160, 160), 20, "3.png", True)
print("All textures generated!")
