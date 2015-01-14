$( document ).ready(function() {$('div#visualizations').jscroll({
    debug: true,
    autoTriggerUntil: 1,
    loadingHtml: '<img src="loading.gif" alt="Loading" /> Loading...',
    padding: 20,
    nextSelector: 'a.scroll:last',
});
			       });
