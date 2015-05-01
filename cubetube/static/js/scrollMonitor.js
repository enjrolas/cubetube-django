/**
 * Scrollmonitor
 * -- Checks if the page has been scrolled, and shrinks the header.
 */

var $header = $('nav:not(.always-small)');
var $window = $(window);

if( $header.length ) {
    $window.scroll(function() {
        checkScroll();
    })
}

function checkScroll() {
    var scrollPos = $window.scrollTop();
    if( scrollPos !== 0 ) {
        $header.addClass('small');
    } else {
        $header.removeClass('small');
    }
}

checkScroll();