


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
    
    translate([0,110,0]) rotate([-90,0,0]) KlesteLeva();
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
    translate([0,kk,50]) KlesteLeva();
    //translate([0,-30-$t*2*rameno,0]) KlestePrava();  
   translate([0,-kk,50]) KlestePrava();   
    //translate([11,-52,-20])rotate([0,0,45])  cube(40);
    //translate([23,-62,-20])rotate([0,0,45])  cube(40);
    translate([0,-kostka/2,0]) color("black",0.5) cube(kostka);    
}

posun = 30;
//KlesteLeva();
module KlesteLeva()
{  
    mirror([0,1,0]) Kleste();
}
//KlestePrava();

module KlestePrava()
{    
    Kleste();
}


module plocha(rozd)
{
    mezera = 3.5;

    difference()
    {
        cube(rozd);
        //pro osu y
        translate([mezera,rozd[1]*2,rozd[2]-mezera]) rotate([90,0,0]) cylinder(h = 100,d = 3);
        translate([mezera,rozd[1]*2,mezera]) rotate([90,0,0]) cylinder(h = 100,d = 3);
        translate([rozd[0]-mezera,rozd[1]*2,rozd[2]-mezera]) rotate([90,0,0]) cylinder(h = 100,d = 3);
        translate([rozd[0]-mezera,rozd[1]*2,mezera]) rotate([90,0,0]) cylinder(h = 100,d = 3);
        
     
        
        for(x = [mezera:(rozd[0] - mezera*2)/4:rozd[0] - mezera*2])
        {
            for(z = [mezera:(rozd[2] - mezera*2)/3:rozd[2] - mezera*2])
            {
                translate([x + mezera/2,-10,z + mezera/2]) OvalnaKrichleY([(rozd[0] - mezera*2)/4 - mezera,100, (rozd[2] - mezera*2)/3 - mezera],2);
                
            }
            
        }
    }
    
    
}
//Kleste();
module Kleste()
{

    v = pena + vrstva;
    vyskaCenter = 20;
    vyskaPlochy = 70;
    sirkaPlochy = 55;
    rezer = 10;
    vrstvaP = 2;
    odstup = sirkaPlochy + 40;

    //translate([j+j+bb/2-2,-v,0]) rotate([0,-90,0]) cylinder(h=bb+3*j,d=bb,center = false);

    
    //translate([0,-v,-4])  cylinder(h=40,d=10,center = false);
    translate([0,alfa,0]) 
    {
        translate([0,0,12]) rotate([0,0,90]) mirror([1,0,0]) RedukBeta();
        //rotate([90,0,0]) cylinder(h=alfa+v,d=10,center = false);
        translate([0,-(kostka/2-rameno + pena+ vrstva + rezer),0]) {
            vyskaBeta = vyskaPlochy;
            translate([0,0,vyskaCenter-vyskaBeta])
            {
                sirkaPlochyB = sirkaPlochy-10;
                difference()
                {
                    SpecCislo = kostka/2-rameno + pena+ vrstva + rezer;
                translate([0,0,0]) 
                    {
                        cube([vrstva,SpecCislo,vyskaBeta]);
                        rotate([0,18,0]) cube([10,SpecCislo,3]);
                        
                        
                        translate([vrstva/2 + 10,0,0])
                        {
                            cube([sirkaPlochy-10,vrstvaP,10]);
                            rotate([-18,0,0]) cube([sirkaPlochyB,10,3]);
                        }
                        
                    }
                    translate([vrstva/2 + 10,0,0])
                    {
                        translate([0,-2,-9]) rotate([-5,0,0]) cube([sirkaPlochyB,20,10]);
                        
                    }
                    translate([-2,00,-9]) rotate([0,5,0]) cube([20,SpecCislo,10]);
                    translate([sirkaPlochy + vrstva+0.67,2-0.22,0]) rotate([-5,0,0])rotate([0,-90+18,0]) cube([20,SpecCislo+2,20]);
                    translate([0,0,10]) for(y=[SpecCislo/6: SpecCislo/3: SpecCislo*(5/6)])
                    {
                        translate([0,y,0]) rotate([0,90,0]) cylinder(h = 20,d = 3);
                        translate([2,y,0]) rotate([0,90,0]) cylinder(h = 20,d = 6);
                        
                    }
                    translate([0,0,5]) for(y=[SpecCislo/3: SpecCislo/3: SpecCislo*(2/3)])
                    {
                        translate([0,y,0]) rotate([0,90,0]) cylinder(h = 20,d = 3);
                        translate([0,y,0]) rotate([0,90,0]) cylinder(h = 1,d = 6);
                        
                    }
                    translate([0,15,6.5]) for(x=[20: 10: sirkaPlochyB +10 ])
                    {
                        translate([x,0,0]) rotate([90,0,0]) cylinder(h = 20,d = 3);
                        //translate([0,0,0]) rotate([0,90,0]) cylinder(h = 2,d = 6);
                        
                    }
                }
                
            }
            
            translate([0,0,vyskaCenter-vyskaPlochy])
            {
                
                
                vyskaP = vyskaPlochy -10;
                translate([vrstva/2,0,10]) plocha([sirkaPlochy ,vrstvaP,vyskaP]);
                //translate([vrstva/2,0,0]) cube([sirkaPlochy,vrstvaP,10]);
                if(tiskBool == false)
                {
                    
                    translate([0,+ vrstva,0]) color("red",0.5) cube([sirkaPlochy ,pena,vyskaPlochy]);
                }
            }
            translate([-23,0,vrstva+5]) OvalnaKrichleY([24,vrstva,20-5]);
            
            translate([-10,0,vrstva+5]) cube([vrstva,15,15]);
            translate([-23,0,20]) cube([24,12,vrstva]);
            
            translate([-10,0,vrstva+5]) cube([10,15,vrstva]);
            translate([-10,0,20]) cube([10,15,vrstva]);
            
            
            
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
                difference()
                {
                translate([0,0,0])cube([odstup,kostka/2-rameno + pena+ vrstva + rezer, vrstva]);
                    translate([odstup,0,0]) rotate([0,0,40]) cube(100);
                    
                    for(x = [5:3:72])
                    {
                        translate([x,kostka/2-rameno + pena+ vrstva + rezer,0]) cylinder(h = 100, d = 2.3);
                        
                    }
                    translate([20,kostka/2-rameno + pena+ vrstva + rezer-10,0]) cylinder(h = 100, d = 4);
                    translate([40,kostka/2-rameno + pena+ vrstva + rezer-10,0]) cylinder(h = 100, d = 4);
                    translate([60,kostka/2-rameno + pena+ vrstva + rezer-10,0]) cylinder(h = 100, d = 4);
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
//RedukBeta();

module RedukBeta( naklon = 20, OddalY = 3, hranX = 30, prodluzY = 10 )
{
    difference()
    {
        translate([0,OddalY,0]) rotate([naklon,0,0]) difference()
        {
            vyskaPolstarku = 1.5;
            prumVyrezDer = 3;
            z_pods = 0;
            dira1 = [5+prumVyrezDer/2 + vyskaPolstarku, 2.5+prumVyrezDer/2,z_pods ];
            dira2 = [10.5+prumVyrezDer/2 + vyskaPolstarku, 19.5+prumVyrezDer/2,z_pods ];
            rozdDer = dira1 - dira2;
            uhel = -asin(rozdDer[0]/rozdDer[1]);
            vyskaDesky = 2.5;
            prumObjimMax = 6;
            vystMax = 0.5;
            delkaPodUhlem = sqrt(rozdDer[0]*rozdDer[0] + rozdDer[1]*rozdDer[1]) + prumObjimMax;
            
            
            translate([0,0,0])
            {
                translate([dira1[0]  - prumObjimMax/0.9 ,-OddalY-prumVyrezDer,0])rotate([0,0,uhel]) OvalnaKrichleZ([25,delkaPodUhlem  + prodluzY-prumVyrezDer/2,vyskaDesky],prumVyrezDer);
                echo(rozdDer);
                echo(uhel);
                translate([0,0,-0.5])
                {
                translate(dira1) cylinder(h = vystMax*2+ vyskaDesky , d = prumObjimMax);
                translate(dira2) cylinder(h = vystMax*2+ vyskaDesky , d = prumObjimMax);
                }
            }

            translate([0,0,-50])
            {
                translate(dira1) cylinder(h = 100 , d = prumVyrezDer);
                translate(dira2) cylinder(h = 100 , d = prumVyrezDer);
            }
            
        }
        translate([0,-100,-50]) cube(100);
        translate([hranX,0,-50]) cube(100);
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