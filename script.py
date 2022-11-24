import numpy as np
import pandas as pd
import random
from sklearn.cluster import MeanShift
from sklearn.datasets import make_blobs
from datetime import datetime
def create_file_output(name,array):
  nr_elements=array.shape[0]
  count=0
  f = open(str(name), "w") 
  print(str(array[0].size)+",",file=f)
  for element in array:
    line=""
    count+=1
    ["{0:0.2f}".format(i) for i in a]
    for value in element:
      line+=str(value)+","
    if line!="":
      if count==nr_elements:
        print(line,file=f,end='')
      else:
        print(line, file=f)
  f.close()
  print("Created "+str(name)+" file")
def generate_blobs(dim_point,nr_clusters,nr_points,cluster_std=0.6):
  clusters = np.zeros((nr_clusters,dim_point))
  for i in range(nr_clusters):
    point=np.zeros(dim_point)
    for j in range(dim_point):
      point[j]=random.randint(-100,100)
    clusters[i]=point
  return make_blobs(n_samples = nr_points, centers = clusters,cluster_std = cluster_std)


X, _ = generate_blobs(4,10,50_000)
print("Start fitting...")
ms = MeanShift(bandwidth=5)
start = datetime.now()
ms.fit(X)
end = datetime.now()
print("Fitting completed in "+str((end-start))+" ms")
create_file_output("points.txt",X)
create_file_output("centroids.txt",ms.cluster_centers_)





