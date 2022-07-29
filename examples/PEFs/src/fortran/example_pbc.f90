program test
  implicit none

  double precision :: Vpot
  double precision, allocatable :: coord(:),grads(:)
  double precision, allocatable :: box(:)
  double precision, allocatable :: virial(:)

  character(len=5), allocatable :: at_name(:),at_name_cp(:)
  character(len=5), allocatable :: monomers(:)
  character(len=20) :: xyz, json_file, inp
  
  integer, allocatable :: nats(:)
  character :: atom

  integer :: n_at,i,j,nmon,k

  external initialize_system
  external get_energy
  external get_energy_g
  external finalize_system
  external get_virial

  ! User needs to provide with lists with the coordinates, atom names, monomer types,
  ! and number of atoms for each monomer.
  ! In this test case we have an input file providing it
  inp = 'input'

  ! The input has the number of monomers in the first line
  open(unit = 55, file = inp, status = 'old', action = 'read')
  read(55,*) nmon
  
  allocate(nats(nmon),monomers(nmon),box(9),virial(9))

  ! And the monomer ids and number of atom of each monomer in the third line
  read(55,*) (monomers(i),i=1,nmon)
  read(55,*) (nats(i),i=1,nmon)
  close(55)

  ! Set json file
  json_file = 'mbx_pbc.json'//CHAR(0)
  ! Open file called input.xyz. 
  ! Contains the coordinates in XYZ format.
  ! Assumes halide at the end (because the C++ code does that)
  xyz='input.xyz'
  open(unit = 55, file = xyz, status = 'old', action = 'read')

  ! Number of atoms
  read(55,*) n_at

  ! Allocation of the coord vector and the atom name vector,
  ! as so as the vector that will store the gradients
  allocate(coord(n_at*3),at_name_cp(n_at),at_name(n_at),grads(3*n_at))

  ! Reading comment file from XYZ. Not saving it
  read(55,*)

  ! Reading and saving the names and coordinates
  ! Coordinates will be XYZXYZXYZ... OHHOHH...OHHX
  do i=1,n_at
    read(55,*) at_name(i), (coord(3*(i-1)+j),j=1,3)
  enddo

  ! Done reading. Closing file.
  close(55)

  ! Copy at_name
  at_name_cp(:) = at_name(:)

  ! MAKE SURE YOU HAVE LEN=5 IN THE CHAR ARRAY!!!!!!
  do i =1,n_at
    at_name(i)=trim(at_name(i))//CHAR(0)
  enddo
  do i=1,nmon
    monomers(i) = trim(monomers(i))//CHAR(0)
  enddo
    
  ! initialize_system
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

  ! get_energy
  ! @coord is a double 1D array with the coordinates of all atoms.
  !        It needs xyzxyzxyzxyz... Basically reading an xyz file
  !        and has length 3*n_at
  ! @n_at is an integer with the number of atoms
  ! @box is a double array of 9 elements (ONLY ORTHOROMBIC BOXES, FOR NOW)
  ! @Vpot is an output double variable that will contain the energy 
  !       after the call to the energy function

  ! get_energy_g 
  ! @grads is an output double 1D array that will contain the gradients 
  !        (XYZ) of each atom as gx1 gy1 gz1 gx2 gy2 gz2 ...
 
  ! First, initialize system
  call initialize_system(coord, nats, at_name, monomers, nmon, json_file)
  write(*,*) "System initialized."
  write(*,*)

  ! Update the box
  box(:) = 0.0
  box(1) = 20.0
  box(5) = 20.0
  box(9) = 20.0

  ! Energy call no gradients pbc
  call get_energy_pbc(coord, n_at, box, Vpot)
  write(*,*) "Testing functions that use an array of the coordinates (PBC)"
  write(*,*)
  write(*,*) "Energy (no gradients) = " , Vpot
  write(*,*)

  ! Energy call gradients pbc
  call get_energy_pbc_g(coord, n_at, box, Vpot, grads)
  write(*,*) "Energy (gradients) = " , Vpot
  write(*,*)
  write(*,*) "Writting gradients below..."
  do i=1,n_at
    write(*,*) at_name_cp(i),(grads(3*(i-1)+j),j=1,3)
  enddo
  write(*,*)

  ! At this point the virial should be updated
  ! For gas phase simulations it will return trash. But you can still get it.
  call get_virial(virial)
  write(*,*) "Virial"
  write(*,*)
  do i=1,3
    write(*,*) (virial(3*(i-1) + j),j=1,3)
  enddo
  write(*,*)

  ! Don't forget to free the system
  call finalize_system()


end program

