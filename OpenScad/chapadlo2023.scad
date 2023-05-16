//Kleste();
//Redukce();
KlestePrava();
//Simul();


//Print5();


module Print5()
{
    difference()
    {
    translate([0,0,22]) rotate([0,0,0])
    {
        translate([0,0,0]) rotate([-90,0,0]) KlesteLeva();
        translate([0,-40,0]) rotate([90,0,0]) KlestePrava(); 
    } 
   translate([-100,-100,-200]) cube(200); 
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
    { 
        rotate([0,0,-20]) Redukce();
    }
    mirror([0,1,0])translate([0,0,0]) Kleste();
}


module KlestePrava()
{    
    translate([-31,62,95]) rotate([0,10,0])
    {
        rotate([180,0,20]) Redukce();
    }
    translate([0,0,0]) Kleste();    
}

//lista();
module lista(sirkaBLista = 20, vyskaCelkLista = 50, delkaLista = 100)
{
    
    vyskaLista = 10;
    vyskaVyrezLista = 20;
    sirkaLista = 10;
    sirkaLista2 = sirkaLista/sqrt(2);
    translate([0,0,sirkaLista/2]) difference()
    {
        translate([0,0,0])
        {
            translate([0,0,0]) cube([delkaLista,sirkaBLista,vyskaCelkLista - sirkaLista/2]);
            translate([0,sirkaLista/2,-sirkaLista/2]) rotate([45,0,0]) cube([delkaLista,sirkaLista2,sirkaLista2]);
            
        }
        translate([0,4,0])  cube([delkaLista,sirkaBLista,vyskaVyrezLista]);
        translate([0,-5,vyskaVyrezLista + 5])  cube([delkaLista,sirkaBLista,vyskaCelkLista-vyskaVyrezLista - vyskaLista/2-10]);
    }
    
    
}

module Kleste()
{

    v = 20;

    alfa=15;
    
    delkaList = 100;
    //translate([0,-v,-4])  cylinder(h=40,d=10,center = false);
    translate([0,0,90]) rotate([-90,0,0]) cylinder(h=25+30+20,d=10,center = false);
    translate([-delkaList/2,0,0]) lista(20,55,delkaList);
    translate([-delkaList/2,30,25]) lista(15,30,delkaList);
    translate([-5,0,30]) cube([10,30,60]);
    translate([-5,0,50]) cube([10,45,40]);
}


module Redukce()
{
    translate([0,0,-2.5/2]) difference()
    {
        union()
        {
            translate([3,0,0]) cylinder(h=2.5+1,d=6,center=false,$fn=64);
            translate([3,0,-1]) cylinder(h=1,d=5,center=false,$fn=64);   
            translate([0,-3,0]) cube([30,40,2.5]);
            translate([20,0,-0.5]) cylinder(h=3.5,d=6,center=false,$fn=64);
            translate([20,0,-1]) cylinder(h=4.5,d=5,center=false,$fn=64);
        }
        translate([3,0,-1.5]) cylinder(h=10,d=3,center=false,$fn=64);
        translate([20,0,-1.5]) cylinder(h=10,d=3,center=false,$fn=64);   
    }
}
