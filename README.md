# BuckysPlasticArm

Robota BuckysPlasticArm jsem vždy koncipoval jako prostředek k rozvíjený znalostí v oblasti IOT. Robot je řízen přes webové rozhraní v kombinaci s joysticky. Veškerá komunikace je vedena přes webserver umístěný na robotovi v podobě ESP32, který zároveň ovládá celého robota.
Zapojení webserveru má svoje výhody ale i nevýhody. Jednou nespornou výhodou je možnost se podílet více zařízeními na ovládání robota. Navíc se stačí jen připojit do stejné sítě a v prohlížeči zadat ip adresu, kterou ukazuje robot. 
Nevýhodou není ani webserver, ale spojení přes wifi, která není stavěná na spojitou komunikaci. To značně komplikuje situaci webserveru. 
Nesmím zapomenout na historii robota. Tento robot se jako rychlo projekt zúčastnil minulý rok Robotického dne na disciplíně Toy Cleanup Beginner. Za dobu existence robota už měl tři různé konstrukce, dva různé hardwary a nekonečně uprav v softwaru. Přírůstkem jsou joysticky. Toto zařízení má čtyři joysticky PS4 a signál zpracovává STM32, které posílá data přes mobilní aplikaci, mnou modifikovanou, přes router do robota. Je úplné k nevíře, že řízení pomoci webu a joysticků je v reálném čase.
Robot je sestaven pomoci stavebnice KSR10. 
[Základy projektu](https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/)

## QR
<img src = "fotodokumentace/QR.png">


## Aktuápní podoba

<img src = "fotodokumentace/sezona 2023/31.05.23a.jpg">
<img src = "fotodokumentace/sezona 2023/31.05.23b.jpg">
<img src = "fotodokumentace/sezona 2023/31.05.23c.jpg">

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