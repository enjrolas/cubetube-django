/**
 * Viz Cards
 */

$('.viz-card').click(function(e) {
    e.preventDefault();

    var $this = $(e.currentTarget);
    openViz( $this.attr('href') );

});

var frameTemplate = '<iframe class="viz-template"></iframe>';
var $body = $(document.body);
var $glass = $('.glass');

$glass.click(function() {
    closeViz();
})

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
        $glass.hide();
        $viz.remove();
        $body.removeClass('no-scroll');
    }
}