# divided_rectangles
Implementation of divided rectangles algorithm.

Here is a [link](https://www.researchgate.net/publication/226216344_Lipschitzian_Optimisation_Without_the_Lipschitz_Constant) to the original paper.

Jones, D.R., Perttunen, C.D. & Stuckman, B.E. Lipschitzian optimization without the Lipschitz constant. *J Optim Theory Appl* **79,** 157–181 (1993). https://doi.org/10.1007/BF00941892



I'm writing this in C++.  This is my first version and I'm sure there are more efficient ways to implement.  I'm using the most straightforward data structures and algorithms for ease of coding.  Then maybe I'll work on steamlining.



This is the [Brenin](https://www.sfu.ca/~ssurjano/branin.html) function.

![Brenin function points](Figure_1.png)

This is the result for the quadratic form
$$
f(x) = x^T \begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix} x + \begin{bmatrix} 3 \\ 3 \end{bmatrix}^T x + 1
$$
![Result for quadratic function](Figure_2.png)

This is the result for the quadratic form with the nonconvex (and non-Lipschitz) penalty added

$$
f(x) = x^T \begin{bmatrix} 2 & 1 \\ 1 & 2 \end{bmatrix} x + \begin{bmatrix} 3 \\ 3 \end{bmatrix}^T x + 1 - \log(\|x\|^2 - 2)
$$

![This is the result for the penalized function](Figure_3.png)

Even though this was a hack (by adding 1000 outside the domain of the penalty), the result worked by finding constrained minima!
