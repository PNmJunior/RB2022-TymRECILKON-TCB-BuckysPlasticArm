


pena = 10;
vrstva = 3;
rameno = 23;
kostka = 60;
alfa=15;

$fn=30;
//tiskBool = false;
tiskBool = true;

//Kleste();
//Redukce();

//KlestePrava();
//Simul();

//Print4();

//Print5();


module tisk()
{
    
    translate([0,45,0]) rotate([-90,0,0]) KlesteLeva();
    translate([0,0,0]) rotate([90,0,0]) KlestePrava(); 
    
}
tisk();

module Print4()
{
    difference()
    {
    translate([0,0,00]) rotate([0,270,0]) 
    {
 KlesteLeva();
translate([0,-40,0]) KlestePrava(); 
    } 
    translate([-50,-70,-128]) cube(100);
    } 
}


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
    //i = 0;
    i = $t;
    kk = alfa+i*rameno;
    translate([0,kk,40]) KlesteLeva();
    //translate([0,-30-$t*2*rameno,0]) KlestePrava();  
   translate([0,-kk,40]) KlestePrava();   
    //translate([11,-52,-20])rotate([0,0,45])  cube(40);
    //translate([23,-62,-20])rotate([0,0,45])  cube(40);
    translate([0,-kostka/2,0]) color("black",0.5) cube(kostka);    
}

posun = 30;
module KlesteLeva()
{   difference()
   { 
    translate([0,0,0])
        {
        translate([-28+posun,-2,42]) rotate([0,20,0])
            { 
                rotate([0,0,-20]) Redukce();

            }
            mirror([0,1,0])translate([0,0,0]) Kleste();
            
        }
        translate([0,alfa ,0]) cube(100);
    }
}


module KlestePrava()
{    
    difference()
    {
        translate([0,0,0])
        {
            translate([-28+posun,2,42]) rotate([0,20,0])
            {
                rotate([180,0,20]) Redukce();
            }
            translate([0,0,0]) Kleste(); 
        }   
        translate([0,-alfa-100 ,0]) cube(100);
    }
}


module plocha(rozd)
{
    mezera = 2.5;
    odx = 10;
    odxS = (rozd[0]-odx*3)/4;
    odxSO = odxS + 3;
    odz = 10;
    odzS = 10;
    odzSO = odzS + 3;
    difference()
    {
        OvalnaKrichleY(rozd,2);
        //pro osu y
        for(x = [mezera:mezera:rozd[0] - mezera])
        {
            translate([x,rozd[1]*2,rozd[2]-mezera]) rotate([90,0,0]) cylinder(h = 100,d = 1);
            translate([x,rozd[1]*2,mezera]) rotate([90,0,0]) cylinder(h = 100,d = 1);
        }
        for(z = [mezera:mezera:rozd[2] - mezera])
        {
            translate([rozd[0]-mezera,rozd[1]*2,z]) rotate([90,0,0]) cylinder(h = 100,d = 1);
            translate([mezera,rozd[1]*2,z]) rotate([90,0,0]) cylinder(h = 100,d = 1);
        }
        
        for(x = [odx:odxSO:rozd[0] - odx])
        {
            for(z = [odz:odzSO:rozd[2] - odz])
            {
                translate([x,-10,z]) OvalnaKrichleY([odxS,100, odzS],2);
                
            }
            
        }
    }
    
    
}

module Kleste()
{

    v = pena + vrstva;
    vyskaCenter = 20;
    vyskaPlochy = 40;
    sirkaPlochy = 60;
    rezer = 10;
    odstup = sirkaPlochy + 40;

    //translate([j+j+bb/2-2,-v,0]) rotate([0,-90,0]) cylinder(h=bb+3*j,d=bb,center = false);

    
    //translate([0,-v,-4])  cylinder(h=40,d=10,center = false);
    translate([0,alfa,0]) 
    {
        //rotate([90,0,0]) cylinder(h=alfa+v,d=10,center = false);
        translate([0,-(kostka/2-rameno + pena+ vrstva + rezer),0]) {
            translate([0,0,vyskaCenter-vyskaPlochy])
            {
            
                plocha([sirkaPlochy ,vrstva,vyskaPlochy]);
                if(tiskBool == false)
                {
                    
                    translate([0,+ vrstva,0]) color("red",0.5) cube([sirkaPlochy ,pena,vyskaPlochy]);
                }
            }
            translate([0,0,vyskaCenter])
            {
                translate([0,vrstva/2,0]) rotate([0,90,0]) cylinder(h = odstup,d = vrstva);
                translate([odstup, vrstva/2,0 ]) 
                {
                   translate([0,0,0]) cylinder(h = 30,d = vrstva);
                   translate([0,0,0]) rotate([0,90,0]) cylinder(h = 20,d = vrstva);
                    translate([20,0,0]) intersection()
                    {
                        delkaPredHaku = 15;
                        vyskaPredHaku = 15;
                        rotate([0,90,0]) cylinder(h = delkaPredHaku, r1 = vyskaPredHaku, r2=0);
                        translate([0,-vrstva/2,-vrstva/2])cube([delkaPredHaku, vrstva, vyskaPredHaku + vrstva]);
                    }
                }
                vyskaPPP = 22;
               translate([posun -25,0,0]) OvalnaKrichleY([35,vrstva, vyskaPPP]); 
                translate([posun -15,0,0]) cube([vrstva, 10,vyskaPPP]);
                translate([posun ,0,0]) cube([vrstva, 20,vyskaPPP]);
                difference()
                {
                translate([0,0,0])cube([odstup,kostka/2-rameno + pena+ vrstva + rezer, vrstva]);
                    translate([odstup,0,0]) rotate([0,0,40]) cube(100);
                    translate([8,20,00]) rotate([0,0,00])  OvalnaKrichleZ([12,12,12],2);
                    for(x = [30:3:74])
                    {
                        translate([x,kostka/2-rameno + pena+ vrstva + rezer,0]) cylinder(h = 100, d = 2.3);
                        
                    }
                    translate([45,kostka/2-rameno + pena+ vrstva + rezer-10,0]) cylinder(h = 100, d = 3);
                    translate([65,kostka/2-rameno + pena+ vrstva + rezer-10,0]) cylinder(h = 100, d = 3);
                }
            }
            
        }
        
        
    }
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



//Kód pro vytvoření oválných kvádrů
module OvalnaKrichleX(Rozdmery, zakulaceni = 1,st = 30)
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


module OvalnaKrichleY(Rozdmery, zakulaceni = 1,st = 30)
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


module OvalnaKrichleZ(Rozdmery, zakulaceni = 1,st = 30)
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