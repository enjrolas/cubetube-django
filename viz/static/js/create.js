
$(function(){

    formatCode();
    setHeight();

    // Switch states
    $('.switch-part').click(function() {

        var $this = $( this );
        var $box = $this.closest('.switch-box');

        $on =  $box.find('.on');
        $off = $box.find('.off');

        $on.removeClass('on').addClass('off');
        $off.removeClass('off').addClass('on');
    })

    $('.run-sketch').click(function() {

        var vizType = getEditVizType();
        
        if( vizType === 'L3D') {
            translateCode(document.getElementById("code").value);
            runSparkSketch();
        } else if ( vizType === 'JS' ) {
            runSketch();
        }
    })

    $('.save-code').click(function() {

        var saveData = getData();
        var goodToGo = validate(saveData);
        
        if( goodToGo ) {

            // Ajax request to save!
        }
    });

});

function validate(data) {

    if( data.name === '') {
        showConsoleError("Oh no, you need a viz name!");
        return false;
    }

    if( data.description === '') {
        showConsoleError("Aww, snap, we need a description to save this!");
        return false;
    }

    if( data.code === '') {
        showConsoleError("Hey, we need some code to save this!");
        return false;
    }

    return true;
}

function showConsoleError() {

}

function getEditVizType() {

    var type;
    var $onElement = $('.viz-type .on');
    var onVal = $onElement.html();
    return onVal;
}

function getData() {

    var name = $('.viz-name').val();
    var description = $('.description').val();
    var published = ($('.published .on').html() === 'Public');
    var interactive = ($('.interactive .on').html() === 'Yes');
    var vizType = getEditVizType();
    if( vizType === 'JS') {
        vizType = 'Javascript';
    } else {
        vizType = 'L3D'
    }

    var code = $('.code').val();

    return {
        name: name,
        description: description,
        published: published,
        interactive: interactive,
        "viz-type": vizType,
        code: code
    }
}