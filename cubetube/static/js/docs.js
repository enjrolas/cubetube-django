var $sidebar = $('.docs-nav');
var $docs    = $('.docs-main')
var turningPoint = $sidebar.offset().top - 55; // added header height.
var endingPoint = $docs.offset().top + $docs.height() - $sidebar.height();// + ($sidebar.height() - window.innerHeight);

console.log( endingPoint );

if( $sidebar.length ) {

    $sidebar.height

    var $window = $(window);
    $window.scroll(function() {
        var scrollPos = $window.scrollTop();
        if( scrollPos > endingPoint ) {
            $sidebar.removeClass('fixed').addClass('absolute');
        } else if ( scrollPos > turningPoint ) {
            $sidebar.addClass('fixed').removeClass('absolute');
        } else {
            $sidebar.removeClass('fixed').removeClass('absolute');
        }
    })

    // Set height of sidebar & handle resizes
}