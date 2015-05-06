var library;
var mouseListener="void mouseOut() {noLoop();}void mouseOver() {loop();}";

$.get('/static/js/l3d-library.js', function(data) {
library=data;
	activatePreviews();
    }, 'text');

(function(global) {
   
  function waitForExit() {
    var checkbox = document.getElementById('expect-exit-callback');
    if (!checkbox) {
      return false;
    }
    return checkbox.checked || checkbox.value;
  }

  global.runSketch = function(canvas, sketchCode) {
      console.log("running javascript sketch id#"+canvas.id);
      //      if(canvas.id==="6")
      //	  console.log(sketchCode);
      sketchCode=sketchCode.concat(mouseListener).concat(library);
      sketchCode=sketchCode.replace("size(500,500, P3D);", "size("+canvas.width+","+canvas.height+", P3D);noLoop();");
      var sketch = Processing.compile(sketchCode);
      instance = new Processing(canvas, sketch);
  };

  global.runL3DSketch = function(canvas, sketchCode) {
  try {
      console.log("running L3D sketch id#"+canvas.id);
      var translatedCode=translateCode(sketchCode);
      var sketchCode=translatedCode.concat(mouseListener).concat(library);
      sketchCode=sketchCode.replace("size(500,500, P3D);", "size("+canvas.width+","+canvas.height+", P3D);noLoop();");
      //      if(canvas.id==="3")
      //  console.log(sketchCode);
    var sketch = Processing.compile(sketchCode);
    instance = new Processing(canvas, sketch);
  } catch (e) {
    console.log("Error! Error was:\n" + e.toString());
  }
};

 
}(window));
