podstavaPrum = 75;
podstavaVyska= 25;
vyska = 70+80+25;//neuvedeno
tyckaPrum = 5;//neuvedeno
trojuhelnikSirka = podstavaPrum/2; //uvedeno, ze je to mensi nrz prumer
trojuhelnikVyska = 70; //Snad to odpovída
trojuhelnikHloubka = 5;//neuvedeno



  module prism(l, w, h){
      polyhedron(//pt 0        1        2        3        4        5
              points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
              faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]);
      }
      
      
module znacka(Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka){
cylinder(h=Zvyska, d = ZtyckaPrum ,$fn=30);
      
translate([-ZtrojuhelnikHloubka/2,0,Zvyska ]) rotate([-90,0,0])
      {
prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska,ZtrojuhelnikSirka);
mirror([0,0,1]) prism(ZtrojuhelnikHloubka, ZtrojuhelnikVyska,ZtrojuhelnikSirka);
      }
  }
  
  
  module znackaSPodstavou(ZpodstavaPrum,ZpodstavaVyska,Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka){
cylinder(h=ZpodstavaVyska, d = ZpodstavaPrum ,$fn=50);
znacka (Zvyska, ZtyckaPrum ,ZtrojuhelnikSirka,ZtrojuhelnikVyska, ZtrojuhelnikHloubka);
      
  }
 
//znackaSPodstavou(podstavaPrum,podstavaVyska,vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);
  
  
translate([0,0,max(tyckaPrum,trojuhelnikHloubka)/2])rotate([0,90,0]) znacka(vyska, tyckaPrum ,trojuhelnikSirka,trojuhelnikVyska, trojuhelnikHloubka);