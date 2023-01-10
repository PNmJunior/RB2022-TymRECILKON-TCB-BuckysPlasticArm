// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/


//komunikační knihovna
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
    str = str.replaceAll(SouborZnak, nahradaSouborZnak);
    str = str.replaceAll(SubSouborZnak, nahradaSubSouborZnak);
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

function  dataMotor(mot, hod)
{
    return [mot, hod];
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
    str=str.replaceAll( nahradaSouborZnak, SouborZnak);
    str=str.replaceAll( nahradaSubSouborZnak, SubSouborZnak);
    return str;
}


//work
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
    websocket.send(motorSend(dataMotor(sliderNumber, sliderValue)));
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


function updateSliderPWMnull(a) {
    websocket.send(motorSend(dataMotor(a, 0)));
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
    motorAllSend();
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
}