# BuckysPlasticArm2022 - Robotický den 2022 - Tým RECYKLON - Úklid hraček

BuckysPlasticArm je robot, ve kterém jsem použil robotické rameno KSR10. 
Přidal jsem k němu podvozek z Totemu a jako řídicí elektroniku jsem použil ESP32. Hmotnost je 1,5kg. Robot zažil hned několik změn konceptů. 
Posledním konceptem je, že v podvozku je dobíjecí baterie z notebooku a v původním prostoru pro baterie je elektronika. 
Původně jsem chtěl robota ovládat pomocí Bluetooth a mnou naprogramované mobilní aplikace, ale rozhodl jsem se pro webové rozhraní vytvoření ESP32. [Z původního projktu z kterého jsem čerpal](https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/), jsem toho hodně přepracoval a vyladil. Velmi jsem urychlil komunikaci (asi desetkrát) a umožnil jsem robota ovládat z několika zařízení najednou.

## Aktuápní podoba

<img src = "fotodokumentace/sezona 2023/21.12.22.jpg">

# Popis větví projektu
## [main](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/main) 
Hlavní větev pro práci.
## [Telnet](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/Telnet) 
Značně propracovaná větev, ale kvůli pomalé rychlosti uzavřena. Tato technologie pomohla mi překlenou období mezi neúspěchem s https a přepsáním [aplikace](https://github.com/PNmJunior/usb-serial-telnet-server/tree/java-android-websocket-client) používané právě pro Telnet, kde jsem přidal připojovaní se pomoci webSocketu s serverem robota a zpracování dat z USB. 
Funkční. Práce ukončeny.
## [https-server](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/https-server) 
Marný pokus o zprovoznění https-serveru proto, abych mohl použít [WebUSB](https://developer.chrome.com/articles/usb/) . Použití by znamenalo značné prodlevy v komunikaci. K samotné zkoušce WebUSB jsem se v této větvi nedostal. 
Nefunkční. Prace pozastaveny.
## [UsbWeb-opening-from-file](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/UsbWeb-opening-from-file) 
Pokus o spuštění webové aplikace s [WebUSB](https://developer.chrome.com/articles/usb/) pomoci klikaceného souboru HTML. Tím jsem chtěl obejit bezpečnost bránicí požití WebUSB z http. Nastali problémy, které jsme nedokázal vyřešit. 
Nefunkční. Práce ukončeny.
## [http-server](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/http-server) 
Funkční verze k 24.únoru 2023
## [Roboticky-den-2022](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/Roboticky-den-2022) 
Soutěžní a funkční verze k 16.řijen 2022
## [main2](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/main2) , [main3](https://github.com/PNmJunior/RB2022-TymRECYLKON-TCB-BuckysPlasticArm/tree/main3) 
Historické. Nedoporučuji čerpat. Uzavřeno. Nefunkční.