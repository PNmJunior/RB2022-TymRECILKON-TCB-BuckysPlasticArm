AvyskaNakPros = 60;
ARozdNakPros = 105;//100fotka//80dokumentace
AvyskaAuta = 120; //Nevym
AhloubkaNakPros = AvyskaNakPros; //Neni podstatne
AdelkaAuta = 220;
AsirkaAuta = 120;





module Auto(vyskaNakPros,RozdNakPros ,vyskaAuta ,hloubkaNakPros,delkaAuta , sirkaAuta)
{
    stenaNakPros = (sirkaAuta - RozdNakPros)/2;
    stenaMoje = 5;
    zaobleni = 3;//moje
    prestnost = 20;
    difference()
    {
        translate([0,0,0])
        {
            OvalnaKrichleCisla([delkaAuta,sirkaAuta,vyskaNakPros], zaobleni, zaobleni, zaobleni,prestnost);
            translate([RozdNakPros + stenaNakPros,0,0]) OvalnaKrichleCisla([delkaAuta - (RozdNakPros + stenaNakPros),sirkaAuta,vyskaAuta], zaobleni, zaobleni, zaobleni,prestnost);
        }
        translate([stenaNakPros,stenaNakPros,vyskaNakPros - hloubkaNakPros]) OvalnaKrichleCisla([RozdNakPros,RozdNakPros,vyskaAuta], zaobleni, zaobleni, zaobleni,prestnost);
        translate([stenaNakPros + RozdNakPros + stenaMoje,stenaMoje,-5]) OvalnaKrichleCisla([delkaAuta - stenaNakPros - RozdNakPros - stenaMoje*2,sirkaAuta - stenaMoje*2,vyskaAuta + 10], 0, 0, zaobleni,prestnost);
    }
    
}

translate([0,0,0]) Auto(AvyskaNakPros,105 ,AvyskaAuta ,AhloubkaNakPros,AdelkaAuta , AsirkaAuta);//v1
translate([0,AsirkaAuta + 10,0]) Auto(AvyskaNakPros,80 ,AvyskaAuta ,AhloubkaNakPros,AdelkaAuta , AsirkaAuta);//v2



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

