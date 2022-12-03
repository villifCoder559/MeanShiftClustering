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

n_points=[10000,25000,50000,100000,200000]
n_centroids=[5,10,20,50,100]
n_dims=[2,3,4,5]
default_n_points=50_000
default_n_dims=3
default_n_centroids=10
for element in n_points:
  X, _ = generate_blobs(default_n_dims,default_n_centroids,element)
  create_file_output("points_"+str(element)+".txt",X)
for element in n_centroids:
  X, _ = generate_blobs(default_n_dims,element,default_n_points)
  create_file_output("centroids_"+str(element)+".txt",X)
for element in n_dims:
  X, _ = generate_blobs(element,default_n_dims,default_n_points)
  create_file_output("dims_"+str(element)+".txt",X)

