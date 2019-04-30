## babyprng

You are provided with a pseudorandom number generator with high bias and supposed to extract uniform randomness from the sequence. So just implement [von Neumann extractor](https://en.wikipedia.org/wiki/Bernoulli_process#Basic_von_Neumann_extractor) with the opcodes and duplicate your outputs for the length check.

(Sadly I observed some unintended solutions but the implementation is not easier than intended one. This is a baby challenge anyway :P)
