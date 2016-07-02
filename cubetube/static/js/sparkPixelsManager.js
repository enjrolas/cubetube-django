var isUserAlerted = false;
var currentMode = '';
var brightness = 20;
var speed = 5;
var modeList;
var modeParmList;
var auxSwtchList = new Array();
var populateModesTimer = null;
var syncInterval = null;
var supportsLocalStorage = function() { return ('localStorage' in window) && window['localStorage'] !== null; };

function AuxSwitch(id, description, onLabel, offLabel, value) {
    this.id = id;
    this.description = description;
    this.onLabel = onLabel;
    this.offLabel = offLabel;
    this.value = value;
}        

function showHideAuxSwitchPanel() {
    if($('div.aux-panel-popover').css('display') !== 'none') {
        $("img#auxWaitIcon").fadeOut(150, function() { $("img#auxSwitchIcon").fadeIn(300); });
        $('div.aux-panel-popover').slideUp(300, 'linear');
    }
    else {
        if (typeof accessToken !== 'undefined' && accessToken !== null)
            $("img#auxSwitchIcon").fadeOut(150, function() { $("img#auxWaitIcon").fadeIn(300) });
            getAuxSwitches();
    }
}

function updateAuxSwitchPanel() {
    $("div.inputs").html('');
    $('div.error-area').text('');
    for(var idx = 0; idx < auxSwtchList.length; idx++) {
        var auxSwitch = $("<span style=\"padding-right: 5px;\"><input type=\"checkbox\" onchange=\"setAuxSwitches('" + auxSwtchList[idx].id + "', this.checked);\" style=\"vertical-align: middle;\" " + (auxSwtchList[idx].value === '1' ? "checked" : '') + " ><span style=\"padding-left: 2px;\" >" + auxSwtchList[idx].description + "</span></span><br>");
        $("div.inputs").append(auxSwitch);
    }
}

function setTimeZoneOffset() {
    var d = new Date()
    var n = d.getTimezoneOffset();
    var localOffset = -(n/60);
    //console.log('getTimezoneOffset(): ' + n);
    //console.log('localOffset: ' + localOffset);
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        var commandString = 'SETTIMEZONE:' + localOffset;
        $.post("https://api.particle.io/v1/devices/" + deviceID + "/Function", {
                access_token: accessToken, args: commandString
        }).success(function (data) {
            //console.log('success! localOffset: ' + data.return_value);
            $("span#device").text('Timezone offset updated to ' + data.return_value);
            $("div#cubeAndModeText").show('slide', {direction: 'left'}, 600).delay(1200).hide('slide', {direction: 'right'}, 600);
        }).fail(function (data) {
            $("span#device").text('Error: Timezone not updated');
            $("div#cubeAndModeText").show('slide', {direction: 'left'}, 600).delay(1200).hide('slide', {direction: 'right'}, 600);
            console.log('fail: ' + data.return_value);
        });
    }
}

function setAuxSwitches(id, checked) {
    $('div.error-area').text('');
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        var commandString = 'SETAUXSWITCH:' + id + ',' + (checked ? '1' : '0') + ';';
        $.post("https://api.particle.io/v1/devices/" + deviceID + "/Function", {
            access_token: accessToken, args: commandString
        }).fail(function (data) {
            $('div.error-area').text('Oh-oh! Could not update Aux Switches\' states');
            console.log('fail: ' + data.return_value);
        });
        console.log('commandString: ' + commandString);
        //updateAuxSwitchPanel();
    }
}

function getAuxSwitches() {
    var deviceID = getDeviceID();

    // Retrieve the device's aux switches list from the cloud API and update array variable
    $.get("https://api.particle.io/v1/devices/" + deviceID + "/auxSwtchList/?access_token=" + accessToken, function (data) {
        console.log('success: ' + data.result);
        var tmpAuxSwtch = data.result.slice(0, data.result.lastIndexOf(';')).split(';');
        if(tmpAuxSwtch.length > 0) {
            auxSwtchList = new Array();
            tmpAuxSwtch.forEach( function(item, index) { 
                var args = item.split(',');
                auxSwtchList.push(new AuxSwitch(args[0], args[1], args[2], args[3], args[4]));
            });
            $("img#auxWaitIcon").fadeOut(150, function() { $("img#auxSwitchIcon").fadeIn(300) });
            if(auxSwtchList.length > 0) {
                updateAuxSwitchPanel();
                $('div.aux-panel-popover').slideDown(300, 'linear');
            }
        }
    }).fail(function (data) {
            $("img#auxWaitIcon").fadeOut(150, function() { $("img#auxSwitchIcon").fadeIn(300) });
            console.log('fail: ' + data.result);
            var message = '';
            var responseText = data.responseText;
            if (responseText.indexOf("Variable not found") >= 0) {
                message += 'The selected Cube (\"' + device.substr(device.indexOf(')') + 2)
                    + '\") does not appear to be running the Spark Pixels viz (it may be the Particle API acting up).';
                message += '\nIf you would like to flash \"' + device.substr(device.indexOf(')') + 2) + '\" with ';
                message += 'Spark Pixels now, just hit [OK].\nElse, click [Cancel] to return to where you were.';
                if (!isUserAlerted) {
                    isUserAlerted = true;
                    if (confirm(message))
                        flashCube();
                    else
                        clearInterval(populateModesTimer);
                }
            }
            else {
                $('div.error-area').text("Oh-oh! Could not retrieve Aux Switches' states");
                $('div.aux-panel-popover').slideDown(300, 'linear');
            }
        });
}

function populateInterval() {
    populateModesTimer = setInterval(function () {
        if (currentMode === null || currentMode === '' || currentMode === 'None') {
            $("select#modes").fadeOut(100);
            $("div#brightnessControl").slideUp(100);
            $("div#speedControl").slideUp(100);
            $("a#updateListButton").html("Please Wait<span class=\"one\">.</span><span class=\"two\">.</span><span class=\"three\">.</span>");
            //$("span#mode").html("Please Wait<span class=\"one\">.</span><span class=\"two\">.</span><span class=\"three\">.</span>");
            populateModes();
        }
        else {
            if ($("select#modes option:selected").val() === currentMode) {
                clearInterval(populateModesTimer);
                isUserAlerted = true;
                $("a#updateListButton").html("Update List");
                //$("span#mode").html(currentMode);
                $("div#updateListDiv").fadeIn(300);
                $("select#modes").fadeIn(300);
                $('select#cubeName').fadeIn(300);
                $("div#cubeAndModeText").hide('slide', {direction: 'right'}, 600); //fadeIn(300);
                //console.log('$("select#modes option:selected").val() = ' + $("select#modes option:selected").val());
                clearModeOptions();
                for(var id=1; id<=6; id++) {
                    getColor(id);
                    if(id <= 4)
                        getSwitchState(id);
                }
                var idx = $("select#modes option:selected").index();
                if(modeParmList[idx] !== 'N')
                    parseModeOptions();
                getSpeed();         //setSpeed();
                getBrightness();    //setBrightness();
                setTimeZoneOffset();
            }
            else {
                // Set the selected mode in the modes dropdown
                // This can fail: 
                //$("select#modes").val(currentMode);
                $.each($("select#modes option"), function (index, option) {
                    option.selected = false;
                });
                $.each($("select#modes option"), function (index, option) {
                    //console.log('option: ' + option.value);
                    if (option.value === currentMode) {
                        option.selected = true;
                        return false;
                    }
                });
            }
        }
    }, 1000);
}

function populateModes() {
    var deviceID = getDeviceID();
    if (deviceID !== '') {
        // Retrieve the device's current mode from the cloud API
        $.get("https://api.particle.io/v1/devices/" + deviceID + "/mode/?access_token=" + accessToken, function (data) {
            //console.log(data.result);
            currentMode = data.result.trim();
            console.log('currentMode: ' + currentMode);
        }, "json").fail(function (data) {   // API call failed to get the mode variable from the cloud;
                var message = '';           // device is likely not running Spark Pixels, or it hasn't
                if (deviceID !== '') {      // successfully published the 'mode' cloud variable.
                    var device = $("select#cubeName option[value = '" + deviceID + "']").text();
                    //if (typeof device !== 'undefined' && device !== null && device !== '') {
                    /*for ( var prop in data ) {
                        console.log( "data." + prop );
                    }
                    console.log("responseText: " + data.responseText);
                    console.log("responseJSON: " + data.responseJSON);*/
                    var responseText = data.responseText;
                    if (responseText.indexOf("Variable not found") >= 0) {
                        message += 'The selected Cube (\"' + device.substr(device.indexOf(')') + 2)
                            + '\") does not appear to be running the Spark Pixels viz (it may be the Particle API acting up).';
                        message += '\nIf you would like to flash \"' + device.substr(device.indexOf(')') + 2) + '\" with ';
                        message += 'Spark Pixels now, just hit [OK].\nElse, click [Cancel] to return to where you were.';
                        if (!isUserAlerted) {
                            isUserAlerted = true;
                            if (confirm(message))
                                flashCube();
                            else
                                clearInterval(populateModesTimer);
                        }
                    }
                    else {
                        if (!isUserAlerted) {
                            isUserAlerted = true;
                            message = '';
                            message+="Oh-oh! I tried to get the cube's current MODE from the Particle cloud, but it would not retrieve it!";
                            message+="\nThis could be due to a network error. If your cube is breathing cyan,\nit may be the Particle API acting up.";
                            message+="\nClick [OK] to retry, [Cancel] to give it a rest for a while.";
                            if (confirm(message))
                                window.location.reload();
                            else
                                clearInterval(populateModesTimer);
                        }
                    }
                }
                else {
                    if (!isUserAlerted) {
                        isUserAlerted = true;
                        message = '';
                        message+="Oh-oh! I tried to get the cube's DEVICE ID, but the Particle cloud reported it as OFFLINE!";
                        message+="\nThis could be due to a network error. If your cube is breathing cyan,\nit may be the Particle API acting up.";
                        message+="\nClick [OK] to retry, [Cancel] to give it a rest for a while. You may try resetting your cube prior to this.";
                        if (confirm(message))
                            window.location.reload();
                        else
                            clearInterval(populateModesTimer);
                    }
                }
            });
        // Retrieve the device's modes list from the cloud API
        $.get("https://api.particle.io/v1/devices/" + deviceID + "/modeList/?access_token=" + accessToken, function (data) {
            console.log('success: ' + data.result.slice(0, data.result.lastIndexOf(';')));
            modeList = data.result.split(";");
            $("select#modes").empty();  //clear all the existing modes before appending new ones
            for (var index = 0; index < modeList.length - 1; index++) {
                var mode = modeList[index].trim();
                $("select#modes").append("<option value=\"" + mode + "\">" + mode + "</option>");
            }
            //console.log('currentMode: ' + currentMode);
        }, "json").fail(function (data) {
            console.log('fail: ' + data.result);
            var message = '';
            var device = $("select#cubeName option[value = '" + deviceID + "']").text();
            var responseText = data.responseText;
            if (responseText.indexOf("Variable not found") >= 0) {
                message += 'The selected Cube (\"' + device.substr(device.indexOf(')') + 2)
                    + '\") does not appear to be running the Spark Pixels viz (it may be the Particle API acting up).';
                message += '\nIf you would like to flash \"' + device.substr(device.indexOf(')') + 2) + '\" with ';
                message += 'Spark Pixels now, just hit [OK].\nElse, click [Cancel] to return to where you were.';
                if (!isUserAlerted) {
                    isUserAlerted = true;
                    if (confirm(message))
                        flashCube();
                    else
                        clearInterval(populateModesTimer);
                }
            }
            else {
                if (!isUserAlerted) {
                    isUserAlerted = true;
                    message = '';
                    message+="Oh-oh! I tried to get the list of MODES from the Particle cloud, but it would not retrieve it!";
                    message+="\nThis could be due to a network error. If your cube is breathing cyan,\nit may be the Particle API acting up.";
                    message+="\nClick [OK] to retry, [Cancel] to give it a rest for a while.";
                    if (confirm(message))
                        window.location.reload();
                    else
                        clearInterval(populateModesTimer);
                }
            }
        });
        // Retrieve the device's parameters list from the cloud API
        $.get("https://api.particle.io/v1/devices/" + deviceID + "/modeParmList/?access_token=" + accessToken, function (data) {
            console.log('success: ' + data.result.slice(0, data.result.lastIndexOf(';')));
            modeParmList = data.result.split(";");
        }, "json").fail(function (data) {
            console.log('fail: ' + data.result);
            var device = $("select#cubeName option[value = '" + deviceID + "']").text();
            var message = '';
            var responseText = data.responseText;
            if (responseText.indexOf("Variable not found") >= 0) {
                message += 'The selected Cube (\"' + device.substr(device.indexOf(')') + 2)
                    + '\") does not appear to be running the Spark Pixels viz (it may be the Particle API acting up).';
                message += '\nIf you would like to flash \"' + device.substr(device.indexOf(')') + 2) + '\" with ';
                message += 'Spark Pixels now, just hit [OK].\nElse, click [Cancel] to return to where you were.';
                if (!isUserAlerted) {
                    isUserAlerted = true;
                    if (confirm(message))
                        flashCube();
                    else
                        clearInterval(populateModesTimer);
                }
            }
            else {
                if (!isUserAlerted) {
                    isUserAlerted = true;
                    message = '';
                    message+="Oh-oh! I tried to get the list of PARAMETERS from Particle cloud, but it would not retrieve it!";
                    message+="\nThis could be due to a network error. If your cube is breathing cyan,\nit may be the Particle API acting up.";
                    message+="\nClick [OK] to retry, [Cancel] to give it a rest for a while.";
                    if (confirm(message))
                        window.location.reload();
                    else
                        clearInterval(populateModesTimer);
                }
            }
        });
    }
}

function flashCube() {
    var deviceID = getDeviceID();
    if (deviceID !== '') {
        var output;
        var request = $.ajax({
            type: "POST",
            url: "/cloudFlash/", /*"{% url 'cloudFlash' %}",*/
            data: {"code": "", "accessToken": accessToken, "deviceID": deviceID, "vizName": "", "vizId": "1438"},
            dataType: "text",
            success: function (data) {
                console.log("success flashing");
                JSONstring = data;
                console.log(JSONstring);
                var result = $.parseJSON(data);
                var result = $.parseJSON(result);
                compilerData = result;
                console.log(compilerData);
                output = "";
                if (result.ok) {
                    output += "Compilation status: " + result.message + " - Flashing device now, please wait.";
                    output += "\nOnce your Cube has restarted, you can close this message.";
                }
                else {
                    var device = $("select#cubeName option[value = '" + deviceID + "']").text();
                    output += "Compilation status: " + result.ok + "\nerrors:\n" + result.errors + "\n";
                    output += "\nPlease make sure " + device.substr(device.indexOf(')') + 2) + " is online (breathing cyan).";
                }
                alert(output);
                window.location.reload();
            },
            error: function (data) {
                console.log("fail");
                console.log(data);
            }
        });
    }
}

function clearModeOptions() {
    // Hide the colors and switches
    for(var count = 1; count <= 6; count++) {
        $("span#color" + count).hide();
        if(count <= 4) { $("span#switch" + count).hide(); }
    }
    // Hide the text input field
    $("div#text").hide();
}

function parseModeOptions() {
        var idx = $("select#modes option:selected").index();
        var options = modeParmList[idx].split(',');
        var numColors, numSwitches;
        $.each(options, function(index, value) {
            //console.log("option " + index + ": " + value);
            switch(value[0]) {
                case 'C':
                    numColors = Number(value[2]);
                    for(var count = 1; count <= numColors; count++)
                        $("span#color" + count).show();
                    break;
                case 'S':
                    numSwitches = Number(value[2]);
                    for(var count = 1; count <= numSwitches; count++) {
                        //console.log("switchLabels: " + options[index+1]);
                        var switchLabels = options[index+1].replace(/\"\"/g, ";").split(';');
                        $("span#switchLbl" + count).text(switchLabels[count-1].replace(/\"/g, ''));
                        $("span#switch" + count).show();
                    }
                    break;
                case 'T':
                    $("div#text").show();
                    break;
                default:
                    break;
            }
        });
        $("a#setModeButton").text("Start " + currentMode + " Mode!");
        $("a#setModeButton").attr("title", "Click to set your Cube to " + currentMode + " mode with the chosen options");
        $("div#params").slideDown("300", "swing");
}

function parseRGBToHexString(r, g, b) {
    var rHex = Number(r).toString(16);
    var gHex = Number(g).toString(16);
    var bHex = Number(b).toString(16);
    rHex = rHex.length > 1 ? rHex : '0' + rHex;
    gHex = gHex.length > 1 ? gHex : '0' + gHex;
    bHex = bHex.length > 1 ? bHex : '0' + bHex;

    return (rHex + gHex + bHex).toUpperCase();
}

function parseIntToHexString(value) {
    return ("000000" + value.toString(16)).slice(-6);
}

function getSwitchState(id) {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        var commandString = 'GETSWITCHSTATE:' + id;
        $.post("https://api.particle.io/v1/devices/" + deviceID + "/Function", {
            access_token: accessToken, args: commandString
        }).success(function (data) {
            console.log('success! getSwitchState(' + id + '): ' + data.return_value);
            if(data.return_value >= 0) {
                var checked = data.return_value === 1 ? true : false;
                $("input#switch" + id).prop('checked', checked);
            }
            else
                console.log('fail getSwitchState(' + id + '): ' + data.return_value);
        }).fail(function (data) {
            console.log('fail getSwitchState(' + id + '): ' + data.return_value);
            if (supportsLocalStorage())
                $("input#switch" + id).prop('checked', localStorage["spark_pixels.switch" + id] === 'true' ? true : false);
        });
        //console.log('commandString: ' + commandString);
    }
}

function getColor(id) {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        var commandString = 'GETCOLOR:' + id;
        $.post("https://api.particle.io/v1/devices/" + deviceID + "/Function", {
            access_token: accessToken, args: commandString
        }).success(function (data) {
            if(data.return_value >= 0) {
                var color = '#' + parseIntToHexString(data.return_value);
                $("input#color" + id).spectrum('set', color);
                console.log('success! getColor(' + id + '): ' + color + '/' + data.return_value);
            }
        }).fail(function (data) {
            console.log('fail getColor(' + id + '): ' + data.return_value);
            if (supportsLocalStorage())
                $("input#color" + id).spectrum("set", "#" + localStorage["spark_pixels.color" + id]);
        });
    }
}

function getBrightness() {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        // Retrieve the device's brightness variable from the cloud API
        $.get("https://api.particle.io/v1/devices/" + deviceID + "/brightness/?access_token=" + accessToken)
            .success(function(data) {
                brightness = (parseInt(data.result) / 255) * 100;
                console.log("success! getBrightness(): " + brightness.toFixed(0) + "/" + data.result);
            }).fail(function(data) {
                console.log('fail getBrightness(): ' + data.return_value);
                if (supportsLocalStorage()) {
                    if (localStorage["spark_pixels.brightness"])
                        brightness = parseInt(localStorage["spark_pixels.brightness"]);
                }
            }).always(function() {
                $("#brightnessSlider").val(brightness.toFixed(0));
                updateBrightnessLabel(brightness.toFixed(0));
                $("div#brightnessControl").slideDown(300);
            });
    }
}

function getSpeed() {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        // Retrieve the device's brightness variable from the cloud API
        $.get("https://api.particle.io/v1/devices/" + deviceID + "/speed/?access_token=" + accessToken)
            .success(function(data) {
                console.log("success! getSpeed(): " + data.result);
                speed = parseInt(data.result);
                $("#speedSlider").val(speed);
                updateSpeedLabel(speed);
            }).fail(function(data) {
                console.log('fail getSpeed(): ' + data.return_value);
                if (supportsLocalStorage()) {
                    if (localStorage["spark_pixels.speed"])
                        speed = parseInt(localStorage["spark_pixels.speed"]);
                }
            }).always(function() {
                $("#speedSlider").val(speed);
                updateBrightnessLabel(speed);
                $("div#speedControl").slideDown(300);
            });
    }
}

function setColors() {
    var colorsString = '';
    for(var count = 1; count <= 6; count++) {
        if($("span#color" + count).css('display').indexOf('none') < 0) {
            var color = $("span#color" + count).find("div.sp-preview-inner").css("background-color");
            var rgbString = color.split("(")[1].split(")")[0].split(",");
            colorsString += 'C' + count + ':' + parseRGBToHexString(rgbString[0], rgbString[1], rgbString[2]) + ',';
            if (supportsLocalStorage()) {
                localStorage["spark_pixels.color" + count] = parseRGBToHexString(rgbString[0], rgbString[1], rgbString[2]);
            }
        }
    }
    if(colorsString.length > 0) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetMode", {access_token: accessToken, args: colorsString})
                .done(function(data) {console.log('done setColors(): ' + data.return_value);})
                .fail(function (data) {console.log('fail setColors(): ' + data.return_value);});
        }
    }
}

function setSwitches() {
    var switchesString = '';
    for(var count = 1; count <= 4; count++) {
        if($("span#switch" + count).css('display').indexOf('none') < 0) {
            var checked = $("input#switch" + count).prop('checked');
            switchesString += 'T' + count + ':' + (checked ? '1' : '0') + ',';
            if (supportsLocalStorage()) {
                localStorage["spark_pixels.switch" + count] = checked.toString();
            }
        }
    }
    if(switchesString.length > 0) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetMode", {access_token: accessToken, args: switchesString})
                .done(function(data) {console.log('done setSwitches(): ' + data.return_value);})
                .fail(function (data) {console.log('fail setSwitches(): ' + data.return_value);});
        }
    }
}

function setText() {
    if ($("input#text").val().trim().length > 0) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetText", {access_token: accessToken, args: $("input#text").val().trim()})
                .done(function(data) {console.log('done setText(): ' + data.return_value);})
                .fail(function (data) {console.log('fail setText(): ' + data.return_value);});
        }
    }
}

function setMode() {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            var commandString = 'M:' + currentMode + ',';

            //$("span#mode").text(currentMode);
            if(currentMode.toLowerCase().indexOf('text') >= 0)
                setText();

            setColors();
            setSwitches();

            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetMode", {access_token: accessToken, args: commandString})
                .success(function(data) {console.log('success! setMode(): ' + data.return_value);})
                .fail(function (data) {console.log('fail setMode(): ' + data.return_value);});
        }
    }
}

function setBrightness() {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            var commandString = 'B:' + (brightness > 0 ? brightness : 1) + ',';
            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetMode", {
                access_token: accessToken, args: commandString
            }).success(function(data) {
                updateBrightnessLabel(brightness);
            }).fail(function(data) {
                console.log('fail setBrightness(): ' + data.return_value);
            });
        }
    }
}

function setSpeed() {
    if (typeof accessToken !== 'undefined' && accessToken !== null) {
        var deviceID = getDeviceID();
        if(deviceID !== '') {
            var commandString = 'S:' + speed + ',';
            $.post("https://api.particle.io/v1/devices/" + deviceID + "/SetMode", {
                access_token: accessToken, args: commandString
            }).success(function(data) {
                updateSpeedLabel(speed);
            }).fail(function(data) {
                console.log('fail setSpeed(): ' + data.return_value);
            });
        }
    }
}