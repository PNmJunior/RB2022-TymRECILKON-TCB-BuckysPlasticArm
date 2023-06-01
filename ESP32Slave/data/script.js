// Complete project details: https://randomnerdtutorials.com/esp32-web-server-websocket-sliders/


//komunikační knihovna
const SouborZnak = ";";
const nahradaSouborZnak =" ///1||| ";
const SubSouborZnak =":";
const nahradaSubSouborZnak= " ///2||| " ;

const conzolChar = '#'

const M_LED = 0;
const M_1 = 1;
const M_2 = 2;
const M_3 = 3;
const M_4 = 4;
const M_Kleste = 5;
const M_Levy = 6;
const M_Pravy = 7;

const ModeWork = 'w';//work
//const ModeWork = 'd';//debug
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

function synchroSend()
{
    
    return sendAuto(dataSynchro());
}

function chatSend(t)
{
    return sendAuto(dataChat(t));
}

function joyAllSend(x1,  y1,  t1,  x2,  y2,  t2,  x3,  y3,  t3,  x4,  y4,  t4)
{
    return sendAuto(dataJoyAll( x1,  y1,  t1,  x2,  y2,  t2,  x3,  y3,  t3,  x4,  y4,  t4));
}

function dataJoyAll( x1,  y1,  t1,  x2,  y2,  t2,  x3,  y3,  t3,  x4,  y4,  t4)
{
    return['J',
    balicekInt(x1), balicekInt(y1), balicekInt(t1), 
    balicekInt(x2), balicekInt(y2), balicekInt(t2), 
    balicekInt(x3), balicekInt(y3), balicekInt(t3), 
    balicekInt(x4), balicekInt(y4), balicekInt(t4)
    ];
}

function dataChat(t)
{
    return ['c',balicekText( t)];
}


function dataJoy(j,x,y,t)
{
    return ['j',balicekInt( j),balicekInt( x),balicekInt( y),balicekInt( t)];
}

function dataMotorAll()
{
    return ['A'];
}

function dataSynchro()
{
    return ['q',balicekInt(((performance.now() - synchroTimeSend) / 100 ).toFixed(0))];
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

const Joystart = addSoubor("s");
const Joystop  = addSoubor("t");
let strJ1 ="";// ";s;j:1:50:10:1;t";//cteni
let Jstartindex = 0;
let Jstopindex = 0;
let strJ2 = "";
let Jmod = -1;

function readJoy()
{
    switch (Jmod) {
        case 0:
            strJ1 += "";//cteni
            Jstartindex = strJ1.indexOf(Joystart);
            if(Jstartindex != -1)
            {
                Jmod = 1;
                strJ2 = strJ1.substring(Jstartindex);
            }
            break;
        case 1:
            strJ2 += "";//cteni
            Jstopindex = strJ2.indexOf(Joystop); 
            if(Jstopindex != -1)
            {
                Jmod = 0;
                strJ1 = strJ2.substring(Jstopindex);
                SendESP(strJ2.substring(0,Jstopindex));
            }
        default:
            break;
    }
    setTimeout(arguments.callee, 10);
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

var synchroTimeSend = performance.now();
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
    Jmod = 0;
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


function sendChat()
{
    let lll = String(document.getElementById("fchat").value);
    if(lll != "")
    {
        SendESP(chatSend(lll));
    }
    
}

function sendChatPrikaz()
{
    let lll = String(document.getElementById("fchat").value);
    if(lll != "")
    {
        SendESP(chatSend(conzolChar+lll));
    }
    
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
    let Astring = synchroSend() + String(a);
    if(Astring == "")
    {
        //console.log("Prazdne pole");
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

function Jwork(ji,jx,jy, jt)
{
    switch (Ji) {
        case 1:
            joystick1.setSecund(Jx,Jy);
            break;
        case 2:
            joystick2.setSecund(Jx,Jy);
            break;
        case 3:
            joystick3.setSecund(Jx,Jy);
            break;
        case 4:
            joystick4.setSecund(Jx,Jy);
            break;
        default:
            break;
    }
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
            case "c":
                document.getElementById("ftextChat").value = ">>"+readText(SoubIn[1])+"\n" + document.getElementById("ftextChat").value;
                break;
            case "J":
                for(let i = 0; i < 4; i++)
                {
                    let Jx = readInt( SoubIn[1+3*i]);
                    let Jy = readInt( SoubIn[2+3*i]);
                    let Jt = readInt( SoubIn[3+3*i]);
                    Jwork(i+1,jx,jy,jt);
                }
                break;
            case "j":
                let Ji = readInt( SoubIn[1]);
                let Jx = readInt( SoubIn[2]);
                let Jy = readInt( SoubIn[3]);
                let Jt = readInt( SoubIn[4]);
                Jwork(ji,jx,jy,jt);
                break;
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
		this.value = { x: 0, y: 0 }; 

		let self = this;

        this.old = { x: 0, y: 0};

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
		    self.value = { x: xPercent, y: yPercent};
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
		    self.value = { x: 0, y: 0 };
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
    zmena(rozd = 5)
    {
        let zozdJx = Number(this.value.x) - Number(this.old.x);
        let zozdJy = Number(this.value.y) - Number(this.old.y);
        let dJ = Math.sqrt(zozdJx * zozdJx + zozdJy * zozdJy);
        if(dJ > rozd)
        {
            this.old.x = this.value.x;
            this.old.y = this.value.y;
            return [this.old.x,this.old.y,0];
        }
        return null;
    }
    prace()
    {
        return self.active;
    }

    setSecund(osa_x, osa_y)
    {
        if(!this.active)
        {
            let stick = document.getElementById(this.id);
            stick.style.transform = `translate3d(${Number(osa_x)}px, ${Number(osa_y)}px, 0px)`;
        }
        
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
    let joyout1 = joystick1.zmena();
    let joyout2 = joystick2.zmena();
    let joyout3 = joystick3.zmena();
    let joyout4 = joystick4.zmena();
    if(joyout1 != null && joyout2 != null && joyout3 != null && joyout4 != null )
    {
        SendESP(joyAllSend(
            joyout1[0],joyout1[1],joyout1[2],
            joyout2[0],joyout2[1],joyout2[2],
            joyout3[0],joyout3[1],joyout3[2],
            joyout4[0],joyout4[1],joyout4[2],
            ));
    }
    else if (joyout1 != null || joyout2 != null || joyout3 != null || joyout4 != null )
    {
        let mmmm;
        if(joyout1 != null)
        {
            mmmm += joySend(1,joyout1[0],joyout1[1], joyout1[2]);
        }
        if(joyout2 != null)
        {
            mmmm += joySend(2,joyout2[0],joyout2[1], joyout2[2]);
        }
        if(joyout3 != null)
        {
            mmmm += joySend(3,joyout3[0],joyout3[1], joyout3[2]);
        }
        if(joyout4 != null)
        {
            mmmm += joySend(4,joyout4[0],joyout4[1], joyout4[2]);
        }
        SendESP(mmmm)
    }
    else if(joyout1.prace() || joyout2.prace() || joyout3.prace() || joyout4.prace() )
    {
        SendESP("");
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
readJoy();
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


