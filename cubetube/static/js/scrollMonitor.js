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

$('body').on('click', '.scroll', function(e) {
    e.preventDefault();
    var $this = $(this);
    $this.html("Loading...");
    $this.removeClass('blue').addClass('red');
    var url = $this.attr('href');
    var button = $this.attr('data-which');
    $.ajax({
      type: 'get',
      url: url,
      success: function(data) {
    	if(button == 'all')
    		$("#viz-in-gallery").replaceWith( data );
    	else
    		$("#scroll").replaceWith( data );
        //$this.replaceWith( data );
//	  activatePreviews();
        setTimeout(function() {
            activatePreviews();
        }, 1);
      }
    })
});