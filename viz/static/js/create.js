
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
});

function getEditVizType() {

    var type;
    var $onElement = $('.viz-type .on');
    var onVal = $onElement.html();
    return onVal;
}