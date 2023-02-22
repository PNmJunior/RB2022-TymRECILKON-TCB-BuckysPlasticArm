// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/


//komunikační knihovna
const SouborZnak = ";";
const nahradaSouborZnak =" ///1||| ";
const SubSouborZnak =":";
const nahradaSubSouborZnak= " ///2||| " ;

const M_LED = 0;
const M_1 = 1;
const M_2 = 2;
const M_3 = 3;
const M_4 = 4;
const M_Kleste = 5;
const M_Levy = 6;
const M_Pravy = 7;

//const ModeWork = 'w';//work
const ModeWork = 'd';//debug
function balicekInt(a)
{
    if(a == -0)
    {
        a = 0;
    }
    else if(a == NaN)
    {

    }
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

function joySend(j,x,y,t = 0)
{
    return sendAuto(dataJoy(j,x,y,t));
}

function motorSend(mot,hod)
{
    return sendAuto(dataMotor(mot,hod));
}

function motorQestionSend(a)
{
    return sendAuto(dataMotorQestion(a));
}

function motorAllSend()
{
    return sendAuto(dataMotorAll());
}

function dataJoy(j,x,y,t)
{
    return ['j',balicekInt( j),balicekInt( x),balicekInt( y),balicekInt( t)];
}

function dataMotorAll()
{
    return ['A'];
}

function dataMotorQestion(mot)
{
    return ['M',balicekInt( mot)];
}

function  dataMotor(mot, hod)
{
    return ['m',balicekInt( mot),balicekInt( hod)];
}

function sendAuto(a)
{
    let str = addSoubor(a[0]);
    a.shift();
    for(const b of a) 
    {
        str = str + addSubSoubor(b);
    }
    return str;

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

function readJoy()
{
    let strJ1 = ""//cteni
    let Jstartindex = 0;
    let Jstopindex = 0;
    let strJ2 = "";
    let strJ3 = "";
    let JRead ;
    do {
        strJ1 += ""//cteni
        Jstartindex = strJ1.indexOf(";s")
    } while (Jstartindex == -1);
    strJ2 = strJ1.substring(Jstartindex);
    do {
        strJ2 += ""//cteni
       Jstopindex = strJ2.indexOf(";t") 
    } while (Jstopindex == -1);
    Jstopindex += 2;
    strJ1 = strJ2.substring(Jstopindex);
    strJ3 = strJ2.substring(0,Jstopindex);
    JRead = read(strJ3);
    if(JRead[0][0] == 's' && JRead[1][0] == 'j' && JRead[2][0] =='t')
    {
        switch (JRead[1][1]) {
            case "0":
                
                break;
            case "1":
                
                break;
            case "2":
                
                break;
            case "3":
                
                break;
            default:
                break;
        }
    }
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

var startTimeSend = performance.now();
var startTimeIn = performance.now();
var startTimeAll = performance.now();
var timeOldSend = performance.now();

function onload(event) {
    initWebSocket();
}

function getValues(){
    SendESP(motorAllSend());
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    if(ModeWork == "w")
    {
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
    }
    if(window.isSecureContext) {
        console.log("Zabezpeceno");
    }
    else
    {
        console.log("Nezabezpeceno");
    }
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
    startTimeSend = performance.now();
    startTimeAll = performance.now();
    var sliderNumber = element.id.charAt(element.id.length-1);
    var sliderValue = document.getElementById(element.id).value;
    //document.getElementById("sV"+sliderNumber).innerHTML = sliderValue;
    console.log(sliderValue);
    SendESP(motorSend(sliderNumber, sliderValue));
    console.log(`Send time ${performance.now() - startTimeSend} milliseconds`);
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


function SendESP(a)
{
    let Astring = String(a);
    if(Astring == "")
    {
        console.log("Prazdne pole");
    }
    else if (ModeWork == "w")
    {
        console.log("Posilano:"); console.log(a);
        websocket.send(a);
    }
    else
    {
        console.log("Debag:"); console.log(a);
    }
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
    startTimeIn = performance.now();
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
    console.log(`In time ${performance.now() - startTimeIn} milliseconds`);
    console.log(`All time ${performance.now() - startTimeAll} milliseconds`);
}


class JoystickController
{
	// stickID: ID of HTML element (representing joystick) that will be dragged
	// maxDistance: maximum amount joystick can move in any direction
	// deadzone: joystick must move at least this amount from origin to register value change
	constructor( stickID, maxDistance, deadzone )
	{
		this.id = stickID;
		let stick = document.getElementById(stickID);

		// location from which drag begins, used to calculate offsets
		this.dragStart = null;

		// track touch identifier in case multiple joysticks present
		this.touchId = null;
		
		this.active = false;
		this.value = { x: 0, y: 0 ,uhel:0, d:0}; 

		let self = this;

        this.old = { x: 0, y: 0 ,uhel:0, d:0};

		function handleDown(event)
		{
		    self.active = true;

			// all drag movements are instantaneous
			stick.style.transition = '0s';

			// touch event fired before mouse event; prevent redundant mouse event from firing
			event.preventDefault();

		    if (event.changedTouches)
		    	self.dragStart = { x: event.changedTouches[0].clientX, y: event.changedTouches[0].clientY };
		    else
		    	self.dragStart = { x: event.clientX, y: event.clientY };

			// if this is a touch event, keep track of which one
		    if (event.changedTouches)
		    	self.touchId = event.changedTouches[0].identifier;
		}
		
		function handleMove(event) 
		{
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    // also handle multiple simultaneous touchmove events
		    let touchmoveId = null;
		    if (event.changedTouches)
		    {
		    	for (let i = 0; i < event.changedTouches.length; i++)
		    	{
		    		if (self.touchId == event.changedTouches[i].identifier)
		    		{
		    			touchmoveId = i;
		    			event.clientX = event.changedTouches[i].clientX;
		    			event.clientY = event.changedTouches[i].clientY;
		    		}
		    	}

		    	if (touchmoveId == null) return;
		    }
            handleMoveB(event.clientX - self.dragStart.x, event.clientY - self.dragStart.y);

        }

        function handleMoveB(xD,yD)
        {
		    var xDiff = Number(xD);
		    var yDiff = Number(yD);
		    const angle = Math.atan2(yDiff, xDiff);
			const distance = Math.min(maxDistance, Math.hypot(xDiff, yDiff));
			const xPosition = distance * Math.cos(angle);
			const yPosition = distance * Math.sin(angle);

/*
		    const angle = Math.atan2(yDiff, xDiff);
			let xPosition = 0;
			let yPosition = 0;
            if(xDiff > 0)
            {
                xPosition = Math.min(xDiff, maxDistance);
            }
            else if(xDiff < 0)
            {
                xPosition = Math.max(xDiff, -maxDistance);
            }

            if(yDiff > 0)
            {
                yPosition = Math.min(yDiff, maxDistance);
            }
            else if(yDiff < 0)
            {
                yPosition = Math.max(yDiff, -maxDistance);
            }

            const distance = Math.max(Math.abs(xPosition), Math.abs(yPosition));
            */
            
            //console.log(xPosition);
            //console.log(yPosition);


			// move stick image to new position
		    stick.style.transform = `translate3d(${xPosition}px, ${yPosition}px, 0px)`;

			// deadzone adjustment
		    var xPercent = xPosition.toFixed(0);
		    var yPercent = yPosition.toFixed(0);
		    self.value = { x: xPercent, y: yPercent , uhel: angle, d:distance};
		  }

		function handleUp(event) 
		{
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    if (event.changedTouches && self.touchId != event.changedTouches[0].identifier) return;

		    // transition the joystick position back to center
		    stick.style.transition = '.2s';
		    stick.style.transform = `translate3d(0px, 0px, 0px)`;

		    // reset everything
		    self.value = { x: 0, y: 0 , uhel: 0, d:0};
		    self.touchId = null;
		    self.active = false;
		}
        
		stick.addEventListener('mousedown', handleDown);
		stick.addEventListener('touchstart', handleDown);
		document.addEventListener('mousemove', handleMove, {passive: false});
		document.addEventListener('touchmove', handleMove, {passive: false});
		document.addEventListener('mouseup', handleUp);
		document.addEventListener('touchend', handleUp);
	}
    zmena(cislo,rozd = 5)
    {
        let strinZmenaJ = String("");
        let zozdJx = Number(self.value.x) - Number(self.old.x);
        let zozdJy = Number(self.value.y) - NUmber(self.old.y);
        let dJ = Math.sqrt(zozdJx * zozdJx + zozdJy * zozdJy);
        if(dJ > rozd)
        {
            self.old.x = self.value.x;
            self.old.y = self.value.y;
            strinZmenaJ = joySend(cislo,self.old.x,self.old.y,0);
        }
        return strinZmenaJ;
    }
    
}

let joystick1 = new JoystickController("stick1", 100, 4);
let joystick2 = new JoystickController("stick2", 100, 10);
let joystick3 = new JoystickController("stick3", 100, 10);
let joystick4 = new JoystickController("stick4", 100, 10);

function update()
{
	document.getElementById("status1").innerText = "Joystick 1: " + JSON.stringify(joystick1.value);
	document.getElementById("status2").innerText = "Joystick 2: " + JSON.stringify(joystick2.value);
    document.getElementById("status3").innerText = "Joystick 3: " + JSON.stringify(joystick3.value);
    document.getElementById("status4").innerText = "Joystick 4: " + JSON.stringify(joystick4.value);
    let stri = joystick1.zmena(1) + joystick2.zmena(2) + joystick3.zmena(3) + joystick4.zmena(4);
    console.log("Ldnxkijndjwsmkjm");
    if(stri != "")
    {
        //websocket.send(stri);
        console.log(stri);
    }   
    setTimeout(arguments.callee, 100);
}


function loop()
{
	requestAnimationFrame(loop);
	//update();
    
}
//setTimeout(update(), 1000);
loop();
update();

function hideRow(element2) {
    let element = document.getElementById('t' + element2.id.toString());
    let hidden = element.getAttribute("hidden");

    if (hidden) {
        element.removeAttribute("hidden");
        //button.innerText = "Hide tr";
    } else {
        element.setAttribute("hidden", "hidden");
        //button.innerText = "Show tr";
    }
}


