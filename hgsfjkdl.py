def load_obj_file(file_path):
    vertices = []
    faces = []
    
    with open(file_path, 'r') as obj_file:
        for line in obj_file:
            if line.startswith('v '):
                vertices.append([float(x) for x in line.split()[1:]])
            elif line.startswith('f '):
                face_vertices = [int(x.split('/')[0]) - 1 for x in line.split()[1:]]
                faces.append(face_vertices)
    
    return vertices, faces

def convert_to_vertex_arrays(vertices, faces):
    vertex_array = []
    index_array = []
    
    for face in faces:
        for vertex_index in face:
            vertex_array.extend(vertices[vertex_index])
            index_array.append(len(index_array))
    
    return vertex_array, index_array

# Example usage
obj_file_path = 'water.obj'
vertices, faces = load_obj_file(obj_file_path)
vertex_array, index_array = convert_to_vertex_arrays(vertices, faces)

print("Vertex Array:")
print(vertex_array)
print("Index Array:")
print(index_array)
