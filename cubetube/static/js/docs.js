var $sidebar = $('.docs-nav');
var turningPoint = $sidebar.offset().top - 55; // added header height.
if( $sidebar.length ) {

    var $window = $(window);
    $window.scroll(function() {
        var scrollPos = $window.scrollTop();
        if( scrollPos > turningPoint ) {
            $sidebar.addClass('fixed');
        } else {
            $sidebar.removeClass('fixed');
        }
    })

    // Set height of sidebar & handle resizes
}