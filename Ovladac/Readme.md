# Ovladač
Jde o zařízení s možností ovládat všech osm motorů tak, aby to zvládal jeden člověk.
<img src = "fotky/ovladac_2_2_2023.jpg">
Se stativem
<img src = "fotky/ovladac_se_stativem.jpg">

## Zapojení
Použil jsem levné Arduino joysticky. Úrovně napětí bude snímat BluePill a posílat data do telefonu přes USB.

## Komunikace
Přes USB pomoci Seriálové linky. V telefoně v Chrome bude tyto data přijímaný přepočítávany a posílána do robota jako pokyny sliderů.
https://developer.chrome.com/articles/usb/

## Koncept
Pochopil jsem, že nejlepší bude vyrobit pákový joystick v rámci kterého bude ještě jeden joystick pro prst.
Pravý Pákovy = Pohon.
- levý motor = vzorec zatím nevyvinut.
- pravý motor = vzorec zatím nevyvinut.

Levý Pákovy = Rameno.
- M1 = osa x.
- M2 = osa y.

Levý Horní = Rameno.
- LED = osa x.
- M3 = osa y.

Levý Horní = Rameno.
- Kleste = osa x.
- M4 = osa y.
    
<img src = "fotky/koncept_ovladace.png">
<img src = "fotky/3.vzorec.png">

## Jednotka joysticku
<img src = "fotky/joystickova_jednotka.jpg">

## Modul pro telefon
<img src = "fotky/modul_pro_telefon_1.jpg"><img src = "fotky/modul_pro_telefon_2.jpg">


## Rychloupínák na stativ
Nákres
<img src = "fotky/rychloupinak_nacrtek.png">

## Schéma
<img src = "fotky/ovladac.png">


