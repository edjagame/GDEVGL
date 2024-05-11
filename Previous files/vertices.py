def extract(file_path):
    vertices = []
    indices = []
    normals = []
    tex_coords = []
    tex_indices = []
    normal_indices = []
    with open(file_path, 'r') as obj_file:
        for line in obj_file:
            if line.startswith('v '):
                vertex = [float(i)/3 for i in line.strip().split()[1:]]
                vertices.append(vertex)
            
            if line.startswith('vn '):
                normal = [float(i) for i in line.strip().split()[2:]]
                normals.append(normal)

            if line.startswith('vt '):
                tex_coord = [float(i) for i in line.strip().split()[2:]]
                tex_coords.append(tex_coords)
                
            elif line.startswith('f '):
                index = [int(x.split('/')[0]) - 1 for x in line.split()[1:]]  # Adjusted since indexes start at 0
                indices.append(index)
                tex_index = [int(x.split('/')[1]) - 1 for x in line.split()[1:]]  # Adjusted since indexes start at 0
                tex_indices.append(tex_index)
                normal_index = [int(x.split('/')[2]) - 1 for x in line.split()[1:]]  # Adjusted since indexes start at 0
                tex_indices.append(tex_index)
    return vertices, indices, tex_coords, tex_indices, normals, normal_indices

file_path = 'amogus.obj'  # Replace 'your_file_path.obj' with the path to your .obj file
rgb = "1.00"
texture = "0.0"
format = "f,\t"
vertices, indices, tex_coords, tex_indices, normals, normal_indices = extract(file_path)



#generates two text files containing the vertices and indices in a "neat" manner
# with open("vertices.txt", "w") as vertexFile:
#     for i, vertices in enumerate(vertices):

#         x = f'{float(vertices[0]):.6f}'
#         y = f'{(float(vertices[1])):.6f}'
#         z = f'{(float(vertices[2])):.6f}'
#         vertexFile.write(str(x) + format)
#         vertexFile.write(str(y) + format)
#         vertexFile.write(str(z) + format + "\t")

#         vertexFile.write(rgb + format)
#         vertexFile.write(rgb + format)
#         vertexFile.write(rgb + format+ "\t")

#         vertexFile.write(texture + format)
#         vertexFile.write(texture + format)

#         vertexFile.write(f" //{i}\n")

# with open("indices.txt", "w") as IndexFile:
#     for i, index in enumerate(indices):
#         IndexFile.write(f"{index[0]:3}" + ", ")
#         IndexFile.write(f"{index[1]:3}" + ", ")
#         IndexFile.write(f"{index[2]:3}" + f", //{i}\n")

#generates two text files containing the vertices and indices in a "neat" manner
# with open("vertices2.txt", "w") as vertexFile:
#     for i, index in enumerate(indices):
#         vertexFile.write(f"//Triangle #{i}\n")

#         A = vertices[index[0]]
#         B = vertices[index[1]]
#         C = vertices[index[2]]

#         normal = list(np.cross(np.subtract(B,A),np.subtract(C,A)))

#         for i in index:
#             x = f'{float(vertices[i][0]):.6f}'
#             y = f'{(float(vertices[i][1])):.6f}'
#             z = f'{(float(vertices[i][2])):.6f}'

#             vertexFile.write(str(x) + format)
#             vertexFile.write(str(y) + format)
#             vertexFile.write(str(z) + format + "\t")
        
#             vertexFile.write(rgb[0] + format)
#             vertexFile.write(rgb[1] + format)
#             vertexFile.write(rgb[2] + format+ "\t")

#             vertexFile.write(f"{normal[0]:.6f}" + format)
#             vertexFile.write(f"{normal[1]:.6f}" + format)
#             vertexFile.write(f"{normal[2]:.6f}" + format + "\t")

#             vertexFile.write(texture + format)
#             vertexFile.write(texture + format)
#             vertexFile.write(f"\n")

#         vertexFile.write(f"\n")

with open("vertices2.txt", "w") as vertexFile:
    for i, index in enumerate(indices):
        vertexFile.write(f"//Triangle #{i}\n")

        for i in index:
            x = f'{float(vertices[i][0]):.6f}'
            y = f'{(float(vertices[i][1])):.6f}'
            z = f'{(float(vertices[i][2])):.6f}'

            vertexFile.write(str(x) + format)
            vertexFile.write(str(y) + format)
            vertexFile.write(str(z) + format + "\t")
        
            vertexFile.write(rgb[0] + format)
            vertexFile.write(rgb[1] + format)
            vertexFile.write(rgb[2] + format+ "\t")

            # 
            vertexFile.write(f"{normal[0]:.6f}" + format)
            vertexFile.write(f"{normal[1]:.6f}" + format)
            vertexFile.write(f"{normal[2]:.6f}" + format + "\t")

            vertexFile.write(texture + format)
            vertexFile.write(texture + format)
            vertexFile.write(f"\n")

        vertexFile.write(f"\n")