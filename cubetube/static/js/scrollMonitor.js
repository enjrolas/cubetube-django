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
	var vizCardHeight = $(".viz-card").height();
	var vizGalleryHeight = $("#viz-cards").height();

	$.ajax({
	    type: 'get',
	    url: url,
	    success: function(data) {
			// Here we replace the entire viz gallery code with the code returned by the view:
	  		//$("#viz-in-gallery").replaceWith( data );
			var scrollDiv = $(data).find('#scroll');
			var vizCardsDiv = $(data).find('#viz-cards');
			var totalCards=0;
			var cardsPerRow = window.innerWidth > 1300 ? 4 : 3;
	  		switch(button) {
				case "all":
					$('#viz-cards').html("");
					vizCardsDiv.find('.viz-card').each(function() { $(this).appendTo('#viz-cards').fadeOut(400); totalCards++; })
					var totalRows = totalCards / cardsPerRow;
					$('#viz-cards').animate({height: (vizCardHeight+(cardsPerRow == 4 ? 25 : 30))*totalRows}, 1200);
					$('.viz-card').each(function() { $(this).fadeIn(1200); });
					break;
				case "more":
				case "back":
					vizCardsDiv.find('.viz-card').each(function() { $(this).appendTo('#viz-cards').height(0); totalCards++; })
					var totalRows = totalCards / cardsPerRow;
					$('#viz-cards').height(vizGalleryHeight)
			  		$('#viz-cards').animate({height: "+=" + (vizCardHeight+(cardsPerRow == 4 ? 25 : 30))*totalRows}, 800, "linear");
			  		$('.viz-card').each(function() { $(this).animate({height: vizCardHeight}, 800, "linear"); });
					break;
	  		}
	  		$("#scroll").replaceWith(scrollDiv);
	  		$("#scroll").height(0);
	  		$("#scroll").animate({height: "150"}, 600, "linear");
			setTimeout(function() {
				activatePreviews();
			}, 1);
		}
	})
});