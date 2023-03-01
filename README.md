# BuckysPlasticArm2022 - Robotický den 2022 - Tým RECYKLON - Úklid hraček

BuckysPlasticArm je robot, ve kterém jsem použil robotické rameno KSR10. 
Přidal jsem k němu podvozek z Totemu a jako řídicí elektroniku jsem použil ESP32. Hmotnost je 1,5kg. Robot zažil hned několik změn konceptů. 
Posledním konceptem je, že v podvozku je dobíjecí baterie z notebooku a v původním prostoru pro baterie je elektronika. 
Původně jsem chtěl robota ovládat pomocí Bluetooth a mnou naprogramované mobilní aplikace, ale rozhodl jsem se pro webové rozhraní vytvoření ESP32. [Z původního projktu z kterého jsem čerpal](https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/), jsem toho hodně přepracoval a vyladil. Velmi jsem urychlil komunikaci (asi desetkrát) a umožnil jsem robota ovládat z několika zařízení najednou.

## Aktuápní podoba

<img src = "fotodokumentace/sezona 2023/21.12.22.jpg">

## Android app TelNet
Našel jsem aplikaci naprogramovanou v java, která se zabývá transportem dat z USB Serial pomoci Telnet Server.
Původní projekt [usb-serial-telnet-server](https://github.com/ClusterM/usb-serial-telnet-server.git)
Tento projekt mám v plánu upravit v mém folk:
Moje verze [usb-serial-telnet-server](https://github.com/PNmJunior/usb-serial-telnet-server.git)
V plánu mám upravit následující:
- Po úspěšném spuštění serveru, automatické kopírování do schránky ip a port serveru.
- Zmenšit prodlevu mezi přijetím a přeposláním.
