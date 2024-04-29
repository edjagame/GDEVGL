class FaceIndex:
    def __init__(self, pos: int, tex: int, norm: int):
        self.pos = pos 
        self.tex = tex
        self.norm = norm

vertices = []
normals = []
tex_coords = []
indices = []
with open('amogus.obj', 'r') as obj_file:
    for line in obj_file:
        if line.startswith('v '):
            vertex = [float(i)/3 for i in line.strip().split()[1:]]
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

file_path = 'amogus.obj'  # Replace 'your_file_path.obj' with the path to your .obj file
rgb = ["1.00", "1.00", "1.00"]
format = "f,\t"

with open("vertices3.txt", "w") as vertexFile:
    #indices[] contains N Triangles
    #Each Triangle has 3 FaceIndex Objects (indices[0]: FaceIndex, indices[1], indices[2])
    for triangleNum, triangle in enumerate(indices):
        vertexFile.write(f"//Triangle #{triangleNum}\n")

        # #specific to amogus
        # if triangleNum <= 43:
        #     rgb = ["1.00", "0.00", "0.00"]
        # if triangleNum >= 44 and triangleNum <= 55:
        #     rgb = ["0.70", "0.20", "0.20"]
        # else:
        #     rgb = ["0.00", "1.00", "1.00"]

        for t in triangle:
            pos = vertices[t.pos]
            norm = normals[t.norm]
            tex = tex_coords[t.tex]
            
            #x, y, z
            for i in pos:
                vertexFile.write(f"{i:.6f}" + format)
            vertexFile.write("\t")

            vertexFile.write(rgb[0] + format)
            vertexFile.write(rgb[1] + format)
            vertexFile.write(rgb[2] + format+ "\t")

            for i in norm:
                vertexFile.write(f"{i:.6f}" + format)
            vertexFile.write(f"\t")

            for i in tex:
                vertexFile.write(f"{i:.6f}" + format)
            vertexFile.write(f"\t")


            # #specific to amogus
            # if triangleNum >= 56:
            #     vertexFile.write(f"1,\t")
            # else:
            #     vertexFile.write(f"0,\t")

            
            vertexFile.write(f"\n")
        vertexFile.write(f"\n")