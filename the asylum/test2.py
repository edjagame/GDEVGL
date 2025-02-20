import numpy as np

"""
-55.547764f,	-5.225585f,	54.202007f,		1.0f,	1.0f,	1.0f,	1.0f,		-0.000000f,	-1.000000f,	-0.000000f,		0.064214f,	0.065665f,		0.000005f,	0.000000f,	-1.000000f,	
-62.848175f,	-5.225585f,	72.094788f,		1.0f,	1.0f,	1.0f,	1.0f,		-0.000000f,	-1.000000f,	-0.000000f,		0.003872f,	0.041045f,		0.000005f,	0.000000f,	-1.000000f,	
-73.870644f,	-5.225585f,	61.072319f,		1.0f,	1.0f,	1.0f,	1.0f,		-0.000000f,	-1.000000f,	-0.000000f,		0.041045f,	0.003872f,		0.000005f,	0.000000f,	-1.000000f,	"""

pos1 = [-55.547764, -5.225585, 54.202007]
pos2 = [-62.848175, -5.225585, 72.094788]
pos3 = [-73.870644, -5.225585, 61.072319]

u1 = 0.064214
v1 = 0.065665
u2 = 0.003872
v2 = 0.041045
u3 = 0.041045
v3 = 0.003872

deltaUV1 = [u2 - u1, v2 - v1]
deltaUV2 = [u3 - u1, v3 - v1]

edge1 = [pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]]
edge2 = [pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]]

f = 1.0 / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1])
tangent = [0, 0, 0]
tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0])
tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1])
tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2])

tangent = np.array(tangent)
tangent = tangent / np.linalg.norm(tangent)

print(tangent)