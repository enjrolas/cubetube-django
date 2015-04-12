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
    
    formatCode();

    // Handle different viz types.
    var vizType = getVizType();
    if( vizType === 'L3D') {

        parseSparkCode( getVizUrl() );
        runSparkSketch();

    } else if ( vizType === 'javascript' ) {
        runSketch();
    }
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