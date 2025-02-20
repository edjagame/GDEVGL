
floats = []
with open("test.txt", "r") as f:
    for line in f:
        floats.append([i.strip().replace("f","") for i in line.split(",")])


for i in floats:
    for j in i:
        if j == "":
            i.remove(j)
        else:
            i[i.index(j)] = float(j)

pos = []
norm = []
tang = []
tex = []
for i in floats:
    pos.append(i[0:3])
    norm.append(i[3:6])
    tang.append(i[6:9])
    tex.append(i[9:11])

for i in range(len(pos)):
    print(f"pos: {pos[i]}")
    print(f"norm: {norm[i]}")
    print(f"tang: {tang[i]}")
    print(f"tex: {tex[i]}")

class Triangle:
    def __init__(self, pos: list, norm: list, tang: list, tex: list):
        self.pos = pos
        self.norm = norm
        self.tang = tang
        self.tex = tex

triangles = []
for i in range(int(len(pos)/3)):
    v1 = 3*i
    v2 = 3*i+1
    v3 = 3*i+2
    triangles.append(Triangle(pos[v1:v1+3], norm[v1:v1+3], tang[v1:v1+3], tex[v1:v1+3]))


def calculate_tangent(triangle):
    edge1 = [triangle.pos[1][0] - triangle.pos[0][0], triangle.pos[1][1] - triangle.pos[0][1], triangle.pos[1][2] - triangle.pos[0][2]]
    edge2 = [triangle.pos[2][0] - triangle.pos[0][0], triangle.pos[2][1] - triangle.pos[0][1], triangle.pos[2][2] - triangle.pos[0][2]]
    deltaU1 = triangle.tex[1][0] - triangle.tex[0][0]
    deltaV1 = triangle.tex[1][1] - triangle.tex[0][1]
    deltaU2 = triangle.tex[2][0] - triangle.tex[0][0]
    deltaV2 = triangle.tex[2][1] - triangle.tex[0][1]
    f = 1.0 / (deltaU1 * deltaV2 - deltaU2 * deltaV1)
    tangent = [f * (deltaV2 * edge1[0] - deltaV1 * edge2[0]), f * (deltaV2 * edge1[1] - deltaV1 * edge2[1]), f * (deltaV2 * edge1[2] - deltaV1 * edge2[2])]
    return tangent

for triangle in triangles:
    print(f"tangent: {calculate_tangent(triangle)} or {triangle.tang}")
    
