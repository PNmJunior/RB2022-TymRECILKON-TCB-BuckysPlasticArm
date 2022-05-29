kolo(70,4);//Kolo4mm4krat
//kolo(70,6);//Kolo6mm4krat
//kolo(70,10,6);//Kolo10mm4krat


module kolo(velikost,vyska, pocLamel = 5)
{
    alfa = vyska;
    difference() 
    {
        union(){
            translate([0,0,0])cylinder(h=14, d=10,$fn=64);  
            for(i=[1:pocLamel])
                {
                    if(alfa >= 4)
                    {
                        alfa=4;
                        echo(alfa);
                        translate([0,0,0]) rotate([0,0,i*(360/pocLamel)]) OvalnaKrichleX([velikost/2 - 3-1,4,alfa],1); 
                       
                    }
                    
                }
        }
         
        translate([0,0,-pad]) shaft();    
    }

    difference() 
    {
        //translate([0,0,0])cylinder(h=4, d=velikost,$fn=64);
        translate([0,0,0])ZaobCylinder(velikost,vyska,1,1);
        //translate([0,0,-0.5])cylinder(h=5, d=velikost-10,$fn=64);    
          translate([0,0,-0.5])ZaobCylinder(velikost-10,vyska+1,2,-2);    
    }
}
module shaft()
 {
     a=3.8;
     difference() 
     {
        cylinder(h=165, d=5.6,$fn=64); 
        translate([-2.5,-a,0])cube([5, 2, 15]);
        translate([-2.5,4-2,0])cube([5, 2, 15]); 
     }   
 }
 
 
 module ZaobCylinder(velikost,vyska,vyskaZaob,rozdil)
 {
     
     translate([0,0,0])cylinder(h=vyskaZaob, d1=velikost-rozdil, d2=velikost,$fn=64);
     translate([0,0,vyskaZaob])cylinder(h=vyska-2*vyskaZaob, d=velikost,$fn=64);
     translate([0,0,vyska-vyskaZaob])cylinder(h=vyskaZaob, d2=velikost-rozdil, d1=velikost,$fn=64);
     
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


module OvalnaKrichleZ(Rozdmery, zakulaceni)
{
	union() {
	translate([zakulaceni,0,0]) cube([Rozdmery[0] - zakulaceni * 2,Rozdmery[1],Rozdmery[2]],center=false);
	translate([0,zakulaceni,0]) cube([Rozdmery[0],Rozdmery[1]  - zakulaceni * 2,Rozdmery[2]],center=false);
	translate([zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
	translate([Rozdmery[0] - zakulaceni,Rozdmery[1] - zakulaceni,0]) rotate([0,0,0]) cylinder( Rozdmery[2],   zakulaceni, zakulaceni,false,$fn=10);
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

   