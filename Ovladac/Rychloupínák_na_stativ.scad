












//Sparně navrženo...
uhel = 45;
vyska = 9;
delniRozAbs = 41.9;
delniRozReal = 38;

dir_roztec = 5;
dir_az = 10;
dir_vel = 3.2;

podlozka = 5;
vyrez = 15;
vyrez_vyska = 3.5;

rych();

module rych()
{
    difference()
    {
        translate() intersection()
        {
            translate([0,0,0]) cube([delniRozAbs,delniRozAbs,vyska]);
            translate([0,0,0]) rotate([0,uhel,0]) cube([delniRozAbs,delniRozAbs,1000]);
            translate([0,0,0]) rotate([-uhel,0,0]) cube([delniRozAbs,delniRozAbs,1000]);
            translate([(delniRozAbs - delniRozReal)/2,(delniRozAbs - delniRozReal)/2,0]) cube([delniRozReal,delniRozReal,vyska]);
        }
        translate([0,delniRozAbs,0]) rotate([uhel,0,0]) cube([delniRozAbs,delniRozAbs,1000]);
        translate([delniRozAbs,0,0]) rotate([0,-uhel,0]) cube([delniRozAbs,delniRozAbs,1000]);
        translate([delniRozAbs/2,delniRozAbs/2,0])
        {
            for(x = [-dir_az : dir_roztec  :dir_az])
            {
                for(y = [-dir_az : dir_roztec  :dir_az])
                {
                    translate([x,y,0]) cylinder(h=100, d=dir_vel, center=true,$fn=30);
                    //translate([x,y,0]) cylinder(h=6, d=10, center=true,$fn=30);
                }
            }
            translate([-vyrez,-vyrez,-vyrez_vyska]) OvalnaKrichleZ([vyrez*2,vyrez*2,vyrez_vyska*2],podlozka,50);
        }
        //translate([0,30,0]) cube([100,100,100]);
    }
}

//Kód pro vytvoření oválných kvádrů
module OvalnaKrichleX(Rozdmery, zakulaceni)
{
	union() {
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1] - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([0,zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,Rozdmery[1] - zakulaceni,zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	translate([0,Rozdmery[1] - zakulaceni,Rozdmery[2] - zakulaceni]) rotate([0,90,0]) cylinder( Rozdmery[0],   zakulaceni, zakulaceni,false,$fn=10);
	}
}


module OvalnaKrichleY(Rozdmery, zakulaceni)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,0,zakulaceni]) cube([Rozdmery[0],Rozdmery[1],Rozdmery[2] - zakulaceni * 2],center=false);
	translate([zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,0,zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,0,Rozdmery[2] - zakulaceni]) rotate([270,0,0]) cylinder( Rozdmery[1],   zakulaceni, zakulaceni,false,$fn=10);
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


module OvalnaKrichle(Rozdmery, zakulaceni, osaX, osaY, osaZ)
{
	cont = false;
	intersection()
	{
		if(osaX == true)
		{
			OvalnaKrichleX(Rozdmery,zakulaceni);
			cont = true;
		}
		if(osaY == true)
		{
			OvalnaKrichleY(Rozdmery,zakulaceni);
			cont = true;
		}
		if(osaZ == true)
		{
			OvalnaKrichleZ(Rozdmery,zakulaceni);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}


module OvalnaKrichleCisla(Rozdmery, osaX, osaY, osaZ)
{
	cont = false;
	intersection()
	{
		if(osaX != 0)
		{
			OvalnaKrichleX(Rozdmery,osaX);
			cont = true;
		}
		if(osaY != 0)
		{
			OvalnaKrichleY(Rozdmery,osaY);
			cont = true;
		}
		if(osaZ != 0)
		{
			OvalnaKrichleZ(Rozdmery,osaZ);
			cont = true;
		}
		if(cont == false)
		{
			cube(Rozdmery,center=false);
		}
	}
}

