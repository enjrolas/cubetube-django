/**
 * Login Management. 
 * - Built off Alex's spark login system.
 */
//var checkbox;

$(document).ready(function(){
	var menuTimer;
	
	$('ul.items').on('mouseover', function() {
		clearTimeout(menuTimer);
		showMenuItems();
		return false;	//required to dodge the firing of the menu action on mobile devices
	}).on('mouseleave', function() {
		menuTimer = setTimeout(function() { hideMenuItems(); }, 800);
		return false;	//required to dodge the firing of the menu action on mobile devices
	});
	
	$('#search-button').on('click', function() {
		justSearch($('#search-box').val());
	});
	$('#search-box').on('keydown', function(e) {
		if (e.keyCode === 13) justSearch($(this).val());	//Enter
	});
	
	$('.on-forum').hide();
	$('.on-gallery').hide();
	$('.on-docs').hide();
	$('.on-overview').hide();
	$('.on-create').hide();

	// Positioning the search box if we are at the Gallery template
	if(window.location.pathname.indexOf('gallery') >= 0)
		$("#search-box-container").show();
	else
		$("#search-box-container").hide();
	//$('#search-button').css('left', ($('#search-box-container').css('left') + $('#search-box-container').css('width')) /*- $('#search-button').css('width')*/);
	
/*
    checkbox = $("#ToS");   // keep reference of checkbox

    $("#ToS").click(function() {
        var checked = checkbox.prop("checked");      // check checkbox status
	console.log(checked);
        checkbox.prop("checked", checked);
    });
*/

    var $popover = $( '.login-signup-popover' );
    var $glass   = $( '.glass' );
    if( $popover.length ) {
        // Sign up forms exist, change type.
        $('.login-title .link').click(function() {
            if( $popover.hasClass('login') ) {
                $popover.removeClass('login').addClass('join');
            } else {
                $popover.removeClass('join').addClass('login');
            }
            clearError();
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
//	    var ToS=checkbox.prop("checked");
//	    console.log(ToS);
            clearError();
            createNewUser(email, nickname, password);
        });

        /**
         * Sign in
         */
        $('.login-component .login').click(function() {
            var email = $('.login-component .email').val();
            var password = $('.login-component .password').val();
            clearError();
            sparkLogin(email, password);
        });
    }
    
    // Show login signup
    $('#login-button').click(function(e) {
		console.log('.login > clicks = ' + clicks);
		var menuVisible = $('li.on-overview').css('display');
		if(menuVisible === 'none' || !clicks) {
			showMenuItems();
			menuTimer = setTimeout(function() { hideMenuItems(); clearTimeout(menuTimer); }, 5000);
			return false;
		} else {
			//hideMenuItems();
			e.preventDefault();
			if(clicks) {
				$popover.show();
				$glass.show();
			}
		}
    });

    /**
     * Log out
     */
    $('.logout').click(function(e) {
		var menuVisible = $('li.on-overview').css('display');
		//This is for showing the menu on mobile devices (no mouseOver support)
		if(menuVisible === 'none' || !clicks) {
			showMenuItems();
			//We wait a longer time before hiding the menu
			menuTimer = setTimeout(function() { hideMenuItems(); clearTimeout(menuTimer); }, 5000);
			return false;
		} else {
			//hideMenuItems();
			e.preventDefault();
			if(clicks) {
				$.removeCookie("accessToken", { path: '/' });
				$.removeCookie("username", { path: '/' });
				$.removeCookie("nickname", { path: '/' });
				$.removeCookie("coreID", { path: '/' });
				location.reload();
			}
		}
    })
});

//window.onresize = $('#search-button').css('left', ($('#search-box-container').css('left') + $('#search-box-container').css('width')) /*- $('#search-button').css('width')*/);

function hideMenuItems() {
	clicks = 0;
	$($("ul.items > li").get().reverse()).each(function(index) {
		if(!$(this).hasClass("sole-button"))
			$(this).delay(200*index).addClass('on-overview').slideUp( 800, "linear", function() { $(this).delay(150*index).removeClass('on-overview'); });
	});
}

function showMenuItems() {
	if(!clicks) {
		clicks = 1;
		$('ul.items > li').each(function(index) {
			if(!$(this).hasClass("sole-button")) {
				$(this).delay(200*index).addClass('on-overview');
				if($(this).next().length)
					$(this).slideDown( 800, "linear", function() { $(this).delay(150*index).removeClass('on-overview'); });
				else $(this).slideDown( 800, "swing");
			}
		});
	}
}

function justSearch(searchTerm) {
	clearTimeout();
	$("#search-button").html("Wait<span class=\"one\">.</span><span class=\"two\">.</span><span class=\"three\">.</span>");
    $('#search-button').prop( "disabled", true );

	var url="/search/1/" + searchTerm + "/8/" 
    //console.log(url);
	$.ajax({
		type: 'get',
		url: url,
		success: function(data) {
			// Here we replace the entire viz gallery code with the code returned by the view:
			$("#viz-in-gallery").replaceWith( data );
			$("#search-button").html("Search!");
			$('#search-button').prop( "disabled", false );
			//setTimeout(function() { activatePreviews(); }, 1);
		},
		error: function(data) {
			$("#search-button").html("Search!");
			$('#search-button').prop( "disabled", false );
			console.log('Error retrieving search!')
			console.log(data)
		}
	})
}

function createNewUser(email, nickname, password) {
    // Register with spark.
    window.spark.createUser(email, password, function(err, data) {
        // We try to login and get back an accessToken to verify user creation
        if (!err) {
        	if(nickname.trim().length > 0)
        		sparkSignup(email, nickname, password);
        	else 
        		displayError(err, 'Whoops - Please enter a nickname.');
        } else {
            if(err.message.indexOf("already exists")>-1) {
                sparkSignup(email, nickname, password);
            } else {
            	var message;
            	if (err === undefined || err === null) {
            		message = 'Could not signup, please try again.';
            	} else {
        	    	// Invalid username or password
        	        if(err.message.toLowerCase() === 'invalid_grant') {
        	        	message = 'Sorry, invalid username or password.';
        	        } else if (err.message.toLowerCase() === 'username must be an email address.') {
        	        	message = 'Urp, username must be an email address!';
        	        } else if(err.message.toLowerCase() === "invalid_client")
        	    		message = 'Please enter both username and password.';
                }
            	displayError(err, message);
            	//console.log(err.message);
                //displayError(err);
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
        
        }).error(function(data) {
        	//console.log(data);
        });

    }, function(error){
    	var message;
    	if (error === undefined || error === null) {
    		message = 'Could not validate login, please try again.';
    	} else {
	    	// Invalid username or password
	        if(error.message.toLowerCase() === 'invalid_grant') {
	        	message = 'Sorry, invalid username or password.';
	        } else if (error.message.toLowerCase() === 'username must be an email address.') {
	        	message = 'Urp, username must be an email address!';
	        } else if(error.message.toLowerCase() === "invalid_client")
	    		message = 'Please enter both username and password.';
        }
    	displayError(error, message);
    	
    	//console.log(error.message);
        //displayError(error);
    });
}

//looks to see                
function checkCookie()
{
    accessToken=$.cookie("accessToken");
    username=$.cookie("username");
    nickname=$.cookie("nickname");
    coreID=$.cookie("coreID");
    if(!(accessToken === undefined || accessToken === null))
	{
	    loginWithAccessToken();
	}
    //    else
    //    setLoggedOut();
}

function loginWithAccessToken()
{
    var loginPromise = window.spark.login({ accessToken: accessToken });
    loginPromise.then(
		      function(data) {
			  console.log("logged in");
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
                cubetubeSignup(email, accessToken, email.split('@')[0]);
            }
        
        }).error(function(data) {
            cubetubeSignup(email, accessToken, email.split('@')[0]);
        });
    
    }, function(error){
    	var message;
    	if (error === undefined || error === null) {
    		message = 'Could not validate login, please try again.';
    	} else {
	    	// Invalid username or password
	        if(error.message.toLowerCase() === 'invalid_grant') {
	        	message = 'Sorry, invalid username or password.';
	        } else if (error.message.toLowerCase() === 'username must be an email address.') {
	        	message = 'Urp, username must be an email address!';
	        } else if(error.message.toLowerCase() === "invalid_client")
	    		message = 'Please enter both username and password.';
        }
    	displayError(error, message);
    	
    	//console.log(error.message);
        //displayError(error, "Sorry, already used username or email!");
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
            //console.log( errorMessage );
        }
    })
}

function displayError(error, message) {
    if( message ) {
        $('.error-area').html(message);
        return;
    }
}

function clearError() {
    $('.error-area').html('')
}