//Kleste();
//Redukce();

//KlestePrava();
//Simul();


Print3();


module Print3()
{
    translate([0,0,0]) rotate([0,0,0])
    {
 translate([0,0,0]) rotate([-90,0,0]) KlesteLeva();
translate([0,-40,0]) rotate([90,0,0]) KlestePrava(); 
    }  
}

//Print2();


module Print2()
{
    translate([0,0,0]) rotate([0,0,0])
    {
 KlesteLeva();
translate([0,-40,0]) KlestePrava(); 
    }  
}


//Print();


module Print()
{
    translate([0,0,35]) rotate([0,160,0])
    {
 KlesteLeva();
translate([0,-40,0]) KlestePrava(); 
    }  
}


module Simul()
{
 KlesteLeva();
translate([0,-30-$t*45,0]) KlestePrava();   
    translate([11,-52,-20])rotate([0,0,45])  cube(40);
    //translate([23,-62,-20])rotate([0,0,45])  cube(40);
    //translate([0,-45,-20])  cube(40);    
}


module KlesteLeva()
{    
    translate([-28,-2,42]) rotate([0,20,0])
   { rotate([0,0,-20]) Redukce();}
   mirror([0,1,0])translate([0,0,0]) Kleste();
}


module KlestePrava()
{    
    translate([-28,2,42]) rotate([0,20,0])
   {rotate([180,0,20]) Redukce();}
   translate([0,0,0]) Kleste();    
}


module Kleste()
{
    c = 10;
    b = 8;
    bb = b-1;
    a= c+b/2;
    dd = 10;
    
    j=20+b/3;
    v = 20;
    translate([0,0,0])
    {
    translate([0,0,0])
    {
  translate([0,0,0]) sphere(d= dd);
    translate([0,0,0]) rotate([90,0,0]) cylinder(h=v,d=bb,center = false);
        translate([0,0,0]) rotate([0,180,0]) cylinder(h=12,d1=dd,d2=0,center = false);
    }
        translate([j,0,0])
    {
    translate([0,0,0]) sphere(d= dd);
    translate([0,0,0]) rotate([90,0,0]) cylinder(h=v,d=bb,center = false);
        translate([0,0,0]) rotate([0,180,0]) cylinder(h=12,d1=dd,d2=0,center = false);
    }
            translate([-j,0,0])
    {
    translate([0,0,0]) sphere(d= dd);
    translate([0,0,0]) rotate([90,0,0]) cylinder(h=v,d=bb,center = false);
        translate([0,0,0]) rotate([0,-90,0]) cylinder(h=15,d1=dd,d2=0,center = false);
        translate([0,0,0]) rotate([0,180,0]) cylinder(h=12,d1=dd,d2=0,center = false);
    }
            translate([2*j,0,0])
    {
    translate([0,0,0]) sphere(d= dd);
    translate([0,0,0]) rotate([90,0,0]) cylinder(h=v,d=bb,center = false);
    translate([0,0,0]) rotate([0,90,0]) cylinder(h=15,d1=dd,d2=0,center = false);
        translate([0,0,0]) rotate([0,180,0]) cylinder(h=12,d1=dd,d2=0,center = false);
    }
}
translate([j+j+bb/2,-v,0]) rotate([0,-90,0]) cylinder(h=bb+3*j,d=bb,center = false);

alfa=15;
    translate([0,-v,-4])  cylinder(h=40,d=10,center = false);
translate([0,alfa,30]) rotate([90,0,0]) cylinder(h=alfa+v,d=10,center = false);
}


module Redukce()
{
    translate([0,0,-2.5/2]) difference()
    {
        union()
        {
            translate([3,0,0]) cylinder(h=2.5+1,d=6,center=false,$fn=64);
            translate([3,0,-1]) cylinder(h=1,d=5,center=false,$fn=64);   
            translate([0,-3,0]) cube([30,15,2.5]);
            translate([19,0,-0.5]) cylinder(h=3.5,d=6,center=false,$fn=64);
            translate([19,0,-1]) cylinder(h=4.5,d=5,center=false,$fn=64);
        }
        translate([3,0,-1.5]) cylinder(h=10,d=3,center=false,$fn=64);
        translate([19,0,-1.5]) cylinder(h=10,d=3,center=false,$fn=64);   
    }
}
