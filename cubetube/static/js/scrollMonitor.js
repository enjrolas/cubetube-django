/**
 * Scrollmonitor
 * -- Checks if the page has been scrolled, and shrinks the header.
 */

//var $header = $('nav:not(.always-small)');
var $header = $('nav:not([style~=#1C1521])');
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
	//var vizCardWidth = $(".viz-card").width();
	//var vizGalleryWidth = $("#viz-cards").width();

	$.ajax({
	    type: 'get',
	    url: url,
	    success: function(data) {
			// Here we replace the entire viz gallery code with the code returned by the view:
	  		//$("#viz-in-gallery").replaceWith( data );
			var scrollDiv = $(data).find('#scroll');
			var vizCardsDiv = $(data).find('#viz-cards');
	  		/*switch(button) {
				case "all":
					$('#viz-cards').fadeOut(1200, "linear").html(vizCardsDiv.html()).fadeIn(1200, "swing");
					break;
				case "more":
				case "back":
					$('.viz-card').animate({width: 0}, 1200).animate({width: vizCardWidth}, 1200);
					break;
	  		}*/
			vizCardsDiv.find('.viz-card').each(function() { $(this).appendTo('#viz-cards') })
	  		//$('#viz-cards').replaceWith(vizCardsDiv);
	  		$("#scroll").replaceWith(scrollDiv);
			setTimeout(function() {
				activatePreviews();
			}, 1);
		}
	})
});