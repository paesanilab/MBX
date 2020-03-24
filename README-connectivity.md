In this README, we will learn how to write a connectivity file

Here is an example of what a connectivity file will look like:
Annotations are given after a #. Do NOT include annotations in
the connectivity file. This is solely for demonstration purposes

h4_dummy                                # Id of the monomer
bond 1 2 harm 2.0 4.0                   # Bond 1 
bond 2 3 harm 2.0 4.0                   # Bond 2
bond 3 4 harm 2.0 4.0                   # Bond 3
angle 1 2 3 harm 1.0 2.0                # Angle 1
angle 2 3 4 harm 3.0 3.14159265	        # Angle 2
dihedral 1 2 3 4 cos 1.0 1.0 0.5        # Dihedral 1
inversion 1 2 3 4 harm 1.0 2.0 	        # Inversion 1
ENDMON                                  # Required flag to signify end of
                                        # molecule reading

Example of two monomer file connectivity file:

h4_dummy                                # Id of the monomer
bond 1 2 harm 2.0 4.0                   # Bond 1 
bond 2 3 harm 2.0 4.0                   # Bond 2
bond 3 4 harm 2.0 4.0                   # Bond 3
angle 1 2 3 harm 1.0 2.0                # Angle 1
angle 2 3 4 harm 3.0 3.14159265	        # Angle 2
dihedral 1 2 3 4 cos 1.0 1.0 0.5        # Dihedral 1
inversion 1 2 3 4 harm 1.0 2.0 	        # Inversion 1
ENDMON
h4_dummy2                               # Id of the monomer
bond 4 2 harm 2.0 4.0                   # Bond 1 
bond 1 5 harm 2.2 3.2                   # Bond 2
bond 6 4 harm 1.6 1.1                   # Bond 3
angle 4 2 3 harm 1.6 2.2                # Angle 1
angle 2 5 4 harm 3.0 4.3                # Angle 2
dihedral 2 4 6 1 hcos 1.0 4.0           # Dihedral 1
inversion 2 3 1 6 harm 1.0 2.0 	        # Inversion 1
ENDMON

(note* the indices are not real, nor is the monomer)

Lets look closer at what each topology and its associated numbers mean

Bond:
bond         1         2          harm        1.0    2.0
^            ^         ^            ^          ^      ^
topology    1st idx  2nd idx     func form     k      r0

Angle:
angle         1         2        3          harm      1.0    2.0
 ^            ^         ^        ^           ^         ^      ^
topology    1st idx  2nd idx   3rd idx    func form    k    theta0

Dihedral:
dihedral      1         2        3       4      cos         1.0   2.0   3.0
^             ^         ^        ^       ^      ^            ^     ^     ^
topology   1st idx  2nd idx   3rd idx  4th idx  func form    A     m     d

Inversion:
inversion     1         2        3       4      harm        1.0   2.0
^             ^         ^        ^       ^       ^           ^     ^
topology   1st idx  2nd idx   3rd idx  4th idx  func form    k    phi0

In the above examples, note that bond will always have "bond" followed
by exactly 2 indices and a functional form. The only parameters that
will vary are the linear and nonlinear parameters. Below you will find
all of the functional forms available to each topology and the order
in which to input them

Bond:
harm      k      r0
morse     E0     k         r0
quartic   k      r0        k'   k''

Angle:
harm      k      theta0
quartic   k      theta0    k'    k''

Dihedral:
cos       A      m         d(delta)
harm      k      phi0
hcos      k      phi0
cos3      A1     A2        A3

Inversion:
harm      k      phi0

Important points to note:
indexes are always 1 based, NOT 0 based. Ie you start counting from
index 1

Units:
r0 is always in angstroms
theta0 is always in radians
phi0 for both dihedral and inversion are always in radians
The inversion potential takes care of all 3 permutations of the inversion
angle. The inversion does not need to be permuted in the connectivity file

