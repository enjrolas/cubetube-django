/**
 * Scrollmonitor
 * -- Checks if the page has been scrolled, and shrinks the header.
 */

//var $header = $('nav:not(.always-small)');
var $header = $('nav:not([style~=#1C1521])');
var $window = $(window);
var taglineTimer;

if( $header.length ) {
    $window.scroll(function() {
        checkScroll();
    })
    $("div.tagline").show();
}
else
	$("div.tagline").hide();

$('.logo-box').on('mouseover', function() {
	clearTimeout(taglineTimer);
	if( $header.length ) 
		return false;
	else
		$("div.tagline").slideDown( 400, "swing");
}).on('mouseleave', function() {
	if( $header.length )
		return false;
	else
		taglineTimer = setTimeout(function() { $("div.tagline").slideUp( 600, "swing"); }, 2400);
});

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
    $this.html("Loading<span class=\"one\">.</span><span class=\"two\">.</span><span class=\"three\">.</span>");
    $this.removeClass('blue').addClass('red');
    $('div.scroll').prop( "disabled", true );

    var url = $this.attr('href');
    var button = $this.attr('data-which');
	var vizCardHeight = $(".viz-card").height();
	var vizGalleryHeight = $("#viz-cards").height();
	var scrollHeight = $("#scroll").length ? $("#scroll").height() : 150;
	
	$.ajax({
	    type: 'get',
	    url: url,
	    success: function(data) {
	    	$('div.scroll').prop( "disabled", false );
			var scrollDiv = $(data).find('#scroll');
			var vizCardsDiv = $(data).find('#viz-cards');
			var vizCardBottomMargin = parseInt(vizCardsDiv.find('.viz-card').css("margin-bottom")) + 10;
			var totalRows=0;
			var cardsPerRow=1;	//window.innerWidth > 1300 ? 4 : 3;
	  		switch(button) {
				case "all":
					// Here we replace the entire viz gallery code with the code returned by the view
					$('#viz-cards').html("");
					vizCardsDiv.find('.viz-card').each(function() {
						$(this).appendTo('#viz-cards').fadeOut(400, "linear");
						if($(this).prev().length) {
							if($(this).position().top != $(this).prev().position().top) 
								totalRows++;
							else 
								if(totalRows == 1) cardsPerRow++;
						} else {
							totalRows++;
							cardsPerRow++;
						}
					});
					var debugMsg = "totalRows=" + totalRows + "\ncardsPerRow=" + cardsPerRow; 
					console.log(debugMsg);
					$('#viz-cards').animate({height: ((vizCardHeight+vizCardBottomMargin)*totalRows)}, 1200, "linear");
					$('.viz-card').each(function() { 
						//First, we need to rebind the YT video preview to the viz card
						bindVideoPreviews($(this));
						//Then we fade it in
						$(this).fadeIn(1200, "linear"); 
					});
					break;
				case "more":
				case "back":
					vizCardsDiv.find('.viz-card').each(function() {
				    	//First, we append the code returned by the view to the existing viz gallery code
						$(this).appendTo('#viz-cards').height(0); 
						if($(this).prev().length) {
							if($(this).position().top != $(this).prev().position().top) 
								totalRows++;
							else 
								if(totalRows == 1) cardsPerRow++;
						} else {
							totalRows++;
							cardsPerRow++;
						}
						//Then we need to rebind the YT video preview to the viz card
						bindVideoPreviews($(this));
					});
					var debugMsg = "totalRows=" + totalRows + "\ncardsPerRow=" + cardsPerRow; 
					console.log(debugMsg);
					$('#viz-cards').height(vizGalleryHeight)
			  		$('#viz-cards').animate({height: "+=" + ((vizCardHeight+vizCardBottomMargin)*totalRows)}, 800, "linear");
			  		$('.viz-card').each(function() { $(this).animate({height: vizCardHeight}, 800, "linear"); });
					break;
	  		}
	  		// Here we append the code returned by the view for the bottom blue paging buttons
	  		$("#scroll").replaceWith(scrollDiv);
	  		$("#scroll").height(0);
	  		if($("#scroll").length) 
	  			$("#scroll").animate({height: scrollHeight}, 600, "linear");
  			else
  				$('#viz-cards').animate({height: "+=" + (scrollHeight*.25)});
  			//setTimeout(function() { activatePreviews(); }, 1);
		}
	});
});