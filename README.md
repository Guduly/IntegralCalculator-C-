# IntegralCalculator-C-

This project implements a generic Binary Search Tree to parse, store, and integrate polynomial expressions (and optional trigonometric expressions for extra credit). The program reads integrals from a file, constructs a BST of terms, computes anti-derivatives, and outputs results in correct mathematical format. Supports both Definite & Indefinite Integration!

## Features

* Parse polynomial terms with integer coefficients/exponents

* Store terms in a generic BST (Node + BinTree classes)

* Combine like terms automatically

* Recursively traverse to generate anti-derivatives

* Support indefinite and definite integrals

* Simplify fractions and format output cleanly

### Trigonometric Integration: 

* Supports sin(kx) and cos(kx):

* Stored alongside polynomial terms

* Preserves original encounter order

### Implements:

∫ sin(kx) dx → –(1/k)cos(kx)

∫ cos(kx) dx → (1/k)sin(kx)

Output formats match assignment requirements

## Input / Output

Input: one integral per line (e.g., | 3x^2 + 2x dx, 1|4 cos 4x dx)

Output: anti-derivative in descending exponent order, with + C or definite value

Files
Main.cpp
Term.h / Term.cpp
Node.h
BinTree.h

Workflow

* Prompt for filename

* Parse terms

* Insert into BST

* Build anti-derivative

* evaluate definite integrals

Print result
