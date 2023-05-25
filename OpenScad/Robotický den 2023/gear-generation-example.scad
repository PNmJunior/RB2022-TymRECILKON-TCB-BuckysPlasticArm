include <gears.scad>

Teeth = 18;
Pitch = 12;

linear_extrude(6)
difference() {
    spur_gear(N=Teeth, P=Pitch);
    circle(d=19.1);
}
// Change this variable to calculate how far
// this gear and another gear need to be spaced
// in order to mesh properly. 
// (output will display in the console in units mm)
OtherGearTeeth = 37;
echo("***");
echo("center distance (mm)", calc_center_distance(Teeth, OtherGearTeeth, Pitch));
echo("***");