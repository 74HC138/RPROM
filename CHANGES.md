# What this fork changes

## Always select slot 1

The RPROM always chooses slot 1. This is the rom slot for the boot menu.

*Unless* a command to change the active rom slot has been received prior to the reset

## Additional field in status command

The RPROM not only reports the currently loaded rom slot but also the saved default slot

## Advancing version

Changed version to 1.2.0

# What this fork adds

## Bootmenu for the host amiga

By holding down the left mouse button of joystick port 1 after a reset the boot menu rom in rom slot 1 will present a menu where the rom to use can be selected.

The selected rom will then automatically be booted when the boot menu is not invoked until a different one is selected

## Dummy ROM

During SRAM ROM updates the RPROM now shows a small 256 byte dummy rom mirrored over the entire address space that during reset just boot loops.
This also makes the system more resilient at bootup since it can quickly show a bootable rom before its done loading the rom from flash

## Soft ROM switchover

When the SRAM ROM is done updating the RPROM will continue to show the dummy rom until the host reads the rom in a manner that would happen on reset or the dummy rom bootlooping to the point where the RPROM can savely switch over to the SRRAM ROM image. 