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
    //var connectedCores = 0;
    var accessToken=$.cookie("accessToken");

    devicesPr.then(
        function(devices) {
		    //console.log('Devices: ', devices);
            if(devices.length == 0) {
            	$("#cubeName").empty().append($("<option></option>").html('Add a photon to get started'));
            }
            else {
            	devices.forEach(function(x,i,a) {
            		var device = devices[i];
            		if(typeof accessToken !== 'undefined' && accessToken !== null) {
            			$.get("https://api.particle.io/v1/devices/" + device.id + "?access_token=" + accessToken, function(data) {
            				console.log(data.name + ': ' + (data.connected ? 'connected' : 'not connected'));
            				if(data.connected) {
            					//connectedCores++;
            					//$("#cubeName").find("option:contains('No cores online :(')").remove();
            					$("#cubeName").find("option[value='-1']").remove();
	    			            if( typeof deviceID !== 'undefined' && deviceID !== null ) {
	    			                deviceID=device.id;
	    			                //console.log(deviceID);
	    		                }
            					deviceType = (device.productId === '0' ? 'Core' : 'Photon');
            					
            					$('#cubeName option').each(function() {
            						if($(this).val() == device.id) {
            							deviceInList = true;
            						}
            					});
            					/*if(device.name == coreID)
								    deviceInList = true;*/
								    
    							//append the cube name and ID to thr dropdown list
    							if(!deviceInList) {
	    							$("#cubeName").append($("<option></option>")
	    								.val(device.id)
	    								.attr("processor", deviceType)
	    								.html("(" + deviceType + ") " + device.name))
	    								.sort();
    							}
            				}
            				else {
            					$("#cubeName").find("option:contains('" + data.name + "')").remove();
            					//$('#cubeName').change();
            				}
            			});
            		}
            	});
            	
            	//console.log('cubeName items count: ' + $("#cubeName option").length);
            	//console.log('connectedCores: ' + connectedCores);
            	if($("#cubeName option").length === 0) {
                	$("#cubeName").empty()
                		.append($("<option></option>")
                		.val('-1').html('No cores online :('));
                }
                else {
	            	coreID = $.cookie("coreID");
	            	if( typeof coreID === 'undefined' || coreID === null ) {
		            	$('#cubeName').val(deviceID);
		            	coreID = deviceID;
		            	var date = new Date();
		            	$.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});
	            	}
            		$('#cubeName').val(coreID);
            	}
            }
        },
        function(err) {
//             console.log('List devices call failed: ', err);
        }
    );
}

