program test
  implicit none

  double precision :: Vpot
  double precision, allocatable :: coord(:),grads(:)

  character(len=5), allocatable :: at_name(:)
  character(len=5), allocatable :: monomers(:)
  character(len=20) :: xyz
  
  integer, allocatable :: nats(:)
  character :: atom

  integer :: n_at,i,j,nmon,k

  ! The two functions will give the same energy.
  ! energyf90 won't compute gradients (and is faster)
  ! energyf90g will compute and return gradients.
  external energyf90;
  external energyf90g;

  ! need to define here the numebr of monomers, and number of atoms in each one
  ! In the case of a water cluster of 256 molecules, the variables should be:
  
  ! nmon = 256
  ! allocate(nats(nmon),monomers(nmon))

  ! do i=1,nmon
  !   nats(i) = 3
  !   monomers(i) = "h2o"
  ! enddo

  ! This will work for the test case, which contains a lithium monomer
  ! at the beggining, and then 6 water molecules
  nmon = 3
  allocate(nats(nmon),monomers(nmon))

  nats(nmon) = 1
  monomers(nmon) = "i"
  do i=1,nmon-1
    nats(i) = 3
    monomers(i) = "h2o"
  enddo

  ! Open file called input.xyz. 
  ! Contains the coordinates in XYZ format.
  ! Assumes halide at the end (because the C++ code does that)
  xyz='1.xyz'
  open(unit = 55, file = xyz, status = 'old', action = 'read')

  ! Number of atoms
  read(55,*) n_at

  ! Allocation of the coord vector and the atom name vector,
  ! as so as the vector that will store the gradients
  allocate(coord(n_at*3),at_name(n_at),grads(3*n_at))

  ! Reading comment file from XYZ. Not saving it
  read(55,*)

  ! Reading and saving the names and coordinates
  ! Coordinates will be XYZXYZXYZ... OHHOHH...OHHX
  do i=1,n_at
    read(55,*) at_name(i), (coord(3*(i-1)+j),j=1,3)
  enddo

  ! Done reading. Closing file.
  close(55)

  ! MAKE SURE YOU HAVE LEN=5 IN THE CHAR ARRAY!!!!!!

  do i =1,n_at
    at_name(i)=trim(at_name(i))//CHAR(0)
  enddo
  do i=1,nmon
    monomers(i) = trim(monomers(i))//CHAR(0)
  enddo
    
  ! energyf90:
  ! @coord is a double 1D array with the coordinates of all atoms.
  !        It needs xyzxyzxyzxyz... Basically reading an xyz file
  !        and has length 3*n_at
  ! @nats is an integer 1D array of length nmon with the number of atoms
  !       in each monomer.
  ! @at_name is a char 2D array. The second dimention needs to be 5. 
  !          The first dimention needs to be of length of number of atoms
  !          It contains the name of the atoms as they appear in the 
  !          periodic table
  ! @monomers is a char 2D array. The second dimention needs to be 5.
  !           The first dimention needs to be of length of number of mons
  !           It contains the names of the monomers (h2o for water,
  !           li, na, k, rb, cs, f, cl, br, i for the alkali metal ions
  !           and the halides)
  ! @nmon is an integer with the total number of monomers 
  ! @Vpot is an output double variable that will contain the energy 
  !       after the call to the energy function

  ! energyf90g:
  ! Same inputs but also has:
  ! @grads is an output double 1D array that will contain the gradients 
  !        (XYZ) of each atom as gx1 gy1 gz1 gx2 gy2 gz2 ...
 

  call energyf90(coord, nats, at_name, monomers, nmon, Vpot)
  write(*,*) "Testing functions that use an array of the coordinates "
  write(*,*)
  write(*,*) "Energy (no gradients) = " , Vpot
  write(*,*)

  call energyf90g(coord, nats, at_name, monomers, nmon, grads, Vpot)
  write(*,*) "Energy (gradients) = " , Vpot
  write(*,*)
  write(*,*) "Writting gradients below..."
  do i=1,n_at
    write(*,*) at_name(i),(grads(3*(i-1)+j),j=1,3)
  enddo
  write(*,*)

end program

