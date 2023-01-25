vrh = 35;
podstava = 43;
rozdilVrhPods = (podstava-vrh)/2;
echo(rozdilVrhPods);
vyskaZkoseni = 7.4;
vyskaCelkova = 8.8;
vyskaPosatavce = vyskaCelkova - vyskaZkoseni;
echo(vyskaPosatavce);

CubePoints = [
  [  0,  0,  0 ],  //0
  [ podstava,  0,  0 ],  //1
  [ podstava,  podstava,  0 ],  //2
  [  0,  podstava,  0 ],  //3
  [  rozdilVrhPods,  rozdilVrhPods,  vyskaZkoseni ],  //4
  [ podstava - rozdilVrhPods,  rozdilVrhPods,  vyskaZkoseni ],  //5
  [ podstava - rozdilVrhPods,  podstava - rozdilVrhPods,  vyskaZkoseni  ],  //6
  [  rozdilVrhPods,  podstava - rozdilVrhPods,  vyskaZkoseni ]]; //7
  
CubeFaces = [
  [0,1,2,3],  // bottom
  [4,5,1,0],  // front
  [7,6,5,4],  // top
  [5,6,2,1],  // right
  [6,7,3,2],  // back
  [7,4,0,3]]; // left
  
  
vyrez = 16;
vyrez_vyska = 4;

dir_roztec = 5;
dir_az = 10;
dir_vel = 3.2;
  
rychoup();

module rychoup()
{
    difference()
    {
        translate([0,0,0]) union()
        {
            translate([0,0,0]) cube([podstava, podstava,vyskaPosatavce]);
            translate([0,0,vyskaPosatavce]) polyhedron( CubePoints, CubeFaces );
        }
        translate([podstava/2,podstava/2,0])
        {
            
            translate([-vyrez,-vyrez,-vyrez_vyska]) OvalnaKrichleZ([vyrez*2,vyrez*2,vyrez_vyska*2],5,50);
            for(x = [-dir_az : dir_roztec  :dir_az])
            {
                for(y = [-dir_az : dir_roztec  :dir_az])
                {
                    translate([x,y,0]) cylinder(h=100, d=dir_vel, center=true,$fn=30);
                    //translate([x,y,0]) cylinder(h=6, d=10, center=true,$fn=30);
                }
            }
        }
        //translate([27,0,0]) cube(100);
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

