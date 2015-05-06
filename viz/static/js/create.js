

var editing = false;

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

            if( editing === false) {

                // // Ajax request to create!
                $.ajax({
                    type: 'post',
                    dataType: 'json',
                    url: '/upload/',
                    data: saveData,
                    success: function( data ) {
                        checkData(data);
                    },
                    error: function( data ) {
                        checkData(data);
                    }
                });

            } else {

                var vizId = $( '.create-wrapper' ).attr('viz-id');

                saveData.vizId = vizId;

                // Ajax request to edit
                $.ajax({
                    type: 'post',
                    dataType: 'json',
                    url: '/save/',
                    data: saveData,
                    success: function( data ) {
                        checkDataAfterSave(data);
                    },
                    error: function( data ) {
                        checkDataAfterSave(data);
                    }
                })
            }
        }
    });

});

function checkDataAfterSave( data ) {
    
    if ( data.success ) {
        alert("Viz Updated!");
    } else {
        console.log( "Error!", data );
    }
}

function checkData( data ) {

    // console.log( data );
    // var data = JSON.parse(data.responseText);
    if ( data.success ) {
        onCreateSuccess( data.id );
    } else {
        console.log( "Error!", data );
    }
}

// Switch to "fork" mode
function onCreateSuccess( id ) {
    
    editing = true;
    $( '.create-wrapper' ).attr('viz-id', id);
    $('.save-code').html('Update Viz');
    alert("Viz Created!");
}

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

function showConsoleError( error ) {
    $('.console').val( error );
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
        sourceCode: code
    }
}