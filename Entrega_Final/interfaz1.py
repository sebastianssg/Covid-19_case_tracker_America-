import matplotlib.pyplot as plt
import numpy as np

dic = {5:0,8:0,11:0,13:0,15:0,17:0,18:0,19:0,20:0,23:0,25:0,27:0,41:0,44:0,47:0,50:0,52:0,54:0,63:0,66:0,68:0,70:0,73:0,76:0,81:0,85:0,86:0,88:0,91:0,94:0,95:0,97:0,99:0}
def obtenerInfo():
    f = open("vis.txt","r")

    contents = f.readlines()
    
    for line in contents:
        line = line.replace("\n", "")
        if int(line) in dic.keys():
            dic[int(line)] += 1


obtenerInfo()

rng = np.random.RandomState(0)

col_img = plt.imread("Colombia.png") #Se carga el mapa politico de Colombia
plt.figure(figsize=(20, 10))


"""" LISTAS CON VALORES DETERMINADOS """
x = [247,272,323,314,380,248,304,143,346,230,312,160,236,391,309,367,93,399,228,206,368,266,252,172,507,476,236,121,474,634,439,527,592]
y = [354,118,480,259,422,434,693,596,196,258,451,424,594,90,149,551,643,274,484,440,358,218,523,537,358,435,725,88,833,586,658,702,469]


listaSizes = []
for a in dic.values():
    listaSizes.append(a)
colors = listaSizes;

sizes = listaSizes
plt.scatter(x, y, c=colors, s=sizes, alpha=0.3, cmap='viridis')
plt.colorbar()
plt.imshow(col_img)
plt.show()


