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
    }

});

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

// 	$("#create-user-button").click( function(e){
// 	    e.preventDefault();  //prevent the page from reloading
// 	    username=$("#create-user-email").val();
// 	    createUser(username, $("#create-user-password").val());
// 		    });

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


// function validateNickname()
// {
//     var nick=encodeURIComponent($("#nickname").val());  //URL-encode the data
//     var url="http://127.0.0.1:8000/validateNickname/"+nick+"/";
//     console.log(url);
//     $.get( url, function( data ) {	
// 	console.log(data);
// 	if(data['status']=='ok')
// 	{
// 	    $("#nickname-error").html("&#x2713;");
// 	    $("#nickname-error").attr("class","valid-nickname");
// 	    $("#set-nickname-button").prop("disabled",false);
// 	}
// 	else
// 	{
// 	    $("#nickname-error").text(data['error']);
// 	    $("#nickname-error").attr("class","invalid-nickname");
// 	    $("#set-nickname-button").prop("disabled",true);
// 	}
//     });
// }

// function setNickname()
// {
//     $.post("http://127.0.0.1:8000/setNickname/",
// 	   {
// 	       email: username,
// 	       accessToken: accessToken,
// 	       nickname: $("#nickname").val(),
// 	       dataType: 'script',
// 	   }).success(function(data)
// 		      {
// 			  setLoginData(data);
// 			  setLoggedIn();
			  
// 		      }).error(function(data){
// 			  console.log(data)});
	
// }

// //login attempts to log into spark's server with the email/password combination
// //retrieves the account's access token, and sets site-wide cookies with the user's
// //email and access token.
// function login( email, password)
// {
//     var loginPromise = window.spark.login({ username: email, password: password });
//     loginPromise.then(function(data){
// 	handleLoginResponse(data);
// 		     },
// 		      function(error){
// 			  handleLoginError(error);
// 		      });
// }

// function handleLoginResponse(data){

// 	  accessToken=data.access_token;
// 	  $.cookie("accessToken", data.access_token, { expires: data.expires_in/86400 , path: '/'});
// 	  console.log(accessToken);

// 	  //let the server know that we're logged in
// 	  $.post("/login/",
// 		 {
// 		     email: username,
// 		     accessToken: data.access_token,
// 		     dataType: 'script',
// 		 }).success(function(data)
// 		 {
// 		     if(data['status']=="ok")
// 		     {
// 			 setLoginData(data);
// 		     }
// 		     else if(data['status']=="newUser")
// 		     {			 
// 			 nicknameDialog.dialog("open");
// 			 //$('#nicknameForm').show();
// 			 $("#set-nickname-button").prop("disabled",true);
// 		     }
// 		     else{
// 			 setLoginData(data);
// //			 logout();
// 		     }
		     
// 		 }).error(function(data)
// 			  {
// 			      console.log("error");

// 			  });
// }

// function handleLoginError(error) {
//         if (error.message === 'invalid_client') {
//           displayLoginError('Invalid username or password.');
//         } else if (error.cors === 'rejected') {
//           displayLoginError('Request rejected.');
//         } else {
//           displayLoginError('Unknown error.');
//           console.log(error);
//         }
// }

// function setLoginData(data){
// 			 nickname=data['nickname'];
// 			 $.cookie("nickname", nickname, { expires: data.expires_in/86400 , path: '/'});
// 			 $('#login-form-email').val('');
// 			 $('#login-form-password').val('');
// 			 displayLoginError('');
// 			 //displayLoginResult(JSON.stringify(data));
// 			 setLoggedIn();
// }

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


// //this function attempts to create a user at the given email/pass combination
// //and if there's no glaring error, logs in with the email/pass combination
// function createUser(email, password)
// {
//     window.spark.createUser(email, password, function(err, data) {
// 	    console.log('err on create user:', err);
// 	    console.log('data on create user:', data);

//   if (!err) {
// //      displayCreateUserResult(JSON.stringify(data));
//       // We try to login and get back an accessToken to verify user creation
//       console.log("trying to log in...");
//       login(email, password);
//   }
//   else
// 	{
// 	    if(err.message.indexOf("already exists")>-1)
// 		login(email, password);
// 	    else
// 		displayCreateUserError(err.message);	    
// 	}	
//     });

// }

// //removes both cookies
// function logout()
// {
//     $.removeCookie("accessToken", { path: '/' });
//     $.removeCookie("username", { path: '/' });
//     $.removeCookie("nickname", { path: '/' });
//     $.removeCookie("coreID", { path: '/' });
//     accessToken="";
//     username="";
//     nickname="";
//     coreID="";
//     setLoggedOut();
// }

// function setLoggedIn()
// {
//     updateStatus("logged in as "+nickname);
//     $("#logout").show();
//     $("#logged-out").hide();
//     updateAction("log out");
//     listCubes();
//     dialog.dialog( "close" );
//     createUserDialog.dialog( "close" );
//     nicknameDialog.dialog("close");
//     $("#cube-selector").show();
//     $("#login").hide();
//     $("#create-user").hide();
//     $("#nicknameForm").hide();
//     $(".requireLogin").show();
// }

// function setLoggedOut()
// {
//     updateStatus("you are logged out");
//     $("#logged-out").show();
//     $("#logout").hide();
//     $("#cube-selector").hide();
//     $("#login").hide();
//     $("#create-user").hide();
//     $("#nicknameForm").hide();
//     $(".requireLogin").hide();
// }

// function showLogin()
// {
//     $("#login").show();
//     $("#create-user").hide();
// }

// function showCreate()
// {
//     $("#login").hide();
//     $("#create-user").show();
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


// //these functions just streamline updating the content of different divs on the page

// function updateStatus(message) {
//     $('#status').text(message);
// }

// function updateAction(message) {
//     $('#action').text(message);
// }

// function displayLoginError(message) {
//     $('#login-error').text(message);
// }

// function displayLoginResult(message) {
//     $('#login-result').text(message);
// }

// function displayCreateUserError(message) {
//     $('#create-user-error').text(message);
// }

// function displayCreateUserResult(message) {
//     $('#create-user-result').text(message);
// }

