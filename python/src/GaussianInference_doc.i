%feature("docstring") OTAGRUM::GaussianBayesianNetwork
"GaussianInference class implements the elimination variable inference
algorihtm for Gaussian Bayesian networks (GBNs),


Available constructor:
    GaussianBayesianNetwork(*gbn)

Parameters
----------
gbn : :class:`~otagrum.GaussianBayesianNetwork`
    The GBN used for inference

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
