# s = ""
# with open("vertices.txt") as file:
#     for line in file:
#         newline = line.split(',')
#         newline.pop()
#         newline = [str(f"{float(i.lstrip(' '))/3:7.4f}") for i in newline]
#         newline += ['1.0f','1.0f','1.0f']
#         s += ", ".join(newline) + '\n'

# with open("verticesNew.txt", "w") as file:
#     file.write(s)
################################################################
# s = ""
# with open("verticesNew.txt") as file:
#     for line in file:
#         newline = line.split(',')
#         newline.pop()
#         newline = [
#             float(i.lstrip(' ')) if 'f' not in i else i for i in newline]
#         newline[1]-=0.5
#         newline = [f"{i:7.4f}" if isinstance(i, float) else i for i in newline]
#         s += ", ".join(newline) + '\n'

# with open("verticesNew.txt", "w") as file:
#     file.write(s)
################################################################
s = ""
with open("verticesNew.txt") as file:
    for line in file:
        newline = line[:-1] + ",  1.0f,\n"
        s += newline

with open("verticesNew.txt", "w") as file:
    file.write(s)