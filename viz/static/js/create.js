
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
});