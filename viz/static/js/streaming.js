var localIP;
var isStreaming=false;
var streaming = null;
var frame=0;
var r, g, b;
var port = 2525;

function getLocalIP(accessToken, coreID)
{
    url="https://api.spark.io/v1/devices/"+coreID+"/ip?access_token="+accessToken;
    $.get(url, function(data){
	    localIP=data['result'];
	    console.log(localIP);
	    streaming=new Streaming("ws://" + localIP + ":" + port);
	    streaming.rate=5;
	});
    
}

function stream()
{
    getLocalIP(accessToken, coreID);
}

function Streaming(address) {
    this.clearToSend = true;
    this.rate = 1000;
    this.size = 8; // TODO support 16^3
    this.frameSize = 512;
    var frameBuffer = new ArrayBuffer(512);

    // open connection
    this.ws = new WebSocket(address);
    console.log("Connecting!");
    //    var cube = this;

    this.ws.onclose = function() {
        if(this.onclose !== undefined) {
            this.onclose(this);
        }
    };

    this.ws.onopen = function() {
        if(streaming.onopen !== undefined) {
            this.onopen(this);
        }

        this.refresh();
    };

    this.ws.onmessage = function(evt) {
        var msg = evt.data;
        console.log("got msg: " + msg);

        if(parseInt(msg) == this.frameSize) {
            this.clearToSend = true;
	    console.log("clear to send");
        }
    };
}

Streaming.prototype.onopen= function() {}
Streaming.prototype.onclose= function() {}
Streaming.prototype.onrefresh= function() {}	
Streaming.prototype.refresh= function() {
    //    var cube = this;
    
    if(this.clearToSend && this.ws.bufferedAmount == 0) {
	if(this.onrefresh !== undefined) {
	    this.onrefresh(this);
	}
	//	console.log("sent frame "+frame);
	console.log(this.frameBuffer);
	this.ws.send(this.frameBuffer);
	this.clearToSend = false; // must get reply before sending again
	
	setTimeout(function() { cube.refresh(); }, cube.rate);
    } else {
	// check for readiness every 5 millis
	setTimeout(function() { cube.refresh(); }, 5);
    }
}

Streaming.prototype.bufferVoxels = function(red, green, blue)
{
    var frameView = new Uint8Array(this.frameBuffer);
    for(var x=0;x<8;x++)
	for(var y=0;y<8;y++)
	    for(var z=0;z<8;z++)
		this.frameBuffer[z*64+y*8+x]=(((red[x][y][z] >> 5) << 5) | ((green[x][y][z] >> 5) << 2) | (blue[x][y][z] >> 6)); 
    frame++;

}


