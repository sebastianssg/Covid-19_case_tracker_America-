import matplotlib.pyplot as plt
import numpy as np


rng = np.random.RandomState(0)

col_img = plt.imread("Colombia.png")
plt.figure(figsize=(20, 10))


"""" LISTAS CON VALORES DETERMINADOS """
# x = [500,257,368,117,212,167,290,693,540,101,512,379,600]
# y = [490,560,807,380,911,860,103,587,640,482,757,851,806]


""" 1000 numeros aleatorios"""
x = np.random.normal(300, 100, 100)
y = np.random.normal(400, 200, 100)

########################## OPCION 1 ################################
colors = rng.rand(100)
sizes = 1000 * rng.rand(100)
plt.scatter(x, y, c=colors, s=sizes, alpha=0.3, cmap='viridis')
plt.colorbar()
plt.imshow(col_img)


########################## OPCION 2 ################################
#plt.scatter(x, y)
#plt.imshow(col_img)
