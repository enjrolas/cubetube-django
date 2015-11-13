
var library;
$.get('/static/js/l3d-library.js', function(data) {
    library=data;
}, 'text');

var editing = false;

$(function(){

    formatCode();
    setHeight();

    if( $('.viz-wrapper').attr('viz-id') ) {
        editing = true;
    }

    // Switch states
    $('.switch-part').click(function() {

        var $this = $( this );
        var $box = $this.closest('.switch-box');

        $on =  $box.find('.on');
        $off = $box.find('.off');

        $on.removeClass('on').addClass('off');
        $off.removeClass('off').addClass('on');

        if( $box.hasClass('interactive') ) {
            if( $on.html() === 'Yes') {
            	$('.video-url').animate({ left: '-100px' }, 100, function() {
                    $('.video-url').hide("fast");
            	});
            } else {
	        	$('.video-url').animate({ left: '+0px' }, 100, 'linear', function() {
	                $('.video-url').show("slow");
	        	});
            }
        }
    })

    $('.run-sketch').click(function() {

        var vizType = getEditVizType();
        
        if( vizType === 'L3D') {
            translateCode(document.getElementById("code").value);
            runSparkSketch();
        } else if ( vizType === 'JS' ) {
            runSketch();
        }
    })

/*$('.save-code').click********************************************/

});	//$(function()

function checkProgramValidity(data){
    var vizType = data['viz-type'];
    var code=data['sourceCode'];
    if( vizType === 'L3D') {
        compileCode();
    } 
    else if ( vizType === 'JS' ) {
	    try{
	    	runSketch();
	    }
	    catch(err){
			console.log(err);
			makePrivate();
	    }
    }
}

function compileCode(){
    var request=$.ajax({
	type:"POST",
	url:"/justCompile/",
	data: {"code": $(".code").val(), "accessToken":accessToken, "coreID":$("#cubeName").val()},
	dataType:"text",
	success: function(data){
	    data = $.parseJSON(data);
	    console.log(data);
	    output="";
	    if(data.compilation_status==="ok")
	    {
		output+="compilation status:  ok\n";
		output+="flashing status: "+data.flash_output;
	    }
	    else
	    {
		output+="compilation status: "+data.compilation_status+"\nerrors:\n"+data.error+"\n";
		makePrivate();
	    }
	    $("textarea.output").val(output);
	},
	error: function(data){
	    console.log("fail");
	    console.log(data);
	}
    });
}

function makePrivate(){
    console.log("viz doesn't run -- making it private");
}

/*checkDataAfterSave*checkData*onCreateSuccess*********************/

function validate(data) {

    if( data.name === '') {
        showConsoleError("Oh no, you need a viz name!");
        return false;
    }

    if( data.description === '') {
        showConsoleError("Aww, snap, we need a description to save this!");
        return false;
    }


    if( data.sourceCode === '') {
        showConsoleError("Hey, we need some code to save this!");
        return false;
    }

    if( data.interactive === true && ( data.videoURL.indexOf("youtube") == -1) ) {
        showConsoleError("Oh no, this interactive viz needs a youtube url to save!");
        return;
    }

    return true;
}

function showConsoleError( error ) {
    $('.console').val( error );
}

function getEditVizType() {

    var type;
    var $onElement = $('.viz-type .on');
    var onVal = $onElement.html();
    return onVal;
}

/*getData***********************************************************/