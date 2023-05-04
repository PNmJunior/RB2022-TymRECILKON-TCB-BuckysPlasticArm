podstavaPrum = 75;
podstavaVyska= 25;
vyska = 70+80+25;//neuvedeno
tyckaPrum = 4;//neuvedeno
trojuhelnikSirka = podstavaPrum/2; //uvedeno, ze je to mensi nrz prumer
trojuhelnikVyska = 70; //Snad to odpovída
trojuhelnikHloubka = tyckaPrum;//neuvedeno


module prism2(l, w, h)
{
    polyhedron(//pt 0        1        2        3        4        5
        points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
        faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]);
}


module prism(l, w, h)
{
    zaokPPP = 1;
    echo(l,w,h);
    minkowski()
    {
      $fn=50;
      prism2(l/2,w-zaokPPP,h-zaokPPP);
      rotate([0,90,0]) cylinder(r=zaokPPP,h=l/2);
    }
}


module znacka(Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka)
{
    cylinder(h=Zvyska -10, d = ZtyckaPrum ,$fn=30);
    difference()
    {
        translate([-ZtrojuhelnikHloubka/2,0,Zvyska ]) rotate([-90,0,0])
        {
            prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska,ZtrojuhelnikSirka);
            mirror([0,0,1]) prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska,ZtrojuhelnikSirka);
        }
        translate([-ZtrojuhelnikHloubka/2,0,Zvyska-12.5 ]) rotate([-90,0,0])
        {
            prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska-20,ZtrojuhelnikSirka-15);
            mirror([0,0,1]) prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska-20,ZtrojuhelnikSirka-15);
        }
    }
}
  
  
module znackaSPodstavou(ZpodstavaPrum,ZpodstavaVyska,Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka)
{
    cylinder(h=ZpodstavaVyska, d = ZpodstavaPrum ,$fn=50);
    znacka (Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka); 
}


module znackaSRozsirenim(podstavaVyska,vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka)
{
    difference()
    {
        znackaSPodstavou(5.8,podstavaVyska,vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);
        translate([tyckaPrum/2,-tyckaPrum/2,-0.5]) cube([tyckaPrum,tyckaPrum,podstavaVyska + 1]);
        translate([-3*tyckaPrum/2,-tyckaPrum/2,-0.5]) cube([tyckaPrum,tyckaPrum,podstavaVyska + 1]);
        
    }
}


translate([0,0,max(tyckaPrum,trojuhelnikHloubka)/2])rotate([0,90,0]) znackaSRozsirenim(podstavaVyska,vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);
 
//znackaSPodstavou(podstavaPrum,podstavaVyska,vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);
  
//translate([0,0,max(tyckaPrum,trojuhelnikHloubka)/2])rotate([0,90,0]) znacka(vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);
