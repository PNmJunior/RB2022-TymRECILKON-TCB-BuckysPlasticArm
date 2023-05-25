include<gears.scad>


vyrezTyckyVerky = 5.6;
vyrezTyckyMaly = 3.9;

module vyrezTycky(vyrezTyckyVyska = 10, vyrezTyckyPocatecniVyska = -1, vyrezTyckyVelikostSterbiny = 1)
{

    translate([0,0,vyrezTyckyPocatecniVyska]) 
    {
        difference()
        {
            cylinder(vyrezTyckyVyska, d = vyrezTyckyVerky, $fn=50);
            translate([-(vyrezTyckyMaly/2+4),-vyrezTyckyVerky/2,0]) cube([4,vyrezTyckyVerky,vyrezTyckyVyska]);
            translate([vyrezTyckyMaly/2,-vyrezTyckyVerky/2,0]) cube([4,vyrezTyckyVerky,vyrezTyckyVyska]);
            
        }
        if(vyrezTyckyVelikostSterbiny != 0)
        {
            vyrezTyckyAlfa = vyrezTyckyVerky + vyrezTyckyVelikostSterbiny*2;
            vyrezTyckyBeta = 0.1;
            translate([-vyrezTyckyBeta/2,-vyrezTyckyAlfa/2,0]) cube([vyrezTyckyBeta,vyrezTyckyAlfa,vyrezTyckyVyska]);
            
        }
    }
}

function vyrezTyckySitka(vyrezTyckyVelikostSterbiny = 1) = vyrezTyckyVerky + 2*vyrezTyckyVelikostSterbiny;


//vyrezTycky();

module Vyrezy(pocetDer,vyrezTyckyVyska = 10, vyrezTyckyPocatecniVyska = -1, vyrezTyckyVelikostSterbiny = 1)
{
    vyrezTycky(vyrezTyckyVyska, vyrezTyckyPocatecniVyska, vyrezTyckyVelikostSterbiny);
    translate([0,0,-1])
    {
        for( i =[20:10:10+pocetDer*10])
        {
            translate([i,0,0]) cylinder(h =  vyrezTyckyVyska, d = 3, $fn=50);
        }
    }
    translate([20,0,3]) cylinder(h =  vyrezTyckyVyska, d = 15, $fn=50);
}
//Vyrezy(2);

module kolecko(vyskaKolo = 6)
{
    difference()
    {
        resize([34.4,34.4,vyskaKolo])linear_extrude(6) spur_gear(N = 15, p = 12,pa=14.2);
        cylinder(h = vyskaKolo, r = 9);
    }
}
 //kolecko();
//translate([30,0,0]) rotate([0,0,11]) kolecko();

module nosnik(pocetDer, kolo, vyska = 3, vyskaTycky = 10, vyskaKola = 6)
{
    $fn=50;
    difference()
    {
        translate([0,0,0])
        {
            cylinder( h = vyska, d = 10);
            cylinder( h = vyskaTycky, d = vyrezTyckySitka()+3);
            translate([0,-5,0]) OvalnaKrichleZ([23-10+pocetDer*10,10,vyska],2);
            if(kolo == 1)
            {
                kolecko(vyskaKola);
            }
            if(kolo == 2)
            {
               rotate([0,0,0]) kolecko(vyskaKola);
            }
            if(kolo != 0)
            {
                translate([-10,-2,0]) cube([20,4,vyskaKola]);
                translate([-2,-10,0]) cube([4,20,vyskaKola]);
            }
            
        }
        
        Vyrezy(pocetDer, vyrezTyckyVyska =  vyskaTycky +2);
    }
    
    
    
}

//Horni 1.nosnik;
translate([20,15,0]) nosnik(2,0);

//Horni 2.nosnik;
translate([20,30,0]) nosnik(2,0);
//intersection()
//{
//Dolni 1.nosnik;
translate([0,0,0]) nosnik(3,1);

//Dolni 2.nosnik;
translate([0,50,0]) nosnik(3,2);

//}

//Kód pro vytvoření oválných kvádrů
module OvalnaKrichleX(Rozdmery, zakulaceni,st = 10)
{
	union() {
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1] - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([0,zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=st);
	translate([0,Rozdmery[1] - zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=st);
	translate([0,zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=st);
	translate([0,Rozdmery[1] - zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=st);
	}
}


module OvalnaKrichleY(Rozdmery, zakulaceni,st = 10)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=st);
	translate([Rozdmery[0] - zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=st);
	translate([zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=st);
	translate([Rozdmery[0] - zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=st);
	}
}


module OvalnaKrichleZ(Rozdmery, zakulaceni,st = 10)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1]  - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=st);
	translate([Rozdmery[0] - zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=st);
	translate([zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=st);
	translate([Rozdmery[0] - zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=st);
	}
}


module OvalnaKrichle(Rozdmery, zakulaceni, osaX, osaY, osaZ, st = 10)
{
	cont = false;
	intersection()
	{
		if(osaX == true)
		{
			OvalnaKrichleX(Rozdmery,zakulaceni,st);
			cont = true;
		}
		if(osaY == true)
		{
			OvalnaKrichleY(Rozdmery,zakulaceni,st);
			cont = true;
		}
		if(osaZ == true)
		{
			OvalnaKrichleZ(Rozdmery,zakulaceni,st);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}


module OvalnaKrichleCisla(Rozdmery, osaX, osaY, osaZ ,st = 10 )
{
	cont = false;
	intersection()
	{
		if(osaX != 0)
		{
			OvalnaKrichleX(Rozdmery,osaX,st);
			cont = true;
		}
		if(osaY != 0)
		{
			OvalnaKrichleY(Rozdmery,osaY,st);
			cont = true;
		}
		if(osaZ != 0)
		{
			OvalnaKrichleZ(Rozdmery,osaZ,st);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}