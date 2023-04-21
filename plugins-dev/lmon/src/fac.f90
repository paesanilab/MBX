module fac

use globals

contains

real(wp) function factorial(n)

implicit none

integer, intent(in) :: n

integer :: j

real(wp) :: f

f=1.0_wp

if (n>1) then
   do j=1,n
      f = f*j
   end do
end if

factorial=f

end function factorial

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

recursive real(wp) function hermite(n,x)

implicit none

integer, intent(in) :: n

real(wp), intent(in) :: x

real(wp) :: herm

if (n==0) then
    herm = 1.0_wp
else if (n==1) then
    herm = 2.0_wp*x
else if (n==2) then
    herm = 4.0_wp*x**2.0_wp-2.0_wp
else if (n==3) then
    herm = 8.0_wp*x**3.0_wp-12.0_wp*x
else if (n==4) then
    herm = 16.0_wp*x**4.0_wp-48.0_wp*x**2.0_wp+12.0_wp
else 
    herm = 2.0_wp*x*hermite(n-1,x)-2.0_wp*(n-1)*hermite(n-2,x)
end if

hermite = herm

end function hermite

end module fac
