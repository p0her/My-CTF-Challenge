from secret import FLAG

p = 251
F = GF(p)

R.<x> = PolynomialRing(F)
degree = 128

Px = R.irreducible_element(degree, algorithm="random")
Qx = R.irreducible_element(degree, algorithm="random")

Nx = Px * Qx

FLAG_poly = sum(ord(FLAG[i]) * x^i for i in range(len(FLAG)))
Cx = pow(FLAG_poly, 0x10001, Nx)

print(Cx)
print(Nx)
