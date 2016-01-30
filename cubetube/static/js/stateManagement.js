/**
 * Javascript state management. For applicable areas.
 * - Viz page cube management
 */

var isActive = true;

window.onfocus = function () {
    isActive = true;
};
window.onblur = function () {
    isActive = false;
};

$(function(){
	checkCookie();
	// If user is logged in, check for cubes via spark.
	$cubeOptions = $( '.cube-options' );
	if( $cubeOptions.length ) {
		listCubes();
		setInterval('listCubes()', 15000);  //update the list of cubes every 15 seconds
	}
	else
	    console.log("no cube options");
})

function listCubes() {
    var devicesPr = spark.listDevices();
    var deviceInList = false;
    var deviceID;
    var connectedCores = 0;
    var accessToken=$.cookie("accessToken");

    devicesPr.then(
        function(devices) {
		    $("#cubeName").empty();	//clear the list
		    //console.log('Devices: ', devices);
            if(devices.length == 0) {
            	$("#cubeName").append($("<option></option>").html('Add a photon to get started'));
            }
            else {
            	devices.forEach(function(x,i,a) {
            		var device = devices[i];
            		if(!(accessToken === undefined || accessToken === null)) {
            			$.get("https://api.particle.io/v1/devices/" + device.id + "?access_token=" + accessToken, function(data) {
            				console.log(data.name + ': ' + (data.connected ? 'connected' : 'not connected'));
            				if(data.connected) {
            					connectedCores++;
            					$("#cubeName").find("option:contains('No cores online :(')").remove();
	    			            if( deviceID != 'undefined' ) {
	    			                deviceID=device.id;
	    			                //console.log(deviceID);
	    		                }
            					
            					deviceType = (device.productId === '0' ? 'Core' : 'Photon');
    							//append the cube name and ID to thr dropdown list
	    						if(!$("#cubeName").find("option:contains('" + device.name + "')").length)
	    							$("#cubeName").append($("<option></option>").val(device.id).attr("processor", deviceType).html("(" + deviceType + ") " + device.name));
	    						
	    						if(device.name == coreID)
	    				            deviceInList = true;
            				}
            			});
            		}
            	});
	
	            if( deviceInList === true ) {
	            	$('#cubeName').val(coreID); 
	            	$('#cubeName').change();
	            } else {
	            	$('#cubeName').val(deviceID);
	            	coreID = deviceID;
	            	var date = new Date();
	            	$.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});   
	            	$('#cubeName').change();
	            }
            }
            if(connectedCores === 0) {
            	$("#cubeName").append($("<option></option>").html('No cores online :('));  //append the cube name and ID to thr dropdown list
            }
        },
        function(err) {
//             console.log('List devices call failed: ', err);
        }
    );
}

