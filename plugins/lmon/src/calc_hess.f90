program opt

use globals

use fac

use matrixos4

implicit none


integer :: natom = 0,nmon = 0, i22=0

character :: filename1*16, filename2*16,filename3*16,filename4*16,atom1*5,atom2*5
character(len=20) :: json_file, inp,xyz

character(len=5), allocatable :: at_name(:)
integer, allocatable :: nats(:)
character(len=5), allocatable :: monomers(:)

real(wp), parameter :: hbar=1.0_wp, c_in_au = 137.035999_wp, a0_to_cm = 5.291772108E-9_wp, debye_to_ea0 = 0.393430307_wp, m_e=1822.8883942_wp,eps=1.0E-3_wp,au_to_kcal=627.509469_wp

real(wp), dimension(:,:), allocatable :: hess, hess_inv

real(wp), dimension(:), allocatable :: m,omega,x, xnew,grad,tempV,work,redmass,grad_nm

integer :: j,k,l,j2,k2,l2,n,info,jmode,kmode,lmode,imode,iter, myid, numprocs

real(wp) :: energy0, energy1,gamma,temp1,temp2

external initialize_system
external get_energy_g
external finalize_system

gamma=1.0_wp

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!  1. Calculate energy, gradient, hessian at initial geometry x0
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! User needs to provide with lists with the coordinates, atom names, monomer types,
! and number of atoms for each monomer.
! In this test case we have an input file providing it
inp = 'input_f90'

! The input has the number of monomers in the first line
open(unit = 55, file = inp, status = 'old', action = 'read')
read(55,*) nmon

allocate(nats(nmon),monomers(nmon))

! And the monomer ids and number of atom of each monomer in the third line
read(55,*) (monomers(i22),i22=1,nmon)
read(55,*) (nats(i22),i22=1,nmon)
close(55)

! Set json file
json_file = 'mbx.json'//CHAR(0)
! Open file called input.xyz.
! Contains the coordinates in XYZ format.
! Assumes halide at the end (because the C++ code does that)
xyz='input.xyz'
open(unit = 55, file = xyz, status = 'old', action = 'read')

! Number of atoms
read(55,*) natom

! Allocation of the coord vector and the atom name vector,
! as so as the vector that will store the gradients
allocate(x(natom*3),at_name(natom))

! Reading comment file from XYZ. Not saving it
read(55,*)

! Reading and saving the names and coordinates
! Coordinates will be XYZXYZXYZ... OHHOHH...OHHX
do i22=1,natom
  read(55,*) at_name(i22), (x(3*(i22-1)+j),j=1,3)
enddo

! Done reading. Closing file.
close(55)


Allocate(xnew(3*natom),grad(3*natom),hess(3*natom,3*natom),hess_inv(3*natom,3*natom),tempV(3*natom),m(natom),omega(3*natom),work(3*(3*natom)-1),redmass(3*natom),grad_nm(3*natom-6))

open(unit=52,file='mass.dat',status='replace')
open(unit=53,file='geom_ang.dat',status='replace')
do j=1,natom
  m(j)=atom_mass(at_name(j))
  write(*,*) at_name(j), m(j)
  write(52,*) m(j)
  write(53,*) (x(3*(j-1)+k),k=1,3)
enddo      
close(52)
close(53)

! MAKE SURE YOU HAVE LEN=5 IN THE CHAR ARRAY!!!!!!

do i22 =1,natom
  at_name(i22)=trim(at_name(i22))//CHAR(0)
enddo
do i22=1,nmon
  monomers(i22) = trim(monomers(i22))//CHAR(0)
enddo

! First, initialize system
call initialize_system(x, nats, at_name, monomers, nmon, json_file)

do j=1,3*natom
  tempV=x
  tempV(j)=tempV(j)+eps
  call get_energy_g(tempV, natom, temp1, hess_inv(:,1))

  tempV=x
  tempV(j)=tempV(j)-eps
  call get_energy_g(tempV, natom, temp1, hess_inv(:,2))

  hess(:,j)=0.5_wp*(hess_inv(:,1)-hess_inv(:,2))/eps
  
end do!j

open(unit=5,file='hess_au.dat')
write(5,'(e24.16)') ((hess(j,k)/au_to_kcal*(a0_to_cm*1.0E8_wp)**2.0_wp,k=1,3*natom),j=1,3*natom)
close(5)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Mass-weight Hessian
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
do j=1,natom*3
  do k=1,natom*3
    hess(j,k)=hess(j,k)/sqrt(m((j-1)/3+1)*m((k-1)/3+1))
  end do!k
end do!j


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Diagonalize mass-weighted Hessian
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

call DSYEV('v','u',3*natom,hess,3*natom,omega,work,3*(3*natom)-1,info)

omega=omega/au_to_kcal*(a0_to_cm*1.0E8_wp)**2.0_wp/m_e

write(*,'(f15.8)') (sqrt(omega(j))/(2.0_wp*pi*c_in_au*a0_to_cm),j=1,3*natom)

CONTAINS 
  FUNCTION atom_mass(atom)

    IMPLICIT NONE

    CHARACTER(LEN=2), INTENT(IN) :: atom
    DOUBLE PRECISION :: atom_mass
    DOUBLE PRECISION, PARAMETER :: Hmass = 1.00782503223
    DOUBLE PRECISION, PARAMETER :: Dmass = 2.01410177812
    DOUBLE PRECISION, PARAMETER :: Cmass = 12.0000000
    DOUBLE PRECISION, PARAMETER :: Omass = 15.99491461957
    DOUBLE PRECISION, PARAMETER :: Fmass = 18.99840316273
    DOUBLE PRECISION, PARAMETER :: Clmass = 34.968852682
    DOUBLE PRECISION, PARAMETER :: Brmass = 78.9183376
    DOUBLE PRECISION, PARAMETER :: Imass = 126.9044719
    DOUBLE PRECISION, PARAMETER :: Limass = 7.0160034366
    DOUBLE PRECISION, PARAMETER :: Namass = 22.989769282
    DOUBLE PRECISION, PARAMETER :: Kmass  = 38.9637064864
    DOUBLE PRECISION, PARAMETER :: Rbmass = 84.9114977282
    DOUBLE PRECISION, PARAMETER :: Csmass = 132.905429



    IF (atom == 'O') THEN
       atom_mass = Omass
    ELSE IF (atom == 'C') THEN
       atom_mass = Cmass
    ELSE IF (atom == 'H') THEN
       atom_mass = Hmass
    ELSE IF (atom == 'D') THEN
       atom_mass = Dmass
    ELSE IF (atom == 'Cl') THEN
       atom_mass = Clmass
    ELSE IF (atom == 'Br') THEN
       atom_mass = Brmass
    ELSE IF (atom == 'I') THEN
       atom_mass = Imass
    ELSE IF (atom =='F') THEN
       atom_mass = Fmass
    ELSE IF (atom =='Li') THEN
       atom_mass = Limass
    ELSE IF (atom =='Na') THEN
       atom_mass = Namass
    ELSE IF (atom =='K') THEN
       atom_mass = Kmass
    ELSE IF (atom =='Rb') THEN
       atom_mass = Rbmass
    ELSE IF (atom =='Cs') THEN
       atom_mass = Csmass
    ELSE
       WRITE(*,*) 'atom ', atom, ' is not recognized'
       STOP
    END IF

  END FUNCTION atom_mass
end program opt
