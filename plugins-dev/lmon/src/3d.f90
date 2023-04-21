program tri

use globals

use fac

use matrixos4

implicit none

character :: filename*16, atom1*2, atom2*2, atom3*2, exch*10, bas*10

character(len=5), allocatable :: at_name(:)
character(len=20) :: json_file, inp,xyz
integer, allocatable :: nats(:)
character(len=5), allocatable :: monomers(:)

! n1,n2 and n3 must be odd
integer, parameter ::  n1 = 9, n2 = 9, n3 = 9, nsize = n1*n2*n3
integer :: nwater=0, natom=0, nmode=0, i22, i2

real(wp), parameter :: hbar=1.0_wp, c_in_au = 137.035999_wp, a0_to_cm = 5.291772108E-9_wp, debye_to_ea0 = 0.393430307_wp, m_e=1822.8883942_wp,au_to_kcal=627.509469_wp

real(wp) :: m1, m2, m3, omega1, omega2, omega3, alpha1, alpha2, alpha3, temp                     

real(wp), dimension(:), allocatable :: grad,xxx

integer ::n, j,k,l,j2,k2,l2,j3,k3,l3,info, charge, multiplicity,monomer,nmon

real(wp), dimension(:,:,:), allocatable :: V,modes

real(wp), dimension(:,:,:,:), allocatable :: D3

real(wp), dimension(:,:), allocatable :: a1, adag1, a2, adag2, a3, adag3, p1, p2,p3, KE, KE0, PE, H, eq, mode1, mode2, mode3,Dx,Dy,Dz

real(wp), dimension(:), allocatable :: mmm,eigen, work, gqpoints, weights, roots1, roots2, roots3,m,omega,alpha,tempV1,tempV,int

integer, dimension(:,:), allocatable :: mon_ass

external initialize_system
external get_energy_g
external get_total_dipole
external finalize_system

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

Allocate(a1(nsize,nsize),a2(nsize,nsize),a3(nsize,nsize),adag1(nsize,nsize),adag2(nsize,nsize),adag3(nsize,nsize),mode1(natom,3),mode2(natom,3),mode3(natom,3),mmm(natom),xxx(3*natom),&
  &p1(nsize,nsize),p2(nsize,nsize),p3(nsize,nsize),V(n1+2,n2+2,n3+2), KE(nsize,nsize),KE0(nsize,nsize),eq(natom,3),&
  &PE(nsize,nsize),H(nsize,nsize),eigen(nsize),work(3*nsize-1),gqpoints(max(n1,n2,n3)+2),&
  &weights(max(n1,n2,n3)+2),roots1(max(n1,n2,n3)+2),roots2(max(n1,n2,n3)+2),roots3(max(n1,n2,n3)+2),tempV1(3*natom),D3(n1+2,n2+2,n3+2,3),Dx(nsize,nsize),Dy(nsize,nsize),Dz(nsize,nsize),int(nsize),tempV(nsize),grad(3*natom),at_name(natom))

read(55,*)
do j=1,natom
  read(55,*) at_name(j), (xxx(3*(j-1)+i2),i2=1,3)
  mmm(j)=atom_mass(at_name(j))
  write(*,*) at_name(j), mmm(j)
enddo
close(55)

nwater = 0
do j=1,nmon
  if (trim(monomers(j)) == "h2o") then
      nwater = nwater + 1
  endif
enddo
nmode=nwater*3

do i22 =1,natom
  at_name(i22)=trim(at_name(i22))//CHAR(0)
enddo
do i22=1,nmon
  monomers(i22) = trim(monomers(i22))//CHAR(0)
enddo

call initialize_system(xxx, nats, at_name, monomers, nmon, json_file)

allocate(modes(natom,3,nmode),m(nmode),omega(nmode),alpha(nmode),mon_ass(nmode,2))

write(*,*) "Located ", nwater, " water molecules."

open(unit=23,file="1_locmonomer.txt")
10 format(f15.4)

open(unit=122,file="line_spectra.dat")

open(unit=1,file="redmass.dat")
read(1,*) (m(j),j=1,nmode)
close(1)

open(unit=4,file="omega.dat")
read(4,*) (omega(j), j=1,nmode)
close(4)

omega=omega*(2.0_wp*pi*c_in_au*a0_to_cm)

do j=1,nmode
   alpha(j) = m(j)*omega(j)
end do


open(unit=5,file="geom_ang.dat")
read(5,*) ((eq(j,k),k=1,3), j=1,natom)
close(5)

open(unit=7,file="modes.dat")
read(7,*) (((modes(j,k,l),k=1,3),j=1,natom),l=1,nmode)
close(7)

open(unit=8,file="assign.dat")
read(8,*) ((mon_ass(j,k),k=1,2),j=1,nmode)
close(8)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! gaussian-hermite quadrature
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

call gauher(gqpoints,weights,max(n1,n2,n3)+2)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! adagger  and a matrices
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

do j=1,nsize
   do k=1,nsize
      a1(j,k)=0.0_wp
      a2(j,k)=0.0_wp
      a3(j,k)=0.0_wp
      adag1(j,k)=0.0_wp
      adag2(j,k)=0.0_wp
      adag3(j,k)=0.0_wp
   end do!k
end do!j


do j=2,n1
   do k=1,n2
      do l=1,n3
         adag1((j-1)*n2*n3+(k-1)*n3+l,(j-2)*n2*n3+(k-1)*n3+l) = sqrt(real(j-1,wp))
         a1((j-2)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-1)*n3+l) = sqrt(real(j-1,wp))
      end do!l
   end do!k
end do!j

do j=1,n1
   do k=2,n2
      do l=1,n3
         adag2((j-1)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-2)*n3+l) = sqrt(real(k-1,wp))
         a2((j-1)*n2*n3+(k-2)*n3+l,(j-1)*n2*n3+(k-1)*n3+l) = sqrt(real(k-1,wp))
      end do!l
   end do!k
end do!j

do j=1,n1
   do k=1,n2
      do l=2,n3
         adag3((j-1)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-1)*n3+l-1) = sqrt(real(l-1,wp))
         a3((j-1)*n2*n3+(k-1)*n3+l-1,(j-1)*n2*n3+(k-1)*n3+l) = sqrt(real(l-1,wp))
      end do!l
   end do!k
end do!j

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

do monomer=1,nwater

    m1=m(mon_ass((monomer-1)*3+1,1))
    m2=m(mon_ass((monomer-1)*3+2,1))
    m3=m(mon_ass((monomer-1)*3+3,1))

    omega1=omega(mon_ass((monomer-1)*3+1,1))
    omega2=omega(mon_ass((monomer-1)*3+2,1))
    omega3=omega(mon_ass((monomer-1)*3+3,1))

    alpha1=alpha(mon_ass((monomer-1)*3+1,1))
    alpha2=alpha(mon_ass((monomer-1)*3+2,1))
    alpha3=alpha(mon_ass((monomer-1)*3+3,1))

    mode1=modes(:,:,mon_ass((monomer-1)*3+1,1))
    mode2=modes(:,:,mon_ass((monomer-1)*3+2,1))
    mode3=modes(:,:,mon_ass((monomer-1)*3+3,1))


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! momenta (leaving i out)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

p1=sqrt(0.5_wp*hbar*omega1)*(adag1-a1)

p2=sqrt(0.5_wp*hbar*omega2)*(adag2-a2)

p3=sqrt(0.5_wp*hbar*omega3)*(adag3-a3)


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! kinetic energy
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
call DGEMM('n','n',nsize,nsize,nsize,-0.5_wp,p1,nsize,p1,nsize,0.0_wp,KE,nsize)

j=n1
   do k=1,n2
      do l=1,n3
        KE((j-1)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-1)*n3+l)=0.5_wp*hbar*omega1*(j-0.5_wp)
      end do!l
  end do!k

call DGEMM('n','n',nsize,nsize,nsize,-0.5_wp,p2,nsize,p2,nsize,0.0_wp,KE0,nsize)

do j=1,n1
    k=n2
      do l=1,n3
        KE0((j-1)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-1)*n3+l)=0.5_wp*hbar*omega2*(k-0.5_wp)
      end do!l
  end do!j

KE=KE+KE0

call DGEMM('n','n',nsize,nsize,nsize,-0.5_wp,p3,nsize,p3,nsize,0.0_wp,KE0,nsize)

do j=1,n1
  do  k=1,n2
       l=n3
        KE0((j-1)*n2*n3+(k-1)*n3+l,(j-1)*n2*n3+(k-1)*n3+l)=0.5_wp*hbar*omega3*(l-0.5_wp)
      end do!k
  end do!j

KE=KE+KE0







roots1=gqpoints/sqrt(alpha1)
roots2=gqpoints/sqrt(alpha2)
roots3=gqpoints/sqrt(alpha3)



do j=1,n1+2
do k=1,n2+2
do l=1,n3+2




do n=1,natom
  do k2=1,3 
    tempV1((n-1)*3+k2)=eq(n,k2)+(roots1(j)*mode1(n,k2)+roots2(k)*mode2(n,k2)+roots3(l)*mode3(n,k2))*a0_to_cm*1.0E8_wp 
  end do!
end do   

do n =1,natom
  at_name(n)=trim(at_name(n))//CHAR(0)
enddo
      
      call get_energy_g(tempV1, natom, V(j,k,l), grad)
      call get_total_dipole(D3(j,k,l,:))
      !call calcpot(2,V(j,k,l),tempV1)
      !call calcdip(6,tempV1,D3(j,k,l,:))




end do!n
end do!k
end do!l
 
V=V/au_to_kcal

temp= V((n1+3)/2,(n2+3)/2,(n3+3)/2)

do l=1,n1+2
   do l2=1,n2+2
      do l3=1,n3+2
      
      V(l,l2,l3)=V(l,l2,l3)-temp                         
                        
      end do!l3
   end do!l2
end do!l



!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! potential energy
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
do j=1,n1
   do k=j,n1
      do j2=1,n2
         do k2=j2,n2
            do j3=1,n3
               do k3=j3,n3
            
                  temp = 0.0_wp

                  do l=1,n1+2
                     do l2=1,n2+2
                        do l3=1,n3+2
                        
      
 temp = temp + weights(l)*weights(l2)*weights(l3)&
 &*hermite(j-1,gqpoints(l))*hermite(k-1,gqpoints(l))*hermite(j2-1,gqpoints(l2))*hermite(k2-1,gqpoints(l2))*hermite(j3-1,gqpoints(l3))*hermite(k3-1,gqpoints(l3))&
                        &*V(l,l2,l3)
                        
                        
                        end do!l3
                     end do!l2
                  end do!l

 temp = temp*1.0_wp/sqrt(2.0_wp**(j+k+j2+k2+j3+k3-6)&
            &*factorial(j-1)*factorial(k-1)*factorial(j2-1)*factorial(k2-1)*factorial(j3-1)*factorial(k3-1))/(pi**1.5_wp)
            
            PE((j-1)*n2*n3+(j2-1)*n3+j3,(k-1)*n2*n3+(k2-1)*n3+k3) = temp


            if ((k .ne. j) .or.(k2 .ne. j2) .or. (k3 .ne. j3))  then
                PE((k-1)*n2*n3+(j2-1)*n3+j3,(j-1)*n2*n3+(k2-1)*n3+k3) = temp
                PE((j-1)*n2*n3+(k2-1)*n3+j3,(k-1)*n2*n3+(j2-1)*n3+k3) = temp
                PE((j-1)*n2*n3+(j2-1)*n3+k3,(k-1)*n2*n3+(k2-1)*n3+j3) = temp
                PE((j-1)*n2*n3+(k2-1)*n3+k3,(k-1)*n2*n3+(j2-1)*n3+j3) = temp
                PE((k-1)*n2*n3+(j2-1)*n3+k3,(j-1)*n2*n3+(k2-1)*n3+j3) = temp
                PE((k-1)*n2*n3+(k2-1)*n3+j3,(j-1)*n2*n3+(j2-1)*n3+k3) = temp
                PE((k-1)*n2*n3+(k2-1)*n3+k3,(j-1)*n2*n3+(j2-1)*n3+j3) = temp
            end if
            
              end do!k3
           end do!j3

         end do!k2
      end do!j2

   end do!k
end do!j


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! diagonalize H
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

H = KE+PE
            
call DSYEV('v','u',nsize,H,nsize,eigen,work,3*nsize-1,info)

if (info .ne. 0) then
    write(*,*) 'diagonalizing algorithm failed to converge', info,'DSYEV'
end if

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Dx
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
do j=1,n1
   do k=j,n1
      do j2=1,n2
         do k2=j2,n2
            do j3=1,n3
               do k3=j3,n3
            
                  temp = 0.0_wp

                  do l=1,n1+2
                     do l2=1,n2+2
                        do l3=1,n3+2
                        
      
 temp = temp + weights(l)*weights(l2)*weights(l3)&
 &*hermite(j-1,gqpoints(l))*hermite(k-1,gqpoints(l))*hermite(j2-1,gqpoints(l2))*hermite(k2-1,gqpoints(l2))*hermite(j3-1,gqpoints(l3))*hermite(k3-1,gqpoints(l3))&
                        &*D3(l,l2,l3,1)
                        
                        
                        end do!l3
                     end do!l2
                  end do!l

 temp = temp*1.0_wp/sqrt(2.0_wp**(j+k+j2+k2+j3+k3-6)&
            &*factorial(j-1)*factorial(k-1)*factorial(j2-1)*factorial(k2-1)*factorial(j3-1)*factorial(k3-1))/(pi**1.5_wp)
            
            
            Dx((j-1)*n2*n3+(j2-1)*n3+j3,(k-1)*n2*n3+(k2-1)*n3+k3) = temp


            if ((k .ne. j) .or.(k2 .ne. j2) .or. (k3 .ne. j3))  then
                Dx((k-1)*n2*n3+(j2-1)*n3+j3,(j-1)*n2*n3+(k2-1)*n3+k3) = temp
                Dx((j-1)*n2*n3+(k2-1)*n3+j3,(k-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dx((j-1)*n2*n3+(j2-1)*n3+k3,(k-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dx((j-1)*n2*n3+(k2-1)*n3+k3,(k-1)*n2*n3+(j2-1)*n3+j3) = temp
                Dx((k-1)*n2*n3+(j2-1)*n3+k3,(j-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dx((k-1)*n2*n3+(k2-1)*n3+j3,(j-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dx((k-1)*n2*n3+(k2-1)*n3+k3,(j-1)*n2*n3+(j2-1)*n3+j3) = temp
            end if
            
              end do!k3
           end do!j3

         end do!k2
      end do!j2

   end do!k
end do!j

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Dy
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
do j=1,n1
   do k=j,n1
      do j2=1,n2
         do k2=j2,n2
            do j3=1,n3
               do k3=j3,n3
            
                  temp = 0.0_wp

                  do l=1,n1+2
                     do l2=1,n2+2
                        do l3=1,n3+2
                        
      
 temp = temp + weights(l)*weights(l2)*weights(l3)&
 &*hermite(j-1,gqpoints(l))*hermite(k-1,gqpoints(l))*hermite(j2-1,gqpoints(l2))*hermite(k2-1,gqpoints(l2))*hermite(j3-1,gqpoints(l3))*hermite(k3-1,gqpoints(l3))&
                        &*D3(l,l2,l3,2)
                        
                        
                        end do!l3
                     end do!l2
                  end do!l

 temp = temp*1.0_wp/sqrt(2.0_wp**(j+k+j2+k2+j3+k3-6)&
            &*factorial(j-1)*factorial(k-1)*factorial(j2-1)*factorial(k2-1)*factorial(j3-1)*factorial(k3-1))/(pi**1.5_wp)
            
            Dy((j-1)*n2*n3+(j2-1)*n3+j3,(k-1)*n2*n3+(k2-1)*n3+k3) = temp


            if ((k .ne. j) .or.(k2 .ne. j2) .or. (k3 .ne. j3))  then
                Dy((k-1)*n2*n3+(j2-1)*n3+j3,(j-1)*n2*n3+(k2-1)*n3+k3) = temp
                Dy((j-1)*n2*n3+(k2-1)*n3+j3,(k-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dy((j-1)*n2*n3+(j2-1)*n3+k3,(k-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dy((j-1)*n2*n3+(k2-1)*n3+k3,(k-1)*n2*n3+(j2-1)*n3+j3) = temp
                Dy((k-1)*n2*n3+(j2-1)*n3+k3,(j-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dy((k-1)*n2*n3+(k2-1)*n3+j3,(j-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dy((k-1)*n2*n3+(k2-1)*n3+k3,(j-1)*n2*n3+(j2-1)*n3+j3) = temp
            end if
            
              end do!k3
           end do!j3

         end do!k2
      end do!j2

   end do!k
end do!j

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Dz
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
do j=1,n1
   do k=j,n1
      do j2=1,n2
         do k2=j2,n2
            do j3=1,n3
               do k3=j3,n3
            
                  temp = 0.0_wp

                  do l=1,n1+2
                     do l2=1,n2+2
                        do l3=1,n3+2
                        
      
 temp = temp + weights(l)*weights(l2)*weights(l3)&
 &*hermite(j-1,gqpoints(l))*hermite(k-1,gqpoints(l))*hermite(j2-1,gqpoints(l2))*hermite(k2-1,gqpoints(l2))*hermite(j3-1,gqpoints(l3))*hermite(k3-1,gqpoints(l3))&
                        &*D3(l,l2,l3,3)
                        
                        
                        end do!l3
                     end do!l2
                  end do!l

 temp = temp*1.0_wp/sqrt(2.0_wp**(j+k+j2+k2+j3+k3-6)&
            &*factorial(j-1)*factorial(k-1)*factorial(j2-1)*factorial(k2-1)*factorial(j3-1)*factorial(k3-1))/(pi**1.5_wp)
            
            Dz((j-1)*n2*n3+(j2-1)*n3+j3,(k-1)*n2*n3+(k2-1)*n3+k3) = temp


            if ((k .ne. j) .or.(k2 .ne. j2) .or. (k3 .ne. j3))  then
                Dz((k-1)*n2*n3+(j2-1)*n3+j3,(j-1)*n2*n3+(k2-1)*n3+k3) = temp
                Dz((j-1)*n2*n3+(k2-1)*n3+j3,(k-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dz((j-1)*n2*n3+(j2-1)*n3+k3,(k-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dz((j-1)*n2*n3+(k2-1)*n3+k3,(k-1)*n2*n3+(j2-1)*n3+j3) = temp
                Dz((k-1)*n2*n3+(j2-1)*n3+k3,(j-1)*n2*n3+(k2-1)*n3+j3) = temp
                Dz((k-1)*n2*n3+(k2-1)*n3+j3,(j-1)*n2*n3+(j2-1)*n3+k3) = temp
                Dz((k-1)*n2*n3+(k2-1)*n3+k3,(j-1)*n2*n3+(j2-1)*n3+j3) = temp
            end if
            
              end do!k3
           end do!j3

         end do!k2
      end do!j2

   end do!k
end do!j

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Intensities
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

do j=2,5

  int(j)=0.0_wp

  call DSYMV('u',nsize,1.0_wp,Dx,nsize,H(:,j),1,0.0_wp,tempV,1)

  int(j)=int(j)+(DDOT(H(:,1),tempV))**2.0_wp


  call DSYMV('u',nsize,1.0_wp,Dy,nsize,H(:,j),1,0.0_wp,tempV,1)

  int(j)=int(j)+(DDOT(H(:,1),tempV))**2.0_wp


  call DSYMV('u',nsize,1.0_wp,Dz,nsize,H(:,j),1,0.0_wp,tempV,1)

  int(j)=int(j)+(DDOT(H(:,1),tempV))**2.0_wp

  int(j)=int(j)*debye_to_ea0**2.0_wp*(eigen(j)-eigen(1))*2.0_wp*pi*6.02214179E23_wp/3.0_wp/c_in_au**2.0_wp*a0_to_cm*1.0E-5_wp



end do!j


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! Energy in cm^-1
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
eigen=eigen/(hbar*2.0_wp*pi*c_in_au*a0_to_cm)

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
! Write out
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
write(23,*) 'monomer=',monomer
write(23,*) 'Local Harmonic frequencies (cm^-1):'
write(23,10) omega1/(2.0_wp*pi*c_in_au*a0_to_cm),omega2/(2.0_wp*pi*c_in_au*a0_to_cm),omega3/(2.0_wp*pi*c_in_au*a0_to_cm)

temp=omega1
k=1
if (omega2 < temp) then 
        temp=omega2
        k=2
end if 
if (omega3 < temp) then 
        temp=omega3
        k=3
end if 

write(23,*) 
write(23,*) 'Local 3-mode Anharmonic scan' 
write(23,'(3A20)') 'frequencies (cm^-1)','Intensity (km/mol)','bend overtone?'

if (k==1) then 
        do j=2,5
                write(23,'(3f20.4)') eigen(j)-eigen(1),int(j),H((2*n2*n3+1),j)
                write(122,'(3f20.4)') eigen(j)-eigen(1),int(j)
        end do!j
end if 
if (k==2) then 
        do j=2,5
                write(23,'(3f20.4)') eigen(j)-eigen(1),int(j),H((2*n3+1),j)
                write(122,'(3f20.4)') eigen(j)-eigen(1),int(j)
        end do!j
end if 
if (k==3) then 
        do j=2,5
                write(23,'(3f20.4)') eigen(j)-eigen(1),int(j),H((2+1),j)
                write(122,'(3f20.4)') eigen(j)-eigen(1),int(j)
        end do!j
end if
 
write(23,*) '#-----------------------' 
write(23,*) 

end do!monomer

call finalize_system()


close(23)
close(122)

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



end program tri

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! gaussian-hermite roots and weights
!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

SUBROUTINE gauher(x,w,n)

use globals  

implicit none    
      
integer, intent(in)  :: n

real(wp), intent(out) :: w(n),x(n)

integer, parameter :: MAXIT=10

real(wp), parameter :: EPS=3.0E-13_wp, PIM4=0.7511255444649425_wp
      
integer :: k,its,j,m
      
real(wp) :: p1,p2,p3,pp,z,z1
      
      
m=(n+1)/2


do k=1,m
        if(k.eq.1) then
          z=sqrt(real(2*n+1,wp))-1.85575_wp*(2.0_wp*n+1.0_wp)**(-0.16667_wp)
        else if(k.eq.2) then
          z=z-1.14_wp*real(n,wp)**0.426_wp/z
        else if (k.eq.3) then
          z=1.86_wp*z-0.86_wp*x(1)
        else if (k.eq.4) then
          z=1.91_wp*z-0.91_wp*x(2)
        else
          z=2.0_wp*z-x(k-2)
        end if
        
        p1=PIM4
        p2=0.0_wp
          
          do j=1,n
            p3=p2
            p2=p1
            p1=z*sqrt(2.0_wp/j)*p2-sqrt(real(j-1,wp)/real(j,wp))*p3
          end do!j
          
          pp=sqrt(2.0_wp*n)*p2
          z1=z
          z=z1-p1/pp
        
        do its=1,MAXIT
        
          if(abs(z-z1) > EPS) then
            p1=PIM4
            p2=0.0_wp
          
          do j=1,n
            p3=p2
            p2=p1
            p1=z*sqrt(2.0_wp/j)*p2-sqrt(real(j-1,wp)/real(j,wp))*p3
          end do!j
          
          pp=sqrt(2.0_wp*n)*p2
          z1=z
          z=z1-p1/pp
          
          end if
        end do !its
          
     if (abs(z-z1) > EPS) then
        write(*,*) 'too many iterations in gauher'
     end if
     
        x(k)=z
        x(n+1-k)=-z
        w(k)=2.0_wp/(pp*pp)
        w(n+1-k)=w(k)     
     
end do!k

end subroutine gauher
