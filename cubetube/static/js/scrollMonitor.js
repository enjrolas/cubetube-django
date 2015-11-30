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
		$header.css('background', '#1C1521 none repeat scroll 0% 0%');
        //$header.addClass('small');
    } else {
		$header.css('background', 'transparent none repeat scroll 0% 0%');
        //$header.removeClass('small');
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
			// Here we replace the entire viz gallery code with the code returned by the view:
    		$("#viz-in-gallery").replaceWith( data );
    	else
			// Here we replace just the section below the viz gallery with the code returned by the view:
    		$("#scroll").replaceWith( data );
        	setTimeout(function() {
            activatePreviews();
        }, 1);
      }
    })
});