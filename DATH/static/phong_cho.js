$(document).ready(function(){
function sendRequest(){
    $.ajax({
        url: '',
        type: "POST",
        dataType: 'json',
        contentType: 'application/json',
        success:
        function(result){
        if(result.status == "turnback"){
        location.replace("/");
        }
        },
        complete:function(result){
                  setTimeout(sendRequest,3000);}
});
}
 $(document).ready(function()
  {
     setTimeout(sendRequest,3000);
    });
});
