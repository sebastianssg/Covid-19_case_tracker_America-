import matplotlib.pyplot as plt
import numpy


# Plot
col_img = plt.imread("Colombia.png")
plt.figure(figsize=(20, 10))


x = [500,257,368,117,212,167,290,693,540,101,512,379,600]
y = [490,560,807,380,911,860,103,587,640,482,757,851,806]


"""numeros aleatorios"""
#x = numpy.random.normal(300, 100, 1000)
#y = numpy.random.normal(400, 200, 1000)



plt.scatter(x, y)
plt.imshow(col_img)

