/**
 * Javascript state management. For applicable areas.
 * - Viz page cube management
 */

$(function(){

    // If user is logged in, check for cube's via spark.
    $cubeOptions = $( '.cube-options' );
    if( $cubeOptions.length ) {
        // listCubes();
        console.log("hi");
    }
})

function listCubes() {
    var devicesPr = spark.listDevices();
    var deviceInList = false;
    var deviceID;
    var connectedCores = 0;

    devicesPr.then(

        function(devices){
            
            //console.log('Devices: ', devices);
            for( var i = 0; i < devices.length; i++ ) {
        
                var device = devices[i];
                if( device.connected ){
                    connectedCores++;
                
                    if( deviceID != 'undefined' ) {
                        deviceID=device.id;
                        // console.log(deviceID);
                    }

                    // console.log(device.name+" is connected");
                    // $("#cubeName").append($("<option></option>").val(device.id).html(device.name));  //append the cube name and ID to thr dropdown list

                    if(device.name == coreID) {
                        deviceInList = true;
                    }
                }
            }

            if( deviceInList == true ) {
                // $('#cubeName').val(coreID);     
            } else {
                // $('#cubeName').val(deviceID);       
                coreID = deviceID;
                var date = new Date();
                 $.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});   
            }
    
            if(devices.length==0) {
                // $("#cubeName").append($("<option></option>").html('Add a core to your spark account to get started'));  //append the cube name and ID to thr dropdown list
            } else if(connectedCores==0) {
                // $("#cubeName").append($("<option></option>").html('None of your cores are online right now'));  //append the cube name and ID to thr dropdown list
            }
        },
        function(err) {
            // console.log('List devices call failed: ', err);
        }
    );
}

function checkCookie() {
    accessToken = $.cookie("accessToken");
    username = $.cookie("username");
    nickname = $.cookie("nickname");
    coreID = $.cookie("coreID");
}