/**
 * Javascript state management. For applicable areas.
 * - Viz page cube management
 */

$(function(){
	checkCookie();
	// If user is logged in, check for cubes via spark.
		$cubeOptions = $( '.cube-options' );
	if( $cubeOptions.length ) {
	    //	    listCubes();
	    setInterval('listCubes()',15000);  //update the cube list every 15 seconds
	    }
	else
	    console.log("no cube options");
    })

function listCubes() {

    var devicesPr = spark.listDevices();
    var deviceInList = false;
    var deviceID;
    var connectedCores = 0;

    devicesPr.then(

        function(devices){
            
            console.log('Devices: ', devices);
            for( var i = 0; i < devices.length; i++ ) {
        
                var device = devices[i];
                if( device.connected ){
                    connectedCores++;
                
                    if( deviceID != 'undefined' ) {
                        deviceID=device.id;
                         console.log(deviceID);
                    }

                     console.log(device.name+" is connected");
                     $("#cubeName").append($("<option></option>").val(device.id).html(device.name));  //append the cube name and ID to thr dropdown list

                    if(device.name == coreID) {
                        deviceInList = true;
                    }
                }
            }

            if( deviceInList == true ) {
                 $('#cubeName').val(coreID);     
            } else {
                 $('#cubeName').val(deviceID);       
                coreID = deviceID;
                var date = new Date();
                 $.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});   
            }
    
            if(devices.length==0) {
                $("#cubeName").append($("<option></option>").html('Add a core to get started'));  //append the cube name and ID to thr dropdown list
            } else if(connectedCores==0) {
                 $("#cubeName").append($("<option></option>").html('No cores online :('));  //append the cube name and ID to thr dropdown list
            }
        },
        function(err) {
             console.log('List devices call failed: ', err);
        }
    );
}

