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

$(function () {
    checkCookie();
    // If user is logged in, check for cubes via spark.
    //$cubeOptions = $( '.cube-options' );
    //console.log(window.location.href);
    var isViz = window.location.href.indexOf("viz") >= 0;
    var isCreate = window.location.href.indexOf("create") >= 0;
    var isEdit = window.location.href.indexOf("edit") >= 0;
    var isPainter = window.location.href.indexOf("cube_painter") >= 0;
    var isPixels = window.location.href.indexOf("spark_pixels") >= 0;

    console.log("isPainter=" + isPainter);
    console.log("isPixels=" + isPixels);
    /*if (isPainter) {
        $('#cube_canvas').height($('#cube_canvas').width());
        $('#colors_canvas').height((($('#colors_canvas').width() / 8) * 3) + 1);
        $('div#colors').css('cssText', $('div#colors').attr('style')+'calc(100% - ' + $('#cube_canvas').height() + ') !important;');
        //$('div#colors').css('margin-top', 'calc(100% - ' + $('#cube_canvas').height() + ')');
    }*/
    if (typeof accessToken !== 'undefined' && accessToken !== null && (isViz || isCreate || isEdit || isPainter || isPixels)) {
        $('select#cubeName').show();
        listCubes();
        cubeNamesUpdateInterval = setInterval('listCubes()', 5000);  //update the list of cubes every 5 seconds
    }
    else {
        console.log("no cube options");
        $('select#cubeName').hide();
        if (typeof cubeNamesUpdateInterval !== 'undefined' && cubeNamesUpdateInterval !== null)
            clearInterval(cubeNamesUpdateInterval);
    }
});

function listCubes() {
    var devicesPr = spark.listDevices();
    var deviceInList = false;
    var deviceID = null;
    //var connectedCores = 0;
    //var accessToken = $.cookie("accessToken");

    devicesPr.then(
        function (devices) {
            //console.log('Devices: ', devices);
            if (devices.length === 0) {
                $("select#cubeName").empty().append($("<option></option>").val('').text('Add a photon to get started'));
            }
            else {
                devices.forEach(function (x, i, a) {
                    var device = devices[i];
                    // Is this device connected?
                    if (device.connected) {
                        //connectedCores++;
                        deviceType = (device.productId === '0' ? 'Core' : 'Photon');
                        deviceInList = $("select#cubeName option[value = '" + device.id + "']").length;

                        // Is this the first device found connected?
                        if (deviceID === null) {
                            // Fill the deviceID variable
                            deviceID = device.id;
                            // Remove the 'No cores online :(' item from the dropdown list
                            $("select#cubeName option[value='']").remove();
                        }

                        // Has this device not yet made it into the dropdown list?
                        if (!deviceInList) {
                            // Append the device name and ID to the dropdown list
                            $("select#cubeName").append($("<option></option>")
                                .val(device.id)
                                .attr("processor", deviceType)
                                .text("(" + deviceType + ") " + device.name))
                                .sort();
                        }
                    }//if (device.connected)
                    /*
                     * I've found that the first API call still often returns false negatives. So in order to cope
                     * with this malfunction, we need to certify that a device deemed "offline" by the promise call
                     * is really offline by querying that device individually; only then we can be sure it's offline.
                     */
                    else {
                        $.get("https://api.particle.io/v1/devices/" + device.id + "?access_token=" + accessToken, function (data) {
                            //console.log(data.name + ': ' + (data.connected ? 'connected' : 'not connected'));
                            if (!data.connected) {
                               $("select#cubeName option[value='" + device.id + "']").remove();
                               //$("select#cubeName").find("option:contains('" + device.name + "')").remove();
                            }
                            else {
                                /*
                                 * And yes, we'll have to do the same all over again, when we find a device's really online...
                                 */
                                deviceType = (device.productId === '0' ? 'Core' : 'Photon');
                                deviceInList = $("select#cubeName option[value = '" + device.id + "']").length;
                                
                                // Is this the first device found connected?
                                if (deviceID === null) {
                                    // Fill the deviceID variable
                                    deviceID = device.id;
                                    // Remove the 'No cores online :(' item from the dropdown list
                                    $("select#cubeName option[value='']").remove();
                                }

                                // Has this device not yet made it into the dropdown list?
                                if (!deviceInList) {
                                    // Append the device name and ID to the dropdown list
                                    $("select#cubeName").append($("<option></option>")
                                        .val(device.id)
                                        .attr("processor", deviceType)
                                        .text("(" + deviceType + ") " + device.name))
                                        .sort();
                                }
                            }
                        });
                    }
                });

                if ($("select#cubeName option").length === 0) {
                    if($('select#cubeName').val() !== '') {
                        $("select#cubeName").empty().append($("<option></option>").val('').text('No cores online :('));
                        $('select#cubeName').change();
                    }
                }
                else {
                    coreID = $.cookie("coreID");
                    var coreID_Before = coreID;
                    //console.log('coreID before: ' + coreID);
                    //console.log('deviceID: ' + deviceID);
                    if (typeof coreID === 'undefined' || coreID === null)
                        coreID = deviceID;
                    //console.log('coreID after: ' + coreID);
                    //console.log('#cubeName.value: ' + $('select#cubeName').val());
                    deviceInList = $("select#cubeName option[value = '" + coreID_Before + "']").length;
                    var onChange = (coreID_Before !== coreID || !deviceInList);
                    console.log('onChange: ' + onChange);
                    if (onChange) {
                        $('select#cubeName').change();
                    }
                }
            }
        },
        function (err) {
//             console.log('List devices call failed: ', err);
        }
    );
}

