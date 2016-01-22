/**
 * Viz Cards
 */

var $cards = $('.viz-card');
var frameTemplate = '<iframe class="viz-template"></iframe>';
var createTemplate = '<iframe class="create-template"></iframe>';
var $body = $(document.body);
var $glass = $('.glass');
//var unbindTimer = null;

$('.create').click(function(e) {
    e.preventDefault();
    openCreate();
})

$('.glass').click(function() {
    closeViz();
    closeCreate();
});

function bindListener() {
    window.addEventListener("message", receiveMessage, false);
}

function receiveMessage(event) {

    // Open code area even more!
    if( event.data === "open" ) {
        $('.viz-template').addClass('more-open');
    } else if (event.data === "close") {
        $('.viz-template').removeClass('more-open');
    }
}

function bindCards() {
    $( "body" ).on( "click", ".viz-card-anchor", function(e) {
      e.preventDefault();
      var $this = $(e.currentTarget);
      openViz( $this.attr('href') );
    });
}

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
        $viz.removeClass('more-open');
        $glass.hide();
        $viz.remove();
        $body.removeClass('no-scroll');
    }
}

function openCreate() {
    var $header = $('nav:not(.always-small)');
    $header.addClass('small');
    
    var frame = $( createTemplate );
    frame.attr('src', '/create/' );

    $body.append( frame );
    $glass.show();
    $body.addClass('no-scroll');
}

function closeCreate() {
    var $viz = $( '.create-template' );
    if( $viz.length ) {
        $glass.hide();
        $viz.remove();
        $body.removeClass('no-scroll');
    }
}

function youtube_parser(url){
    var regExp = /^.*((youtu.be\/)|(v\/)|(\/u\/\w\/)|(embed\/)|(watch\?))\??v?=?([^#\&\?]*).*/;
    var match = url.match(regExp);
    if (match&&match[7].length==11){
        return match[7];
    }else{
        return 'fail';
    }
}

// The YT API will call this function when the video player is ready.
function onPlayerReady(event) {
	var player = event.target;
	player.mute();	//We want sound off as soon as the video is loaded
}

/* Programatically builds a YT video player with the following settings:
 * Autoplay on, no user controls, loop video on, no video info,
 * no related videos on end, lowest video quality, no branding info */
function bindPlayer(videoId, playerId) {
	var player = new YT.Player(playerId, {
		playerVars: { 
			'autoplay': 1,
			'modestbranding': 1,
			'loop': 1,
			'playlist': videoId,			
			'controls': 0,
			'showinfo': 0,
			'rel': 0,
			"vq": 'sd240'
		},
		videoId: videoId,
		events: { 'onReady': onPlayerReady }
    });
}

/* Loads the YT iframe replacing the <div>
 * element with an id with the HTML from YT */
function bindVideoThumbnail( container ) {
	var $videoContainer = container.find('div.interactive-bg-video');
	var vizId = $videoContainer.attr('data-vizId');
	var frame = $videoContainer.find('iframe#player-' + vizId);  
	if(!frame.length) {
    	var url = $videoContainer.attr('data-url');	//This is our YT URL
		var id = youtube_parser(url);				//This is the Video ID part
		var playerId = 'player-' + vizId;			//This is our player's 'id'
		bindPlayer(id, playerId);	//We'll create the video player programatically
		frame = $videoContainer.find('iframe#player-' + vizId);
		if(frame.length) {
			var $playerContainer = container.find('div.interactive-bg');
			//We need to force the iframe to bear our desired attributes
			frame.attr('frameborder', '0');
			frame.attr('allowfullscreen', '0');
			frame.attr('width', $playerContainer.width());
			frame.attr('height', $playerContainer.height());
		}
		container.find('div.interactive-bg').stop().fadeTo(300,0);
	}
}

/* Takes the iframe loaded by YT out,
 * replacing the HTML with a <div> with an id */
function unbindVideoThumbnail( container ) {
	var $videoContainer = container.find('div.interactive-bg-video');
	var vizId = $videoContainer.attr('data-vizId');
	var frame = $videoContainer.find('iframe#player-' + vizId);
	if(frame.length) {	//Try to locate if there's an iframe created by YT
		$videoContainer.html('<div class="player" id="player-' + vizId + '"></div>');
		container.find('div.interactive-bg').stop().fadeTo(300,0.5);
	}
}

function bindVideoPreviews(card) {
	var $vizDemo = card.find('div.viz-demo');
	var $videoContainer = card.find('div.interactive-bg-video'); 
	if($videoContainer.length) {
	    //Bind the YT thumbnail images to each card that has a YT URL attached to it;
		var url = $videoContainer.attr('data-url');	//This is our YT URL
		var id = youtube_parser(url);				//This is the Video ID part
		var thumbUrl = 'url("http://img.youtube.com/vi/' + id + '/mqdefault.jpg")';
		var $playerContainer = $vizDemo.find('div.interactive-bg');
		//Replace the default background with the YT video background image
		$playerContainer.css('background-image',thumbUrl);
		//Adjust the dimensions of the new image to fit the container
		$playerContainer.css('background-size',$playerContainer.width() + 'px ' + $playerContainer.height() + 'px');
		$vizDemo.find('div.play-icon').remove();	//Also remove the 'play arrow' from the preview area (requested by Alex)
	    //Also bind the mouse events to trigger the video loading/unloading
		card.on('mouseover', function() {
			clearTimeout(card.unbindTimer); 
			bindVideoThumbnail($($vizDemo)); 
		}).on('mouseleave', function() {
			card.unbindTimer = setTimeout(function() {
				unbindVideoThumbnail($($vizDemo)); 
			}, 1600); 
		});
	}
}


if( $cards.length ) {
    bindCards();
    bindListener();
	//We need to bind the YT video previews to each viz card
    $cards.each(function() { bindVideoPreviews($(this)); });
    
    var cards = $('.viz-cards');
    if( cards.length )  {
        id = cards.attr('data-viz');
        if( id ) {
            openViz( '/viz/' + id);
        }
    }
}