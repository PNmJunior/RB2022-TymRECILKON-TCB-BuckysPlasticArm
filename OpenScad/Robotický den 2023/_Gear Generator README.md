# gears.scad

In order to use this code, you will need to copy gears.scad to your Libraries folder.
https://en.wikibooks.org/wiki/OpenSCAD_User_Manual/Libraries

And then you can run *Untitled.scad* as any other normal scad project file.

If you don't want to mess around with the library, just combine the two code files together into one.

## Library Functions

This code will provide a function

**spur_gear()**

Which takes the following parameters:
- N = Number of teeth
- P = Diametral Pitch (defaults to 12)
- pa = Pressure Angle (defaults to 14.5)

The output of this method will be a 2D gear (converted to mm) which you simply need to *linear_extrude* in order to print on a 3D printer.

The library also includes a number of other mathematical functions including:

**calc_center_distance()**

Which takes the following parameters:
- N1 = Tooth count of the first gear
- N2 = Tooth count of the second gear
- P = Diametral Pitch

The output of this function will be the mm distance you must space both gears for them to mesh perfectly.


![picture of the openscad output](https://i.imgur.com/W8r2YJl.png "A gear generated with this script")