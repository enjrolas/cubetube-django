var localIP;
var isStreaming=false;
cube = null;
var frame=0;
var r, g, b;
var frameBuffer = new ArrayBuffer(512);
var port = 2525;

function getLocalIP(accessToken, coreID)
{
    url="https://api.spark.io/v1/devices/"+coreID+"/ip?access_token="+accessToken;
    $.get(url, function(data){
	    localIP=data['result'];
	    console.log(localIP);
	    cube=new Cube("ws://" + localIP + ":" + port);
	    cube.rate=5;
	});
    
}

function stream()
{
    getLocalIP(accessToken, coreID);
}

function bufferVoxels(red, green, blue)
{
    for(x=0;x<8;x++)
	for(y=0;y<8;y++)
	    for(z=0;z<8;z++)
		frameBuffer[z*64+y*8+x]=((red[x][y][z] >> 5) << 5) | ((green[x][y][z] >> 5) << 2) | (blue[x][y][z] >> 6); 
    if(frame===1)
	console.log(frameBuffer);
    frame++;

}

function Cube(address) {
    this.clearToSend = true;
    this.rate = 1000;
    this.size = 8; // TODO support 16^3
    this.frameSize = 512;

    // open connection
    this.ws = new WebSocket(address);
    console.log("Connecting!");

    var cube = this;

    this.ws.onclose = function() {
        if(cube.onclose !== undefined) {
            cube.onclose(cube);
        }
    };

    this.ws.onopen = function() {
        if(cube.onopen !== undefined) {
            cube.onopen(cube);
        }

        cube.refresh();
    };

    this.ws.onmessage = function(evt) {
        var msg = evt.data;
        console.log("got msg: " + msg);

        if(parseInt(msg) == cube.frameSize) {
            cube.clearToSend = true;
	    console.log("clear to send");
        }
    };
}

Cube.prototype.onopen= function() {}
Cube.prototype.onclose= function() {}
Cube.prototype.onrefresh= function() {}	
Cube.prototype.refresh= function() {
    var cube = this;
    
    if(this.clearToSend && this.ws.bufferedAmount == 0) {
	if(this.onrefresh !== undefined) {
	    this.onrefresh(this);
	}
	console.log("sent frame "+frame);
	console.log(frameBuffer);
	this.ws.send(frameBuffer);
	this.clearToSend = false; // must get reply before sending again
	
	setTimeout(function() { cube.refresh(); }, cube.rate);
    } else {
	// check for readiness every 5 millis
	setTimeout(function() { cube.refresh(); }, 5);
    }
}
