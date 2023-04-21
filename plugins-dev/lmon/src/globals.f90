!-----------------------------------------------------------------------
!
!  Globals module.  Put all variables to be accessible everywhere
!    here. 
!
!-----------------------------------------------------------------------

module globals

implicit none

  ! this sets the floating-point precision to double; basically
  ! it says "give me the precision needed to get 14 decimal places,
  ! and call it wp".  From now on, we declare variables to be of
  ! type wp.  You can change to "p=4" for single precision, but
  ! for now to be safe we will stick to doubles.

  integer, parameter  :: wp = selected_real_kind(p=14)
  
  real(wp), parameter :: pi = 3.1415926535897_wp  ! PI
  
  !real(wp), parameter :: h = 6.626068e-34_wp      ! Plank constant
  
  !real(wp), parameter :: c = 2.99792458e8_wp      ! speed of light
  
  ! 'parameter' here means that the value of the variable can't be changed
  complex(wp), parameter :: i = (0.0_wp, 1.0_wp)  ! sqrt (-1)
  
end module globals
