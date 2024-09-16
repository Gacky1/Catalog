# Catalog hackathon

Secret Sharing Scheme 

Introduction
This project demonstrates how to solve a secret sharing problem using Lagrange Interpolation. The code accepts JSON input, parses it, and performs polynomial interpolation based on the number of points (k). It then recovers a secret (constant term) based on these points.

Core Features:
Input is provided in JSON format, with values in different numerical bases (binary, decimal, hexadecimal, etc.).
The Lagrange Interpolation algorithm is used to compute the secret based on a subset of the provided points.


Step-1:- Clone the repository

Step-2:- Open the folder by writing this "cd Catalog"

Step-3: Open terminal and write this g++ shamir.cpp -o shamir

Step-4:- Run the file by writing ./shamir
