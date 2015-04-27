/**
 * Scrollmonitor
 * -- Checks if the page has been scrolled, and shrinks the header.
 */

var $header = $('nav:not(.always-small)');
if( $header.length ) {

    var $window = $(window);
    $window.scroll(function() {
        var scrollPos = $window.scrollTop();
        if( scrollPos !== 0 ) {
            $header.addClass('small');
        } else {
            $header.removeClass('small');
        }
    })
}
