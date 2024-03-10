import math

PI = math.pi
ASPECT_RATIO = 1

vertices  = f""
indices   = f""

r = 32
index = 0
constants = "0.000f, 1.0f, 1.0f, 1.0f"
def pointX(i) -> float:
    return math.cos(PI/(r/2)*i)/ASPECT_RATIO/4.0

def pointY(i) -> float:
    return math.sin(PI/(r/2)*i)/4.0

def scale(point) -> float:
    return (point + 0.75) * 2/3

def tex_coords(axis: str, scale: float, i: int):
    if axis == 'x':
        return math.cos(PI/(r/2.0)*i)/2.0*scale+0.5
    return math.sin(PI/(r/2.0)*i)/2.0*scale+0.5


################################ Ring 1 ################################

vertices += "//Ring 1\n"
indices += "//Ring 1\n"

vertices += f"{0:7.4f}, {0:7.4f}, {constants}, {0.5:7.4f}, {0.5:7.4f}, //0\n"

for i in range(r):
    vertices+=f"{pointX(i):7.4f}, {pointY(i):7.4f}, {constants}, {tex_coords('x', 1/3, i):7.4f}, {tex_coords('y', 1/3, i):7.4f}, //{i+1}\n"
    if i!=r-1:
        indices +=f"{0}, {i+1}, {i+2},\n"
    else:
        indices +=f"{0}, {r}, 1,\n"

index += 33

################################ Ring 2 ################################

vertices += "\n//----------------Ring 2----------------//\n"
indices += "\n//----------------Ring 2----------------//\n"


vertices += "\n//1st Half Inner\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i):7.4f}, {pointY(i):7.4f}, {constants}, {tex_coords('x', 1/3, i):7.4f}, {tex_coords('y', 1/3, i):7.4f}, //{index+i}\n"

vertices += "\n//1st Half Outer\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i)*2.0:7.4f}, {pointY(i)*2.0:7.4f}, {constants}, {tex_coords('x', 2/3, i):7.4f}, {tex_coords('y', 2/3, i):7.4f}, //{index+int(r/2)+1+i}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

vertices += "\n//2nd Half Inner\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i+int(r/2)):7.4f}, {pointY(i+int(r/2)):7.4f}, {constants}, {tex_coords('x', 1/3, i+int(r/2)):7.4f}, {tex_coords('y', 1/3, i+int(r/2)):7.4f}, //{index+i}\n"

vertices += "\n//2nd Half Outer\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i+int(r/2))*2.0:7.4f}, {pointY(i+int(r/2))*2.0:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(r/2)):7.4f}, {tex_coords('y', 2/3, i+int(r/2)):7.4f}, //{index+int(r/2)+1+i}\n"

indices += "\n//2nd Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

################################ Ring 3 ################################

vertices += "\n//----------------Ring 3----------------//\n"
indices += "\n//----------------Ring 3----------------//\n"

vertices += "\n//1st Half Inner\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i+int(r/4))*2.0:7.4f}, {pointY(i+int(r/4))*2.0:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(r/4)):7.4f}, {tex_coords('y', 2/3, i+int(r/4)):7.4f}, //{index+i}\n"

vertices += "\n//1st Half Outer\n"
for i in range(int(r/2)+1):
    vertices+=f"{pointX(i+int(r/4))*3.0:7.4f}, {pointY(i+int(r/4))*3.0:7.4f}, {constants}, {tex_coords('x', 1, i+int(r/4)):7.4f}, {tex_coords('y', 1, i+int(r/4)):7.4f}, //{index+int(r/2)+1+i}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

vertices += "\n//2nd Half Inner\n"
for i in range(int(r/4)):
    vertices+=f"{pointX(i+int(3*r/4))*2.0:7.4f}, {pointY(i+int(3*r/4))*2.0:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(3*r/4)):7.4f}, {tex_coords('y', 2/3, i+int(3*r/4)):7.4f}, //{index+i}\n"
for i in range(int(r/4)+1):
    vertices+=f"{pointX(i)*2.0:7.4f}, {pointY(i)*2.0:7.4f}, {constants}, {tex_coords('x', 2/3, i):7.4f}, {tex_coords('y', 2/3, i):7.4f}, //{index+int(r/4)+i}\n"

vertices += "\n//2nd Half Outer\n"
for i in range(int(r/4)):
    vertices+=f"{pointX(i+int(3*r/4))*3.0:7.4f}, {pointY(i+int(3*r/4))*3.0:7.4f}, {constants}, {tex_coords('x', 1, i+int(3*r/4)):7.4f}, {tex_coords('y', 1, i+int(3*r/4)):7.4f}, //{index+int(2*r/4)+i+1}\n"
for i in range(int(r/4)+1):
    vertices+=f"{pointX(i)*3.0:7.4f}, {pointY(i)*3.0:7.4f}, {constants}, {tex_coords('x', 1, i):7.4f}, {tex_coords('y', 1, i):7.4f}, //{index+int(3*r/4)+i+1}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

################################ Corner ################################

vertices += "\n//----------------Corner----------------//\n"
indices += "\n//----------------Corner----------------//\n"

for i in range(r):
    vertices+=f"{pointX(i)*3.0:7.4f}, {pointY(i)*3.0:7.4f}, {constants}, {math.cos(PI/(r/2.0)*i)/2.0+0.5:7.4f}, {math.sin(PI/(r/2)*i)/2.0+0.5:7.4f}, //{index+i}\n"


vertices+=f"{0.75:7.4f}, {0.75:7.4f}, {constants}, {1:7.4f}, {1:7.4f}, //{index+r}\n"
vertices+=f"{-0.75:7.4f}, {0.75:7.4f}, {constants}, {0:7.4f}, {1:7.4f}, //{index+r+1}\n"
vertices+=f"{-0.75:7.4f}, {-0.75:7.4f}, {constants}, {0:7.4f}, {0:7.4f}, //{index+r+2}\n"
vertices+=f"{0.75:7.4f}, {-0.75:7.4f}, {constants}, {1:7.4f}, {0:7.4f}, //{index+r+3}\n"


for i in range(int(r/4)):
    indices += f"{index+r},{index+i},{index+i+1},\n"

for i in range(int(r/4)):
    indices += f"{index+r+1},{index+i+int((r/4))},{index+i+1+int((r/4))},\n"

for i in range(int(r/4)):
    indices += f"{index+r+2},{index+i+int(2*r/4)},{index+i+1+int(2*r/4)},\n"

for i in range(int(r/4)-1):
    indices += f"{index+r+3},{index+i+int(3*r/4)},{index+i+1+int(3*r/4)},\n"
indices += f"{index+r+3},{index+int(r/4)-1-1+1+int(3*r/4)},{index},\n"


################################ Write ################################


with open('vertices.txt', 'w') as f:
    f.write(vertices)
    f.close()
with open('indices.txt', 'w') as f:
    f.write(indices)
    f.close()
