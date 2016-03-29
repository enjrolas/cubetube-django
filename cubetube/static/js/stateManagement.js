/**
 * Javascript state management. For applicable areas.
 * - Viz page cube management
 */

var isActive = true;
var cubeNamesUpdateInterval = null;

window.onfocus = function () {
    isActive = true;
};
window.onblur = function () {
    isActive = false;
};

$(function(){
	checkCookie();
	// If user is logged in, check for cubes via spark.
	//$cubeOptions = $( '.cube-options' );
	//console.log(window.location.href);
	var isViz = window.location.href.indexOf("viz") >= 0;
	var isCreate = window.location.href.indexOf("create") >= 0;
	var isEdit = window.location.href.indexOf("edit") >= 0;
	var isPainter = window.location.href.indexOf("cube_painter") >= 0;
	if(typeof accessToken !== 'undefined' && accessToken !== null && (isViz || isCreate || isEdit || isPainter)) {
		$('select#cubeName').show();
		listCubes();
		cubeNamesUpdateInterval = setInterval('listCubes()', 5000);  //update the list of cubes every 5 seconds
	}
	else {
	    console.log("no cube options");
	    $('select#cubeName').hide();
	    if(typeof cubeNamesUpdateInterval !== 'undefined' && cubeNamesUpdateInterval !== null)
	    	clearInterval(cubeNamesUpdateInterval);
	}
});

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
            	$("select#cubeName").empty().append($("<option></option>").html('Add a photon to get started'));
            }
            else {
            	devices.forEach(function(x,i,a) {
            		var device = devices[i];
            		if(typeof accessToken !== 'undefined' && accessToken !== null) {
            			$.get("https://api.particle.io/v1/devices/" + device.id + "?access_token=" + accessToken, function(data) {
            				//console.log(data.name + ': ' + (data.connected ? 'connected' : 'not connected'));
            				if(data.connected) {
            					//connectedCores++;
            					$("select#cubeName option[value='-1']").remove();
            					deviceType = (device.productId === '0' ? 'Core' : 'Photon');
            					deviceInList = $("select#cubeName option[value = '" + device.id + "']").length;
								if( typeof deviceID !== 'undefined' && deviceID !== null ) {
									deviceID=device.id;
									//console.log(deviceID);
								}            					
								    
    							//append the cube name and ID to thr dropdown list
    							if(!deviceInList) {
	    							$("select#cubeName").append($("<option></option>")
	    								.val(device.id)
	    								.attr("processor", deviceType)
	    								.html("(" + deviceType + ") " + device.name))
	    								.sort();
    							}
            				}
            				else {
            					$("select#cubeName").find("option:contains('" + data.name + "')").remove();
            				}
            			});
            		}
            	});
            	
            	//console.log('cubeName items count: ' + $("#cubeName option").length);
            	//console.log('connectedCores: ' + connectedCores);
            	if($("select#cubeName option").length === 0) {
                	$("select#cubeName").empty()
                		.append($("<option></option>")
                		.val('-1').html('No cores online :('));
                	if($('select#cubeName').length) 
                		$('select#cubeName').change();
                }
                else {
	            	coreID = $.cookie("coreID");
	            	if( typeof coreID !== 'undefined' && coreID !== null )
	            		coreID = $('select#cubeName').val();
	            	if( typeof deviceID !== 'undefined' && deviceID !== null )
	            		deviceID = coreID;
	            	//console.log('coreID: ' + coreID);
					var valueChanged = $('select#cubeName').val() !== coreID;
					if(valueChanged) {
						$('select#cubeName').val(coreID);
						if($('select#cubeName').length)
							$('select#cubeName').change();
					}
                }
            }
        },
        function(err) {
//             console.log('List devices call failed: ', err);
        }
    );
}

