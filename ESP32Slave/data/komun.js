var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onload);


const Amask   =                    parseInt(11000000,2);
const Amotor    =                  parseInt(00000000,2);

const Atext        =               parseInt(01000000,2);
const AsetMotor    =               parseInt(10000000,2);
const AsetView     =               parseInt(11000000,2);
const Aposun = 6;
const Bmask = parseInt(0100000,2);
const BsMax_smerVpred_tOst  =      parseInt(0000000,2);
const BsMin_smerVzad_err     =     parseInt(0100000,2);
const Bposun  =5;
const Cmask = parseInt(0010000,2);
const Cano_brzda_frek_krok_ost_chat   =    parseInt(0010000,2);
const Cne_brzda_frek_krok_ost_inf     =    parseInt(0000000,2);
const Cposun  = 4;
const INDEXmask = parseInt(0001111,2);






function onload(event) {
    initWebSocket();
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.binaryType = "arraybuffer"
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

var dataFace = {
    AtypeNum:0,
    BtypeBool:Boolean(false),
    CtypeBool:Boolean(false),
    Index:0,
    Data:0,
    text:"",
    len:0,
};

function onMessage(event) {
    if (event.data instanceof ArrayBuffer) {
        // binary frame
        const view = new DataView(event.data);
        console.log(view.getInt32(0));
    } 
    else 
    {
        console.log("Problema maxima 1");
        console.log(event.data);
    }
    var dataFaceIN = {
        AtypeNum:0,
        BtypeBool:Boolean(false),
        CtypeBool:Boolean(false),
        Index:0,
        Data:0,
        text:"",
        len:0,
    };
    console.log(event.data);
    var bytes = new Uint8Array(data);
    var debugBytes = "";
    for(var i=0; i<bytes.byteLength; i++) {
        debugBytes.concat(bytes[i].toString());
        debugBytes.concat(",");
    }

    console.log('Processing packet [' + bytes[1] + '] ' + debugBytes);
    var ind = 0;
    
    for(var i )
    do{

        ind = ind + 1;
        dataFace.AtypeNum

    }while()





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


function DT(dataIN)
{
    var dataFaceIN = {
        AtypeNum:0,
        BtypeBool:Boolean(false),
        CtypeBool:Boolean(false),
        Index:0,
        Data:0,
        text:"",
        len:0,
    };
    var i = 0;
    do{

        ind = ind + 1;
        dataFaceIN.AtypeNum =  (dataIN[0 + i] & Amask) >> Aposun;
        if((dataIN[0 + i] & Bmask) != 0)
        {
        dataFaceIN.BtypeBool = Boolean(1);
        }
        else
        {
            dataFaceIN.BtypeBool = Boolean(0);
        } 
        if((dataIN[0 + i] & Cmask) != 0)
        {
        dataFaceIN.CtypeBool = Boolean(1);
        }
        else
        {
            dataFaceIN.CtypeBool = Boolean(0);
        } 
        dataFaceIN.Index = dataIN[0 + i] & INDEXmask;
        dataFaceIN.Data = dataIN[0 + i];
        i =i + 2;
        if(dataFaceIN.AtypeNum = 1)
        {
            dataFaceIN.text = toString();
            for(var o = 0; o < dataFaceIN.Data; o++)
            {
                dataFaceIN.text = dataFaceIN.text.concat(dataIN.text[o].toString());
                i = i +1;
            }
        }
        workIn(dataFaceIN);

    }while(i<dataIN.len())
}

function workIn(faceIn)
{
    switch(faceIn.AtypeNum) {
        case 0:
          // motor
          if(faceIn.BtypeBool == 1)
          {
            sliderIN(faceIn.index, faceIn.data);
          }
          else
          {
            sliderIN(faceIn.index, faceIn.data * (-1));
          }
          
          break;
        case 1:
          console.log(faceIN.text);
          break;
        default:
          // code block
      } 
}

function sliderIN(index, hod)
{
    document.getElementById("sV"+ index.toString()).innerHTML = hod.toString();
    document.getElementById("slider"+index.toString()).value = hod.toString();
}