# BuckysPlasticArm2022 - Robotický den 2022 - Tým RECYKLON - Úklid hraček

BuckysPlasticArm je robot, ve kterém jsem použil robotické rameno KSR10. 
Přidal jsem k němu podvozek z Totemu a jako řídicí elektroniku jsem použil ESP32. Hmotnost je 1,5kg. Robot zažil hned několik změn konceptů. 
Posledním konceptem je, že v podvozku je dobíjecí baterie z notebooku a v původním prostoru pro baterie je elektronika. 
Původně jsem chtěl robota ovládat pomocí Bluetooth a mnou naprogramované mobilní aplikace, ale rozhodl jsem se pro webové rozhraní vytvoření ESP32. [Z původního projktu z kterého jsem čerpal](https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/), jsem toho hodně přepracoval a vyladil. Velmi jsem urychlil komunikaci (asi desetkrát) a umožnil jsem robota ovládat z několika zařízení najednou.

## Aktuápní podoba

<img src = "fotodokumentace/sezona 2023/21.12.22.jpg">

## HTTPS

Vzhledem k tomu, že se mi nepodařilo najít https server knihovnu s rozumnou odezvou (pod 50ms), rozhodl jsem se nejít cestou https serveru.