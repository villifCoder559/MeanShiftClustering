import numpy as np
import pandas as pd
import random
from sklearn.cluster import MeanShift
from sklearn.datasets import make_blobs
from datetime import datetime

nr_clusters=15
nr_dim=4
clusters = np.zeros((nr_clusters,nr_dim))
for i in range(nr_clusters):
  point=np.zeros(nr_dim)
  for j in range(nr_dim):
    point[j]=random.randint(-100,100)
  clusters[i]=point
print(clusters)
X, _ = make_blobs(n_samples = 100000, centers = clusters,
                                   cluster_std = 0.60)

ms = MeanShift(bandwidth=5)
start = datetime.now()
ms.fit(X)
end = datetime.now()
print("TIME "+str((end-start)))
f = open("points.txt", "w") 
print(str(X[0].size)+",",file=f)
for element in X:
  line=""
  for value in element:
    line+=str(value)+","
  if line!="":
    print(line, file=f)
f.close()
