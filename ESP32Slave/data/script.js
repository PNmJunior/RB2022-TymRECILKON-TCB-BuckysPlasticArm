// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getValues(){
    websocket.send("getValues");
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
    document.getElementById("sV"+sliderNumber).innerHTML = sliderValue;
    console.log(sliderValue);
    websocket.send(sliderNumber+"s"+sliderValue.toString());
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


function updateSliderPWM2(a,b) {
    websocket.send(a.toString()+"s"+b.toString());
}

function updateSliderPWMnullLevy() {
    websocket.send("2s0");
}

function updateSliderPWMnullPravy() {
    websocket.send("1s0");
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
}

function onMessage(event) {
    console.log(event.data);
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
}