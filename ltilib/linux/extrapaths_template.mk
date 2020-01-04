# Example extrapath.mk file.
#
# You can just rename this file in extrapath.mk and edit your options.
#
# The default variables are empty
EXTRACXXFLAGS=
EXTRAINCLUDEPATH=
EXTRALIBPATH=
EXTRALIBS=

# ToUCam
# EXTRACXXFLAGS=-D_USE_PHILIPS_TOUCAM

# Firewire DCAM
# EXTRACXXFLAGS=-D_USE_FIRE_WIRE_DCAM
# EXTRALIBS=-ldc1394_control -lraw1394

# Pulnix + MicroEnable
# EXTRAINCLUDEPATH = -I/usr/src/menable/include
# EXTRALIBPATH = -L/usr/src/menable/lib
# EXTRALIBS =  -lpulnixchanneltmc6700 -lmenable
