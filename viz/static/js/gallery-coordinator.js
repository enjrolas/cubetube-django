//this file is in charge of going through the vizs in the gallery and starting each sketch

function activatePreviews(){
  $("canvas.js-preview").each(function(){
	  var id=$(this).attr('id');
	  var canvas=$(this)[0];
	  var type=$(this).attr("type");
	  var source=$("div.code#viz"+id).text();
	  if(type==="javascript")
	      runSketch(canvas, source);
	  if(type==="L3D")
	      runL3DSketch(canvas, source);
      });
}