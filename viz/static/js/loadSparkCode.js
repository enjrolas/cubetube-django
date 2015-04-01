var library;

$(function(){
  $.get('http://127.0.0.1:8000/static/js/l3d-library.js', function(data) {
	  library=data;
      }, 'text');
    
    formatCode();  //applies codeMirror formatting to the code
    runSparkSketch();
    
  function formatCode()
  {
	  var editor = CodeMirror.fromTextArea(document.getElementById("code"), {
		  lineNumbers: true,
		  styleActiveLine: true,
		  matchBrackets: true
	      });
      editor.setOption("theme", "tomorrow-night-eighties");

  }


    });

