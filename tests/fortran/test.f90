program test
  implicit none

  double precision :: Vpot
  double precision, allocatable :: coord(:),grads(:)

  character(len=5), allocatable :: at_name(:)
  character(len=5), allocatable :: monomers(:)
  character(len=20) :: xyz
  
  integer, allocatable :: nats(:)
  character :: atom

  integer :: n_at,i,j,nmon

  external energyf90;
  external energyf90g;

  ! need to define here the numebr of monomers, and number of atoms in each one)
  nmon = 7
  allocate(nats(nmon),monomers(nmon))

  nats(1) = 1
  monomers(1) = "li"
  do i=2,nmon
    nats(i) = 3
    monomers(i) = "h2o"
  enddo

  ! Open file called input.xyz. 
  ! Contains the coordinates in XYZ format.
  ! Assumes halide at the end (because the C++ code does that)
  xyz='input.xyz'
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

  ! These functions need the coordinates in a single vector (x1,y1,z1,x2,y2,...), the atom names in a single vector, trimed and with a null character at the end, and the double in which you want the potential. MAKE SURE YOU HAVE LEN=5 IN THE CHAR ARRAY!!!!!!

  do i =1,n_at
    at_name(i)=trim(at_name(i))//CHAR(0)
  enddo
  do i=1,nmon
    monomers(i) = trim(monomers(i))//CHAR(0)
  enddo
    
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

