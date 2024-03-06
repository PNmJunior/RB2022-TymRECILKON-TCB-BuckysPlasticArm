# Článek o vývoji robota BuckysPlasticArm

Robot BuckysPlasticArm je pojízdný robot s manipulačním ramenem. Sestavil jsem si ho, abych si vyzkoušel práci s webovým  serverem umístěným v ESP32 (ESP32DevKitV1 [^11]), který přes webserver  umožňuje řídit pohyb robota s osmi DC motory. Webserver poskytuje v lokální sítí webovou službu, přes  kterou jde z více webových prohlížečů najednou ovládat robota. 

Prakticky jsem si funkčnost robota ověřoval na soutěži Robotický den 2022 a 2023. [^2][^3]

## Historie projektu

### 1.Etapa 
S projektem jsem začal v březnu 2022 a mým cílem byl Robotický den 2022 konající se 5. a 6. června. [^1] Jako základ jsem vybral projekt s nazývem „ESP32 Web Server (WebSocket) with Multiple Sliders: Control LEDs Brightness (PWM).“ od „Random Nerd Tutorials“, který jsem našel na internetu, a který jsem upravil pro své potřeby.[^3] Šlo o ovládání tří parametrů LEDek. Zvětšil jsem množství ovládaných parametrů na osm. Každý ovládací parametr řídil jeden motor. Do původní webové stránky [^3] jsem přidal jak nové horizontální, tak i vertikální slidery (ovládací posuvný prvek v HTML(```<imput type="range">```)).[^10] Zjistil jsem, že z Androidů lze ovládat dva slidery najednou, zatím  co na počítači limitovaného  myší, šlo ovládat jen jeden slider. Můj plán byl umožnit řídit robota ze čtyř mobilních zařízení najednou v reálném čase pomoci týmů lidí. Kvůli řízení v reálném čase, jsem musel minimalizovat množství posílaných dat. Díky tomu jsem se dostal na odezvu robota v řádech desítek milisekund. Změnil jsem typ http serveru ze synchronního na asynchronní.

Na soutěži jsem si ověřil, že řízení pomocí více lidí je špatný nápad, kvůli jejích nutné vzájemné souhře.

### 2.Etapa
Technologie webserveru v ESP32 mě zaujala, a tak jsem se rozhodl pokračovat ve vývoji. Pustil jsem se do velké přestavby konstrukce robota, hardwaru a softwaru. Vytvořil jsem si jednoduchý ovládací panel, který mi umožňoval výběr WiFi a  zadání hesla pomoci enkodéru pro připojení k lokální síti. Pomoci segmentového LED displeje robot informoval o vlastní aktuální IP adrese v lokální síti. Vytvořil jsem si vlastní komunikační protokol mezi serverem a klienty. Přidal jsem do zobrazení webu čtyři joysticky se synchronizací (každý vidí, co dělají ostatní).[^7] Synchronizace sliderů byla už z původního projektů.[^3] Kód pro joystick, který byl napsaný v JavaScriptu, hýbal s obrázkem páčky. ESP32 převádělo pohyby joysticků na pohyb sliderů. Díky tomu šlo jedním kliknutím ovládat až dva motory najednou. Tím se mi snížil počet lidí s telefonem k ovládaní robota na dva. 

### 3.Etapa - Fyzické joysticky
Rozhodl jsem se, že umožním řízení zmíněných osmi motorů pomoci jednoho člověka. Pro tento účel jsem vyrobil dvojici fyzických joysticků (pro každou ruku), kde se zápěstím ovládá dolní joystick a druhý/horní joystick se ovládá pomoci palce. 

Po té jsem začal řešit spojení s robotem. Ano, šlo to jednoduše vyřešit dalším ESP32, ale to bych si nevyzkoušel nové technologie, a proto jsem se rozhodl, že použiju WebUSB.[^4] Pomoci desky Bluepill(deska s čipem ARM STM32 STM32F103C8 [^12]) jsem chtěl komunikovat s webovým klientem v prohlížeči právě přes USB. Zde jsem narazil na překážku v podobě bezpečnosti, kde bylo podmínkou použití protokolu https. Celý projekt do této doby fungoval přes http. Po nastudování dokumentace a různých diskusí jsem došel k názoru, že přeprogramovat ESP32 na https server by bylo možné. Ale mnohonásobně by se tím zvýšila doba odezvy až na sekundy. Zároveň by se snížil počet možných webových klientů z osmi na maximálně čtyři, co je na hranici plánovaného používaní.[^8] [^9] K řešení problému s bezpečností nepřispělo ani nahrávaní stránek z disku. Nakonec jsem musel použit aplikaci, která umožňovala převádět komunikaci z USB na Telnet napsané v Javě.[^5] Tento koncept jsem použil a zprovoznil, ale doba odezvy byla vysoká a tak jsem přeprogramoval aplikaci pro simulaci klienta.[^6] Tím jsem maximálně snížil dobu odezvy na pohyb joysticků. Můj protokol nebyl navržen pro stream, ale pouze pro paketovou přepravu. To mělo za následek, že jsem musel vyřešit na straně aplikace rozpoznávání zamýšlených paketů.

### Testování webu na konci 2.etapy a v průběhu celé 3.etapy 
Projekt jsem ladil pouze na prohlížeči Google Chrome pro platformy Windows a Android. K tomu se mi občas objevila možnost si kód otestovat na jiných zařízeních, platformách a prohlížečích. Díky tomu jsem zjistil několik zajímavých informací z pohledu kompatibility kódu. Při testovaní jsem se zaměřoval na funkce  sliderů a  softwarových joysticků. Testy s Mac a IOS byly prováděny pouze na prohlížeči Safari. U Androidu jsem provedl testovaní na Webview, Google Chrome a Firefox. Na Windows jsem to otestoval na  Webview, Google Chrome, Firefox a Edge. Na Linuxech jsem testoval Google Chrome a Firefox. Podle očekávaní se slider zobrazoval na různých prohlížečích trochu jinak(různá implementace HTML prvku).[^11] Překvapil mě rozdíl podpory zobrazení horizontálních a vertikálních sliderů v rámci stejného prohlížeče. Dokonce na Firefoxu mi vertikální slidery vůbec nefungovaly. Je ale důležité upozornit, že joysticky (manipulovaní s obrázkem) vždy fungovaly na všech prohlížečích správně a měly ten samý vzhled a chování.[^7]

Nalezl jsem článek, který popisuje tuhle problematiku. [^13] 

### 4.etapa
Znova jsem celého robota přestavěl konstrukčně a ještě před soutěží Robotický den 2023 [^2] jsem implementoval funkce pro kontrolu spojení. Na soutěži mi robot kvůli neustálé detekci rušení spojení přerušoval pohyb. Bohužel neměl jsem čas dostatečně vyladit tuhle kontrolu.

### Finální zakončení projektu
Po soutěži jsem vytvořil github větev def2024, kde je program bez funkcí kontroly spojení.

Pro vyladění kontroly spojení by bylo nutné téměř vše přeprogramovat. 

Odstranil jsem problémy řešené v kapitole „Testování webu na konci 2.etapy a v průběhu celé 3.etapy“ pomoci článku zabývající se slidery. [^13]

## Technický popis:

V projektu jsem si praktický ověřil a otestoval: 
- Poskytování webových služeb v lokální síti přes ESP32.
- Koncept řízení s více klienty najednou. 
- Řízení až 8 parametrů najednou z 4 klientů. 
- Kontrola spojení. 
- Napsání jednoduchého html/css/javascript kódu pro řízení a kontrolu nastavení jednotlivých ovládaných parametrů. 
- Otestoval jsem kombinaci různých druhů řízení a kontroly.
- Zkusil jsem si tvorbu vlastního komunikačního protokolu a jeho zefektivnění. 
- Upravil  jsem aplikaci v Javě, která emulovala chovaní klienta a analyzovala přeposílána data.

### Řízení z vice zdrojů (klient a jiné vstupy dat)
U řízení z více zdrojů jsem došel k závěru, že nejprve je nutné popsat druhy a množství vstupních zdrojů. Dají se rozdělit na dva typy: Jednosměrné a synchronní.

#### Jednosměrné řízení
Jednosměrné jsou ovladače bez zpětné vazby, a proto nelze si ověřit aktuálnost dat a žádná data se nezobrazují ani nijak nezpracovávají. Kvůli zjednodušení lze je považovat za direktivní, informace s maximální prioritou nelze ignorovat. To znamená, že synchronní řízení podléhá přímému řízení.

#### Synchronní řízení
Synchronní řízení je typické pro web s klienty. Lze řídit a vidět, co dělají ostatní.

Kolize pokynů nebo strata spojení nebo následná kombinace obojího je velký problém. Například když klient dá parametr na 100% a následně se přeruší spojení, je nutné buď nastavit parametr na 0 nebo nechat jiného klienta převzít řízení. Zároveň detekce ztráty spojení pro ESP32 je taky dost náročná na výkon. Dalším problémem je situace, kdy dva klienti ovládají stejný parametr. Další situace je když jeden klient ovládá parametr a přeruší se spojení na určitou dobu. Je nutné, aby detekce spojení umožnila jinému uživateli převzít řízení a následně, aby vyřešila situaci s náhlým množstvím expirovaných instrukcí z původního klienta.

### Kontrola spojení
Kontrola spojení u synchronních zdrojů je nutností kvůli řízení v reálném čase. Kvůli analýze spojení je nutné neustále posílat alespoň datový paket s informací o čase. Následně je otázka, jak zareagovat na pakety, které jsou starší než by měly být. Pro ESP32 je velmi náročné přijímat neustále data od vice klientů, je zde velká pravděpodobnost zahlcení a častého restartovaní.

### Jednoduchý kód pro web na různých platformách
Prakticky jsem si vyzkoušel tvorbu ovládacích prvků pomocí obrázků a JavaScriptu. Rozhodl jsem se nespoléhat na ovládací prvky v HTML a na platformně závislé funkce v JavaScriptu. Pokud si to naprogramujete pomocí základních JavaScriptových funkcí, tak vzhled a chování by mělo být jednotné na mnoha platformách po dlouhou dobu. Slidery se v každém prohlížeči zobrazovaly a chovaly jinak. Zatímco kód emulující joysticky [^7] s grafikou pomoci obrázků vždy se choval a vypadal stejně. Vice v předchozí kapitole Historie-Testování.

### Komunikační protokol mezi serverem a klienty

#### Původní protokol
Nejprve jsem použil komunikační protokol z původního projektu, založený na JSON.[^3] Tento protokol byl velmi popisný, proto vyžadoval velké množství dat. Data pro osm parametrů se museli dělit do dvou paketů, co značně zpomalovalo komunikaci. 

#### Optimalizace protokolu
Nejprve jsem optimalizoval množství posílaných dat. Upravil jsem protokol tak , že na místo posílání klíčového slova jsem posílal jen jedno písmeno. Tím jsem celkovou velikost snížil na jeden paket při osmi parametrech.

#### Vlastní protokol
Můj protokol byl textový a měl dva klíčové znaky:
- „;“ pro zahájeni balíčku, 
- „:“  rozdělení jednotlivých subbalíčků o různém počtu.

Balíček obsahoval soubor subbaličků, kde první subbalíček určoval typ informace(příkaz) a z toho se odvodil následný počet subbalíčků. V jednom paketu mohlo být více balíčků. Díky TCP funkcím v ESP32 se vždy server začal zabývat daty až když byly kompletní. Díky tomu nebylo nutné implementovat koncový znak. 

#### Problémy
Při komunikaci přes USB mezi aplikací a Bluepill([^12]), který snímal polohu mechanických joysticků, jsem musel vkládat specifické balíčky. Na začátek a konec každého posílaného myšleného paketu ze strany Bluepill, jsem vložil balíček, aby aplikace jej mohla identifikovat a celý ho poslat.

## Závěr

Na tomto projektu jsem si otestoval mnoho technologií a postupů. Vzhledem ke komplikovanosti problematiky serverů z pohledu bezpečnosti a výkonu, jsem se rozhodl si vytvořit nový projekt zabývající se částečným převodem povinností serveru na aplikaci, která by s ESP32 komunikovala přes Bluetooth.

## Zdroje:
Zdroje jsou zobrazené na konci README.md

[^1]: Robotic Day 2022 [online]. 2022 [cit. 2024-02-15]. Dostupné z: http://robotickyden.cz/2022/

[^2]: Robotic Day 2023 [online]. 2023 [cit. 2024-02-15]. Dostupné z: http://robotickyden.cz/2023/

[^3]: ESP32 Web Server (WebSocket) with Multiple Sliders: Control LEDs Brightness (PWM). Random Nerd Tutorials [online]. 2021 [cit. 2024-02-15]. Dostupné z: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/

[^4]: Access USB Devices on the Web. Chrome for developers [online]. [cit. 2024-02-15]. Dostupné z: https://developer.chrome.com/docs/capabilities/usb

[^5]: CLUSTER, Alexey. USB Serial Telnet Server. GitHub [online]. [cit. 2024-02-15]. Dostupné z: https://github.com/ClusterM/usb-serial-telnet-server.git

[^6]: ŇACHAJ, Pavol. USB Serial Telnet Server. GitHub [online]. 2023 [cit. 2024-02-15]. Dostupné z: https://github.com/PNmJunior/usb-serial-telnet-server.git

[^7]: STEMKOSKI. Touch-enabled Virtual JoyStick Controller In JavaScript. CSS Script [online]. 2020 [cit. 2024-02-16]. Dostupné z: https://www.cssscript.com/touch-joystick-controller/

[^8]: HESSEL, Frank. ESP32 HTTPS Server. GitHub [online]. [cit. 2024-02-22]. Dostupné z: https://github.com/fhessel/esp32_https_server.git

[^9]: HTTPS server. ESPRESSIF [online]. [cit. 2024-02-22]. Dostupné z: https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/api-reference/protocols/esp_https_server.html?highlight=https

[^10]: ```<input type="range">```. Mdm [online]. [cit. 2024-02-26]. Dostupné z: https://developer.mozilla.org/en-US/docs/Web/HTML/Element/input/range

[^11]: PK-DevKitV1 Esp32. PROKYBER s.r.o. [online]. [cit. 2024-02-26]. Dostupné z: https://e-shop.prokyber.cz/esp32/esp32-pkdevkitv1/

[^12]: BluePill ARM STM32 STM32F103C8 Vývojová deska. LaskaKit [online]. [cit. 2024-02-26]. Dostupné z: https://www.laskakit.cz/bluepill-arm-stm32-stm32f103c8-vyvojova-deska/

[^13]: Stylování ```<input type=range>```. JAHODA, Bohumil. JE ČAS [online]. 2023 [cit. 2024-03-05]. Dostupné z: https://jecas.cz/input-range


===

Původní verze úvodu:

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