/**
 * Viz Cards
 */

var $cards = $('.viz-card');
var frameTemplate = '<iframe class="viz-template"></iframe>';
var $body = $(document.body);
var $glass = $('.glass');

function bindGlass() {
    $glass.click(function() {
        closeViz();
    })
}

function bindListener() {
    window.addEventListener("message", receiveMessage, false);
}

function receiveMessage(event) {
        
    console.log(event);

    // Open code area even more!
    if( event.data === "open" ) {
        $('.viz-template').addClass('more-open');
    } else if (event.data === "close") {
        $('.viz-template').removeClass('more-open');
    }
}

function bindCards() {
    $('.viz-card').click(function(e) {
        e.preventDefault();

        var $this = $(e.currentTarget);
        openViz( $this.attr('href') );
    });
}

function openViz( url ) {

    var frame = $( frameTemplate );
    frame.attr('src', url );

    $body.append( frame );
    $glass.show();
    $body.addClass('no-scroll');
};

function closeViz() {

    var $viz = $( '.viz-template' );
    if( $viz.length ) {
        $viz.removeClass('more-open');
        $glass.hide();
        $viz.remove();
        $body.removeClass('no-scroll');
    }
}

if( $cards.length ) {
    bindCards();
    bindListener();
    bindGlass();    
}
