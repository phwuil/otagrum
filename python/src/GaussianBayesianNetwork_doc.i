%feature("docstring") OTAGRUM::GaussianBayesianNetwork
"GaussianBayesianNetwork class represents a Gaussian bayesian network (GBN),
that is a bayesian network parameterized by conditional linear Gaussian (CLG)
for each node:
    .. math::
            f(x_i|\text{Pa}_i) = N(x_i; \mu_i + \sum_{j=1}^{|\text{Pa}_i|} x_j, \sigma_i)


Available constructor:
    GaussianBayesianNetwork(*dag)

Parameters
----------
dag : :class:`~otagrum.NamedDAG`
    The structure of the CBN

Examples
--------
Create a GBN from a DAG structure and a collection of distributions

>>> import otagrum
>>> import openturns as ot
>>> import pyAgrum as gum
>>> bn = gum.BayesNet.fastPrototype('A->C->B;')
>>> ndag = otagrum.NamedDAG(bn)
>>> marginals = [ot.Uniform(0.0, 1.0) for i in range(ndag.getSize())]
>>> copulas = []
>>> for i in range(ndag.getSize()):
...     dim = 1 + ndag.getParents(i).getSize()
...     R = ot.CorrelationMatrix(dim)
...     for j in range(dim):
...         for k in range(j):
...             R[j, k] = 0.8
...     copulas.append(ot.Normal([0.0]*dim, [1.0]*dim, R).getCopula())
>>> cbn = otagrum.ContinuousBayesianNetwork(ndag, marginals, copulas)

Draw a sample from the joint distribution encoded by the CBN

>>> sample = cbn.getSample(100)"

// ----------------------------------------------------------------------------
