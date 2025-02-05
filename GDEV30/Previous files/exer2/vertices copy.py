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

def tex_coords(axis: str, scale: float, i: int) -> float:
    if axis == 'x':
        return math.cos(PI/(r/2.0)*i)/2.0*scale+0.5
    return math.sin(PI/(r/2.0)*i)/2.0*scale+0.5


    
         

################################ Ring 1 ################################

vertices += "//Ring 1\n"
indices += "//Ring 1\n"

vertices += f"{0:7.4f}, {0:7.4f}, {constants}, {0.5:7.4f}, {0.5:7.4f}, {0:7.4f}, {0:7.4f}, //0\n"

for i in range(r):
    vertices+=f"{pointX(i):7.4f}, {pointY(i):7.4f}, {constants}, {tex_coords('x', 1/3, i):7.4f}, {tex_coords('y', 1/3, i):7.4f}, {pointX(i):7.4f}, {pointY(i):7.4f}, //{i+1}\n"
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
    x = pointX(i)
    y = pointY(i)
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 1/3, i):7.4f}, {tex_coords('y', 1/3, i):7.4f}, {x:7.4f}, {y+0.1:7.4f}, //{index+i}\n"

vertices += "\n//1st Half Outer\n"
for i in range(int(r/2)+1):
    x = pointX(i)*2.0
    y = pointY(i)*2.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 2/3, i):7.4f}, {tex_coords('y', 2/3, i):7.4f}, {x:7.4f}, {y+0.1:7.4f}, //{index+int(r/2)+1+i}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

vertices += "\n//2nd Half Inner\n"
for i in range(int(r/2)+1):
    x = pointX(i+int(r/2))
    y = pointY(i+int(r/2))
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 1/3, i+int(r/2)):7.4f}, {tex_coords('y', 1/3, i+int(r/2)):7.4f}, {x:7.4f}, {y-0.1:7.4f}, //{index+i}\n"

vertices += "\n//2nd Half Outer\n"
for i in range(int(r/2)+1):
    x = pointX(i+int(r/2))*2.0
    y = pointY(i+int(r/2))*2.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(r/2)):7.4f}, {tex_coords('y', 2/3, i+int(r/2)):7.4f}, {x:7.4f}, {y-0.1:7.4f}, //{index+int(r/2)+1+i}\n"

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
    x=pointX(i+int(r/4))*2.0
    y=pointY(i+int(r/4))*2.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(r/4)):7.4f}, {tex_coords('y', 2/3, i+int(r/4)):7.4f}, {x-0.4:7.4f}, {y:7.4f}, //{index+i}\n"

vertices += "\n//1st Half Outer\n"
for i in range(int(r/2)+1):
    x=pointX(i+int(r/4))*3.0
    y=pointY(i+int(r/4))*3.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 1, i+int(r/4)):7.4f}, {tex_coords('y', 1, i+int(r/4)):7.4f}, {x-0.4:7.4f}, {y:7.4f}, //{index+int(r/2)+1+i}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

vertices += "\n//2nd Half Inner\n"
for i in range(int(r/4)):
    x=pointX(i+int(3*r/4))*2.0
    y=pointY(i+int(3*r/4))*2.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 2/3, i+int(3*r/4)):7.4f}, {tex_coords('y', 2/3, i+int(3*r/4)):7.4f}, {x+0.4:7.4f}, {y:7.4f}, //{index+i}\n"
for i in range(int(r/4)+1):
    x=pointX(i)*2.0
    y=pointY(i)*2.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 2/3, i):7.4f}, {tex_coords('y', 2/3, i):7.4f}, {x+0.4:7.4f}, {y:7.4f}, //{index+int(r/4)+i}\n"

vertices += "\n//2nd Half Outer\n"
for i in range(int(r/4)):
    x=pointX(i+int(3*r/4))*3.0
    y=pointY(i+int(3*r/4))*3.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 1, i+int(3*r/4)):7.4f}, {tex_coords('y', 1, i+int(3*r/4)):7.4f}, {x+0.4:7.4f}, {y:7.4f}, //{index+int(2*r/4)+i+1}\n"
for i in range(int(r/4)+1):
    x=pointX(i)*3.0
    y=pointY(i)*3.0
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {tex_coords('x', 1, i):7.4f}, {tex_coords('y', 1, i):7.4f}, {x+0.4:7.4f}, {y:7.4f}, //{index+int(3*r/4)+i+1}\n"

indices += "\n//1st Half\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+int(r/2)},{index+i+int(r/2)+1},\n"
for i in range(int(r/2)+1):
    indices+=f"{index+i},{index+i+1},{index+i+int(r/2)+1},\n"
index += 34

################################ Corner ################################

vertices += "\n//----------------Corner----------------//\n"
indices += "\n//----------------Corner----------------//\n"

extra = 0
for i in range(r):
    
    x = pointX(i)*3.0
    y = pointY(i)*3.0
    dispX = 0.3 if x > 0 else -0.3
    dispY = 0.3 if y > 0 else -0.3
    
    if i%8==0:
        vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {math.cos(PI/(r/2.0)*i)/2.0+0.5:7.4f}, {math.sin(PI/(r/2)*i)/2.0+0.5:7.4f}, {x+dispX:7.4f}, {y+dispY:7.4f}, //{index+i+extra}\n"
        extra += 1

    if i==0:
        dispY = -dispY
    if i==8:
        dispX = -dispX
    if i==16:
        dispY = -dispY
    if i==24:
        dispX = -dispX
        
    vertices+=f"{x:7.4f}, {y:7.4f}, {constants}, {math.cos(PI/(r/2.0)*i)/2.0+0.5:7.4f}, {math.sin(PI/(r/2)*i)/2.0+0.5:7.4f}, {x+dispX:7.4f}, {y+dispY:7.4f}, //{index+i+extra}\n"


vertices+=f"{0.75:7.4f}, {0.75:7.4f}, {constants}, {1:7.4f}, {1:7.4f}, {0.75+0.3:7.4f}, {0.75+0.3:7.4f}, //{index+r+4}\n"
vertices+=f"{-0.75:7.4f}, {0.75:7.4f}, {constants}, {0:7.4f}, {1:7.4f}, {-0.75-0.3:7.4f}, {0.75+0.3:7.4f}, //{index+r+5}\n"
vertices+=f"{-0.75:7.4f}, {-0.75:7.4f}, {constants}, {0:7.4f}, {0:7.4f}, {-0.75-0.3:7.4f}, {-0.75-0.3:7.4f}, //{index+r+6}\n"
vertices+=f"{0.75:7.4f}, {-0.75:7.4f}, {constants}, {1:7.4f}, {0:7.4f}, {0.75+0.3:7.4f}, {-0.75-0.3:7.4f}, //{index+r+7}\n"


for i in range(int(r/4)):
    indices += f"{index+r+4},{index+i+1},{index+i+2},\n"

for i in range(int(r/4)):
    indices += f"{index+r+5},{index+i+int((r/4))+2},{index+i+3+int((r/4))},\n"

for i in range(int(r/4)):
    indices += f"{index+r+6},{index+i+int(2*r/4)+3},{index+i+4+int(2*r/4)},\n"

for i in range(int(r/4)-1):
    indices += f"{index+r+7},{index+i+int(3*r/4)+4},{index+i+5+int(3*r/4)},\n"
indices += f"{index+r+7},{index+int(r/4)-1-1+1+int(3*r/4)+4},{index},\n"


################################ Write ################################


with open('vertices.txt', 'w') as f:
    f.write(vertices)
    f.close()
with open('indices.txt', 'w') as f:
    f.write(indices)
    f.close()
