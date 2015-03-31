function fork (viz)
{
    $.post("{% url 'fork' %}",
	   {"accessToken":accessToken, "vizId":viz},
	   function(data){
	       alert(data);
	   });
}
