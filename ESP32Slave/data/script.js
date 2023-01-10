// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/

const SouborZnak = ";";
const nahradaSouborZnak =" ///1||| ";
const SubSouborZnak =":";
const nahradaSubSouborZnak= " ///2||| " ;


function balicekInt(a)
{
    return String(a);
}


function balicekText(a)
{
    let str = String(a)
    str.replace(SouborZnak, nahradaSouborZnak);
    str.replace(SubSouborZnak, nahradaSubSouborZnak);
    return str;
}


function addSoubor(a)
{
    return SouborZnak + a;
}


function addSubSoubor(a)
{
    return SubSouborZnak + a;
}


function motorSend(a)
{
    return addSoubor(balicekText('m')) + addSubSoubor(balicekInt(a[0])) + addSubSoubor(balicekInt(a[1]));
}

function motorQestionSend(a)
{
    return addSoubor(balicekText('M')) + addSubSoubor(balicekInt(a));
}

function motorAllSend()
{
    return addSoubor(balicekText('A'));
}

function viceMotoru(a)
{
    let s = "";
    for(let element of a) 
    {
        s = s + motor(element);
    }
    return s;
}


function read(a)
{
    const str =  String(a);
    const soubor = str.split(SouborZnak);
    let pole = [];
    soubor.shift();
    for(const subsoubor of soubor) 
    {
        pole.push(subsoubor.split(SubSouborZnak));
    }
    console.log(pole);
    return pole;
}


function readInt(a)
{
    return Number(a);
}


function readText(a)
{
    let str = String(a)
    str.replace( nahradaSouborZnak, SouborZnak);
    str.replace( nahradaSubSouborZnak, SubSouborZnak);
    return str;
}






var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getValues(){
    websocket.send(motorAllSend());
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    getValues();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function updateSliderPWM(element) {
    var sliderNumber = element.id.charAt(element.id.length-1);
    var sliderValue = document.getElementById(element.id).value;
    //document.getElementById("sV"+sliderNumber).innerHTML = sliderValue;
    console.log(sliderValue);
    websocket.send(motorSend([sliderNumber, sliderValue]));
}


function delay(delayInms) {
    return new Promise(resolve => {
      setTimeout(() => {
        resolve(2);
      }, delayInms);
    });
  }
  
async function sample() {
    console.log('a');
    console.log('waiting...')
    let delayres = await delay(3000);
    console.log('b');
  }


function updateSliderPWMnullLevy() {
    websocket.send(motorSend([8,0]));
}

function updateSliderPWMnullPravy() {
    websocket.send(motorSend([7,0]));;
}

function reset123()
{
    location.href = "/reset";
    setTimeout(function(){ console.log("After 0.5 seconds!"); }, 500);
    location.href = "/";
    stisknuto();
}

function stisknuto()
{ 
    /*  
    websocket.send("9s0"); 
    var a = window.open("/stop");
    a.close();
    
    for(var i = 1; i < 9; i++)
    {        
        document.getElementById("slider"+i.toString()).innerHTML = 0;
        console.log("a");
        websocket.send(i.toString()+"s0");
        console.log("b");
    }
    websocket.send("9s0");
    var b = window.open("/stop");
    b.close();
    */
}

function onMessage(event) {
    console.log(event.data);

    var dataIn = read(event.data);
    for(const SoubIn of dataIn) 
    {
        console.log("SoubIn");
        console.log(SoubIn);
        switch (readText( SoubIn[0]))
        {
            

            case "m":
                let key = readInt( SoubIn[1]);
                let myObj = readInt( SoubIn[2]);
                console.log("key"); 
                console.log(key);
                
                console.log("myObj");
                console.log(myObj);
                console.log("slider"+ key.toString()+"="+myObj.toString());
                document.getElementById("slider"+ key.toString()).value = myObj;
                //document.getElementById(key.toString()).innerHTML = myObj;
                document.getElementById("sV"+key.toString()).innerHTML = myObj;
                break;
            default:
                break;

        }
    }



/*
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);
    console.log(myObj);
    console.log(keys);

    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        var keyB = key.toString().charAt(2);
        console.log("kiijonikn");
        console.log(key);
        document.getElementById(key).innerHTML = myObj[key];
        console.log(myObj[key]);
        console.log("slider"+ key.toString()+"="+myObj[key].toString());
        document.getElementById("slider"+ keyB.toString()).value = myObj[key];
        
    }
    */
}