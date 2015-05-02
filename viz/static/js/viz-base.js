/**
 *  Viz base.
 *  - Code specific to the viz tab. Saving, editing, forking etc.
 */

/**
 * Vars
 */

var library;

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
                // runSparkSketch();
                $( '.CodeMirror' ).height(window.innerHeight - 120)
            }, 1);
        } );

    } else if ( vizType === 'javascript' ) {
        runSketch();
    }

    // View code
    $('.view-code').click(function() {

        var state = $( '.viz-wrapper' ).hasClass('code-active');
        $( '.viz-wrapper' ).toggleClass('code-active', !state);

        if( state == true ) {
            $('.view-code').text('View Code')
        } else {
            $('.view-code').text('Hide Code')
        }
    });

    // Send to cube.
    $('.sent-to-cube').click(function() {

    });

    // Select Cube.
});



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

function initializeSparkSketch() {
}

function fork() {
}

function edit() {
}

// Load JAVASCRIPT code
function loadCode() {
}

// Applies codeMirror formatting code.
function formatCode() {
    var editor = CodeMirror.fromTextArea(document.getElementById("code"), {
        lineNumbers: true,
        styleActiveLine: true,
        matchBrackets: true
    });
    editor.setOption("theme", "tomorrow-night-eighties");
}