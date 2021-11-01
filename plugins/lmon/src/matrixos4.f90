module matrixos4

use globals

contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! the determinant of a square n by n matrix A (complex)

complex(wp) function det(A)

implicit none

complex(wp), dimension(:,:), intent(in):: A

complex(wp), dimension(:,:), allocatable:: B

integer, dimension(size(A,1)):: ipiv

integer :: n,info, j

complex(wp):: determinant

n=size(A,1)

allocate(B(n,n))

B(1:n,1:n)=A

call ZGETRF(n,n,B(1:n,1:n),n,ipiv,info)

if (info .ne. 0) then
    write(*,*) 'illegal argument value or singular matrix', info,'det'
    determinant =0.0_wp+i*0.0_wp
else
    
determinant = 1.0_wp+i*0.0_wp

do j=1,n
if (ipiv(j).ne.j) then
determinant = -determinant * B(j,j)
else
determinant = determinant * B(j,j)
end if
end do

end if

deallocate(B)

det=determinant

end function det


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! the trace of a square n by n matrix A (complex)

complex(wp) function trace(A)

implicit none

complex(wp), dimension(:,:), intent(in) :: A

integer :: n,j

complex(wp):: tr

tr=0.0_wp+i*0.0_wp

n=size(A,1)

do j=1,n
tr=tr+A(j,j)
end do

trace=tr

end function trace

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

! inverse of a matrix
subroutine inverse(A,B)

implicit none

complex(wp),dimension(:,:), intent(in):: A

complex(wp),dimension(size(A,1),size(A,2)), intent(out):: B

complex(wp) :: work(5*size(A,1))

complex(wp), dimension(size(A,1),size(A,2)):: U, Vt

real(wp) :: S(size(A,1)), Rwork(5*size(A,1))

integer :: j, k, n, info, ipiv(size(A,1))

B=A

n=size(A,1)

call ZGETRF(n,n,B,n,ipiv,info)

if (info .ne. 0) then
write(*,*) 'illegal argument value or singular matrix', info, 'ZGETRF'

B=A

call ZGESVD('A','A',n,n,B,n,S,U,n,Vt,n,work,5*n,Rwork,info)

if (info .ne. 0) then
    write(*,*) 'illegal argument value or singular matrix', info,'ZGESVD'
end if

do j=1,n
   do k=1,n
      B(j,k)=0.0_wp+i*0.0_wp
   end do
end do


do j=1,n
   call ZGERC(n,n,S(j)/(S(j)**2.0_wp+1.0E-12_wp)+i*0.0_wp,conjg(Vt(j,1:n)),1,U(1:n,j),1,B,n)
end do

else

call ZGETRI(n,B,n,ipiv,work,n,info)

end if



end subroutine inverse


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
complex(wp) function ZDOT(v1,v2)

implicit none

complex(wp), dimension(:),intent(in) :: v1,v2

integer :: n,j

complex(wp) :: dd

n=size(v1)

dd=0.0_wp

do j=1,n
   dd=dd+v1(j)*v2(j)
end do

ZDOT=dd

end function ZDOT
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! the determinant of a square n by n matrix A (real)

real(wp) function realdet(A)

implicit none

real(wp), dimension(:,:), intent(in):: A

real(wp), dimension(:,:), allocatable:: B

integer, dimension(size(A,1)):: ipiv

integer :: n, info, j

real(wp):: determinant

n=size(A,1)

Allocate(B(n,n))

B(1:n,1:n)=A

call DGETRF(n,n,B(1:n,1:n),n,ipiv,info)

if (info .ne. 0) then
   write(*,*) 'illegal argument value or singular matrix', info, 'realdet'
   determinant = 0.0_wp
   
else

determinant = 1.0_wp

do j=1,n
if (ipiv(j).ne.j) then
determinant = -determinant * B(j,j)
else
determinant = determinant * B(j,j)
end if
end do

end if

!if (determinant < 1.0E-60_wp) then
!determinant = 1.0E-60_wp
!end if

deallocate(B)

realdet=determinant

end function realdet

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! the trace of a square n by n matrix A (real)

real(wp) function realtrace(A)

implicit none

real(wp), dimension(:,:), intent(in) :: A

integer :: n,j

real(wp):: tr

n=size(A,1)

tr=0.0_wp

do j=1,n
tr=tr+A(j,j)
end do

realtrace=tr

end function realtrace

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! inverse of a matrix
subroutine realinverse(A,B)

implicit none

real(wp),dimension(:,:), intent(in):: A

real(wp),dimension(size(A,1),size(A,2)), intent(out):: B

real(wp) ::S(size(A,1)), work(5*size(A,1))

real(wp), dimension(size(A,1),size(A,2)):: U, Vt

integer :: j, k, n, info, ipiv(size(A,1))

B=A

n=size(A,1)

call DGETRF(n,n,B,n,ipiv,info)

if (info .ne. 0) then
write(*,*) 'illegal argument value or singular matrix', info, 'DGETRF'

B=A

call DGESVD('A','A',n,n,B,n,S,U,n,Vt,n,work,5*n,info)

if (info .ne. 0) then
    write(*,*) 'illegal argument value or singular matrix', info,'DGESVD'
end if

do j=1,n
   do k=1,n
      B(j,k)=0.0_wp
   end do
end do


do j=1,n
   call DGER(n,n,S(j)/(S(j)**2.0_wp+1.0E-12_wp),Vt(j,1:n),1,U(1:n,j),1,B,n)
end do

else

call DGETRI(n,B,n,ipiv,work,n,info)

end if

end subroutine realinverse


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
real(wp) function DDOT(v1,v2)

implicit none

real(wp), dimension(:),intent(in) :: v1,v2

integer :: n,j

real(wp) :: dd

n=size(v1)

dd=0.0_wp

do j=1,n
   dd=dd+v1(j)*v2(j)
end do

DDOT=dd

end function DDOT

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   

end module matrixos4
