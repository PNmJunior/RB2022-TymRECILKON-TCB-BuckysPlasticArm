module pieSlice(a, r, h){
  // a:angle, r:radius, h:height
  rotate_extrude(angle=a) square([r,h]);
}
//pieSlice(110,20,3);

    motorKul = 54;//53.8
    motorVyska = 40.2;//40.1
    motorSirka  = 29.3;
    motorDiraVel = 2;
    motorDiraY = 36;
    motorDiraX = 31;
    motorVM = 25.4;
    motorVV = 33.6;
    motorVS = 5.2;
    motorVelkeUhel = 45;
    motorVelke = 41;//40.5
    motorMale = 36.5;//36.1
    motorAsimetrie = (motorVV - motorVS/2)-motorKul/2;
$fn=50;

module motor()
{

    echo(motorAsimetrie);
    $fn=50;
    
    rotate([0,0,180]) translate([-motorVyska/2,-motorAsimetrie,]) 
    {
        difference()
        {
            cylinder(motorSirka, d = motorKul, $fn=50);
            translate([-(motorVyska/2+40),-motorKul/2,0]) cube([40,motorKul,motorSirka]);
            translate([motorVyska/2,-motorKul/2,0]) cube([40,motorKul,motorSirka]);
            
        }
        translate([-motorDiraX/2, - motorDiraY/2,-50 + motorSirka/2])
        {
            translate([0,0,0]) cylinder(h = 100, d = motorDiraVel, $fn=50);
            translate([motorDiraX,0,0]) cylinder(h = 100, d = motorDiraVel, $fn=50);
            translate([0,motorDiraY,0]) cylinder(h = 100, d = motorDiraVel, $fn=50);
            translate([motorDiraX,motorDiraY,0]) cylinder(h = 100, d = motorDiraVel, $fn=50);
        }
        translate([0,motorAsimetrie,-50 + motorSirka/2])
        {
            cylinder(h = 100, d = motorMale, $fn=50);
            rotate([0,0,-90 + motorVelkeUhel/2]) pieSlice(180-motorVelkeUhel, motorVelke/2, 100);
            rotate([0,0,180-90 + motorVelkeUhel/2]) pieSlice(180-motorVelkeUhel, motorVelke/2, 100);
        }
        
    }
    
}

//motor();

module troj()
{
    translate([-40,40,0]) rotate([0,0,-35]) cube([40,100,30]);
    
    
}




module nosnik()
{
    vyska = 4;
    vyskaVyrez = 3;
    difference()
    {
        translate([0,0,0])
        {
            translate([0,16,0]) OvalnaKrichleZ([50,40,4],2);
            
            
        }
        translate([(50-motorVyska)/2,0,vyskaVyrez]) motor();
        translate([0,0,-10])
        {
            translate([25,40,0]) cylinder(h = 20, d = 3);
            translate([25,50,0]) cylinder(h = 20, d = 3);
            translate([15,35,0]) cylinder(h = 20, d = 3);
            translate([35,35,0]) cylinder(h = 20, d = 3);
        }
        
        translate([0,0,-10])
        {
            troj();
            mirror([1,0,0]) translate([-motorVyska-10,0,0]) troj();
        }   
    }
}

translate([0,0,0]) nosnik();
translate([60,0,0]) nosnik();

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