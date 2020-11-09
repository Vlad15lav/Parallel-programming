# The task of parallel programming.
## Tasks
1. Multiplication of polynomials.
2. Binary merge.
3. Doubling.
4. Block multiplication of a matrix by a vector.
5. Odd even marger sort.
6. Block sorting.
7. Dining Philosophers.
8. Barbershop Problem.

### Multiplication of polynomials
Multiplication of two polynomials using the formula for calculating the k-th coefficient.</br>
![](/Polynomials/formula.png)
### Binary merge
Using a pass of array A relative to B and array B relative to A.</br>
![](/BinaryMerge/formula.png)
### Doubling
![](/Doubling/Cascade.jpg)
### Block multiplication of a matrix by a vector
To deal with the race condition when adding up to the final vector, a synchronization mechanism is used. Mutex.</br>
![](/BlockMultiplication/Scheme.png)
### Odd even marger sort
![](/OddEvenMergesort/Mergers.png)
### Block sorting
![](/BlockSort/BlockMergers.png)
### Dining philosophers
Mutex - fork. Semaphore - number of chairs.</br>
![](/DiningPhilosophers/Table.png)
### Barbershop problem
Client-server model. Two tables - two data structures. Interaction with the table - mutex. Waiting for a client or Barber to act - semaphore.</br>
![](/BarbershopProblem/Barbershop.png)</br>
![](/BarbershopProblem/Model.png)
