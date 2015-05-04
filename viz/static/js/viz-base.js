/**
 *  Viz base.
 *  - Code specific to the viz tab. Saving, editing, forking etc.
 */

/**
 * Vars
 */

var library;
var editor;

/**
 * App
 */

$(function(){
    $.get('/static/js/l3d-library.js', function(data) {
        library=data;
    }, 'text');
    
    // Init!
    var vizType = getVizType();
    if( vizType === 'L3D') {

        parseSparkCode( getVizUrl(), function() {
            setTimeout( function() {
                formatCode();
                runSparkSketch();
                setHeight();
            }, 1);
        } );

    } else if ( vizType === 'javascript' ) {
        runSketch();
    }

    // View code
    $('.view-code').click(function() {

        var state = $( '.viz-wrapper' ).hasClass('code-active');

        if( state == true ) {
            $('.view-code').text('View Code')
            $( '.viz-wrapper' ).toggleClass('code-active', !state);
            parent.postMessage( "close", "*" );
            $body.removeClass('no-scroll');
        } else {
            
            $('body').animate( 
                {scrollTop:'0px'},
                300,
                'swing',
                function(){ 
                    parent.postMessage( "open", "*" );
                    $( '.viz-wrapper' ).toggleClass('code-active', !state);
                    $('.view-code').text('Hide Code');
                    $body.addClass('no-scroll');
                } 
            )
        }
    });

    // Send to cube.
    $('.sent-to-cube').click(function() {
    });

    $('.run-sketch').click(function() {
        //editor.save();
        translateCode(document.getElementById("code").value);
        runSparkSketch();
    });

    $('.save-sketch').click(function() {

    });

    $('.fork-sketch').click(function() {

    });

    $('.twitter').click(function() {
        openTwitterPopup();
    })

    $('.facebook').click(function() {
        openFBPopup();
    })
});

function setHeight() {
    $( '.CodeMirror' ).height(window.innerHeight - 180)
}

window.onresize = setHeight;

/**
 * Functions
 */
function getVizType() {
    var $wrapper = $('.viz-wrapper');
    return $wrapper.attr('data-viz-type');
}

function getVizUrl() {
    var $wrapper = $('.viz-wrapper');
    return $wrapper.attr('data-viz-url');
}

function formatCode() {

    // Applies codeMirror formatting code.
    editor = CodeMirror.fromTextArea(document.getElementById("code"), {
        lineNumbers: true,
        styleActiveLine: true,
        matchBrackets: true
    });
    editor.setOption("theme", "tomorrow-night-eighties");
    editor.on("change", function(cm, change) {
        document.getElementById("code").value = cm.getValue();
    })
}

function openTwitterPopup() {
   var width  = 575,
       height = 250,
       left   = (screen.width / 2)  - (width  / 2),
       top    = (screen.height / 2) - (height / 2),
       url    = 'http://twitter.com/share',
       opts   = 'status=1' +
                ',width='  + width  +
                ',height=' + height +
                ',top='    + top    +
                ',left='   + left;
   
   window.open(url, 'twitter', opts);

   return false;
}

function openFBPopup() {

    var url = "http://url.com";
    var title = "title";
    var descr = "desc";

    var winWidth = $(window).width();
    var winHeight = $(window).height();
    var winTop = (screen.height / 2) - (winHeight / 2);
    var winLeft = (screen.width / 2) - (winWidth / 2);

    window.open('http://www.facebook.com/sharer.php?s=100&p[title]=' + title + '&p[summary]=' + descr + '&p[url]=' + url, 'sharer', 'top=' + winTop + ',left=' + winLeft + ',toolbar=0,status=0,width=' + winWidth + ',height=' + winHeight);
}
