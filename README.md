# Mean Shift Clustering: A Comparison of Parallel and Sequential Implementations
This project compares the performance of a parallel and a sequential implementation of the mean shift clustering algorithm, in order to determine the best approach for a given dataset and computational resources. 
The project is implemented in C++, and uses the sklearn library to test the correctness of the parallel mean-shift clustering algorithm. 
The parallel implementation is done using OpenMP, a widely used application programming interface for shared-memory parallelism.<br/>
You can read a detailed pdf **project report** inside the folder "project_report".

### Prerequisites
- C++ compiler that supports OpenMP
 
### Results
The results of the tests show that the parallel implementation, using OpenMP, is significantly faster than the sequential implementation,
especially when the dataset is large and the number of clusters is high. 

### Conclusion
This project demonstrates that parallelization can be a powerful tool for speeding up the mean shift clustering algorithm.
By using OpenMP, we were able to achieve significant speedup without compromising the accuracy of the clustering results.
This makes it a suitable option for big data analysis.
