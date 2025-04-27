import numpy as np

class FaceIndex:
    def __init__(self, pos: int, tex: int, norm: int):
        self.pos = pos 
        self.tex = tex
        self.norm = norm

vertices = []
normals = []
tex_coords = []
indices = [] 
model = "Mirror"
inputFile = f"{model}Object.obj"
outputFile = f"{model}Verts.txt"

with open(inputFile, 'r') as obj_file:
    for line in obj_file:
        if line.startswith('v '):
            vertex = [float(i) for i in line.strip().split()[1:]]
            vertices.append(vertex)
        
        if line.startswith('vn '):
            normal = [float(i) for i in line.strip().split()[1:]]
            normals.append(normal)

        if line.startswith('vt '):
            tex_coord = [float(i) for i in line.strip().split()[1:]]
            tex_coords.append(tex_coord)
            
        elif line.startswith('f '):
            triangle = []
            for x in line.split()[1:]:
                facePos = int(x.split('/')[0]) - 1 
                faceTex = int(x.split('/')[1]) - 1 
                faceNorm = int(x.split('/')[2]) - 1 
                triangle.append(FaceIndex(facePos, faceTex, faceNorm))
            indices.append(triangle)


vertex_smooth = {i:[] for i in range(len(vertices))}
for triangleNum, triangle in enumerate(indices):
    for t in triangle:
        if False:
            vertex_smooth[t.pos].append(normals[t.norm])

smooth_normal = {}
for k,v in vertex_smooth.items():
    x = []
    for sublist in v:
        if sublist not in x:
            x.append(sublist)
    v = x
    if v:
        length = len(v)
        x = 0
        y = 0
        z = 0
        for vec in v:
            x += vec[0]
            y += vec[1]
            z += vec[2]
        smooth_normal[k] = [x/length, y/length, z/length]

rgba = ["1.0", "1.0", "1.0", "1.0"]
format = "\n"
numTriangles = len(indices) - 1


def calculate_tangent(triangle):
    
    edge1 = np.subtract(vertices[triangle[1].pos], vertices[triangle[0].pos])
    edge2 = np.subtract(vertices[triangle[2].pos], vertices[triangle[0].pos])
    deltaUV1 = np.subtract(tex_coords[triangle[1].tex], tex_coords[triangle[0].tex])
    deltaUV2 = np.subtract(tex_coords[triangle[2].tex], tex_coords[triangle[0].tex])

    f = 1.0 / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1])
    tangent = [0, 0, 0]
    tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0])
    tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1])
    tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2])

    tangent = np.array(tangent)
    tangent = tangent / np.linalg.norm(tangent)
    
    return tangent

with open(outputFile, "w") as vertexFile:
    #indices[] contains N Triangles
    #Each Triangle has 3 FaceIndex Objects (indices[0]: FaceIndex, indices[1], indices[2])
    for triangleNum, triangle in enumerate(indices):

        #Calculate Tangent
        tangent = calculate_tangent(triangle)

        for t in triangle:
            pos = vertices[t.pos]
            if t.pos in smooth_normal:
                norm = smooth_normal[t.pos]
            else:
                norm = normals[t.norm]
            tex = tex_coords[t.tex]
            
            #x, y, z
            for i in pos:
                vertexFile.write(f"{i:.6f}" + format)

            #RGBa
            vertexFile.write(rgba[0] + format)
            vertexFile.write(rgba[1] + format)
            vertexFile.write(rgba[2] + format)
            vertexFile.write(rgba[3] + format)


            #vertex normal coords
            for i in norm:
                vertexFile.write(f"{i:.6f}" + format)

            #tex coords
            for i in tex:
                vertexFile.write(f"{i:.6f}" + format)

            for i in tangent:
                vertexFile.write(f"{i:.6f}" + format)
            # #tex coords
            # vertexFile.write(f"1,")
            # vertexFile.write(f"\t")

            #end of file



        print(f"Triangle {triangleNum} out of {numTriangles}")
    