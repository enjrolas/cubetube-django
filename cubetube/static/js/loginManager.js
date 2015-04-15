/**
 * Login Management. 
 * - Built off Alex's spark login system.
 */

$(function(){

    var $popover = $( '.login-signup-popover' );
    var $glass   = $( '.glass' );
    if( $popover.length ) {

        // Show login signup
        $('.login').click(function(e) {
            e.preventDefault();
            $popover.show();
            $glass.show();
        });
        
        // Sign up forms exist, change type.
        $('.login-title .link').click(function() {
            if( $popover.hasClass('login') ) {
                $popover.removeClass('login').addClass('join');
            } else {
                $popover.removeClass('join').addClass('login');
            }
        });

        // Close the glass layer.
        $('.glass').click(function() {
            $popover.removeClass('join').addClass('login');
            $popover.hide();
            $glass.hide();
        });

        /**
         * Sign up... Registers a new user.
         */
        $('.signup-component .signup-button').click(function() {
            var email = $('.signup-component .email').val();
            var nickname = $('.signup-component .nickname').val();
            var password = $('.signup-component .password').val();
            createNewUser(email, nickname, password);
        });

        /**
         * Sign in
         */
        $('.login-component .login').click(function() {
            var email = $('.login-component .email').val();
            var password = $('.login-component .password').val();
            sparkLogin(email, password);
        });
    }

    /**
     * Log out
     */
    $('.logout').click(function(e) {
        e.preventDefault();
        $.removeCookie("accessToken", { path: '/' });
        $.removeCookie("username", { path: '/' });
        $.removeCookie("nickname", { path: '/' });
        $.removeCookie("coreID", { path: '/' });
        location.reload();
    })
});

function createNewUser(email, nickname, password) {
    
    // Register with spark.
    window.spark.createUser(email, password, function(err, data) {
    
        // We try to login and get back an accessToken to verify user creation
        if (!err) {
            sparkSignup(email, nickname, password);
        } else {
            if(err.message.indexOf("already exists")>-1) {
                sparkSignup(email, nickname, password);
            } else {
                displayCreateUserError(err.message);
            }
        }
    });
}

// Signs up to spark, and then logs you in.
function sparkSignup(email, nickname, password) {
    var loginPromise = window.spark.login({ username: email, password: password });
    loginPromise.then(function(data){
        // They have logged in with spark.

        var accessToken = data.access_token;
        $.cookie("accessToken", data.access_token, { expires: data.expires_in/86400 , path: '/'});

        //let the server know that we're logged in
        $.post("/login/", {
            email: email,
            accessToken: data.access_token,
            dataType: 'script',
        }).success(function(data) {
            
            // Logged in.
            if( data['status'] == "ok" ) {
                nickname=data['nickname'];
                $.cookie("nickname", nickname, { expires: data.expires_in/86400 , path: '/'});
                location.reload();

            // Create new user
            } else if ( data['status'] == "newUser" ) {
                cubetubeSignup(email, accessToken, nickname)
            }
        }).error(function(data) {});

    }, function(error){
        displayError(error);
    });
}

// Logs into spark, or errors.
function sparkLogin(email, password) {
    var loginPromise = window.spark.login({ username: email, password: password });
    loginPromise.then(function(data){
        // They have logged in with spark.

        var accessToken = data.access_token;
        $.cookie("accessToken", data.access_token, { expires: data.expires_in/86400 , path: '/'});

        //let the server know that we're logged in
        $.post("/login/", {
            email: email,
            accessToken: data.access_token,
            dataType: 'script',
        }).success(function(data) {
            
            // Logged in.
            if( data['status'] == "ok" ) {
                nickname=data['nickname'];
                $.cookie("nickname", nickname, { expires: data.expires_in/86400 , path: '/'});
                location.reload();
            } else {
                displayError('Sorry, no such user exists');
            }
        }).error(function(data) {
            displayError('Sorry, no such user exists');
        });

    }, function(error){
        displayError(error);
    });
}

function cubetubeSignup(email, accessToken, nickname) {
    $.ajax({
        type: 'post',
        url: '/newUser/',
        data: {
            email: email,
            accessToken: accessToken,
            nickname: nickname
        },
        success: function( data ) {
            nickname=data['nickname'];
            $.cookie("nickname", nickname, { expires: data.expires_in/86400 , path: '/'});
            location.reload();
        },
        error: function( errorMessage ) {
            console.log( errorMessage );
        }
    })
}

function displayError(error) {
    console.log( "Login error: ", error );
}

// var accessToken, username, nickname, coreID;  //global vars holding the user's accesstoken and email

// //upon page load
// $(function(){
//         //check to see if we have a set cookie, to see if we're logged in
// 	checkCookie();

//         //register functions for the three buttons on the page
// 	$("#login-form-button").click(function(e){ 
// 	    e.preventDefault();  //prevent the page from reloading
// 	    username=$("#login-form-email").val();
// 	    login( username, $("#login-form-password").val());
// 	    });

	// $("#create-user-button").click( function(e){
	//     e.preventDefault();  //prevent the page from reloading
	//     username=$("#create-user-email").val();
	//     createUser(username, $("#create-user-password").val());
	// 	    });

// 	$("#logout").click(function(e) {
// 		e.preventDefault();  //prevent the page from reloading
// 		logout();
// 	    });
 
//     $('#nickname').bind('input',function(){ 
// 	validateNickname();
//     });

//     $("#set-nickname-button").click(function(e) {
// 	e.preventDefault();  //prevent the page from reloading
// 	setNickname();
//     });

//     $("#cubeName").change(function(){
// 	coreID=$(this).find(":selected").val();
// 	var date=new Date();
// 	$.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});	
//     });

//     jQuery.fn.extend({
//         disable: function(state) {
//             return this.each(function() {
//                 this.disabled = state;
//             });
//         }
//     });
//    });


// //login attempts to log into spark's server with the email/password combination
// //retrieves the account's access token, and sets site-wide cookies with the user's
// //email and access token.

// function listCubes()
// {
//     var devicesPr = spark.listDevices();
//     var deviceInList=false;
//     var deviceID;
//     var connectedCores=0;
//     $("#cubeName").empty();   //clear the dropdown list
//     devicesPr.then(
// 	function(devices){
// 	    console.log('Devices: ', devices);
// 	    for(var i=0;i<devices.length;i++)
// 	    {
// 		var device=devices[i];
// 		if(device.connected){
// 		    connectedCores++;
// 		    if(deviceID != 'undefined')
// 		    {
// 			deviceID=device.id;
// 			console.log(deviceID);
// 		    }
// 		    console.log(device.name+" is connected");
// 		    $("#cubeName").append($("<option></option>").val(device.id).html(device.name));  //append the cube name and ID to thr dropdown list
// 		    if(device.name==coreID)
// 			deviceInList=true;
// 		}
// 	    }
// 	    if(deviceInList==true)
// 		$('#cubeName').val(coreID);	    
// 	    else
// 	    {
// 		$('#cubeName').val(deviceID);	    
// 		coreID=deviceID;
// 		var date=new Date();
// 		$.cookie("coreID", coreID, { expires: date.getTime()+86400 , path: '/'});	
// 	    }
// 	    if(devices.length==0)
// 		$("#cubeName").append($("<option></option>").html('Add a core to your spark account to get started'));  //append the cube name and ID to thr dropdown list
// 	    else if(connectedCores==0)
// 		$("#cubeName").append($("<option></option>").html('None of your cores are online right now'));  //append the cube name and ID to thr dropdown list
// 	},
// 	function(err) {
// 	    console.log('List devices call failed: ', err);
// 	}
//     );
// }


// function loginWithAccessToken()
// {
//     var loginPromise = window.spark.login({ accessToken: accessToken });
//     loginPromise.then(
//       function(data) {
// 	  console.log("logged in");
//       });
// }

// //looks to see
// function checkCookie()
// {
//     accessToken=$.cookie("accessToken");
//     username=$.cookie("username");
//     nickname=$.cookie("nickname");
//     coreID=$.cookie("coreID");
//     if(!(accessToken === undefined || accessToken === null))
//     {
// 	loginWithAccessToken();
// 	setLoggedIn();
//     }
//     else
// 	setLoggedOut();
// }